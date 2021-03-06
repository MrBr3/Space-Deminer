/*
** GLIM - OpenGL Immediate Mode
** Copyright Jan Krassnigg (Jan@Krassnigg.de)
** For more details, see the included Readme.txt.
*/

#include "glimBatchData.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace NS_GLIM
{
	GlimArrayData::GlimArrayData ()
	{
		Reset ();	
	}

	void GlimArrayData::Reset (void)
	{
		m_DataType = GLIM_NODATA;
		m_ArrayData.clear ();
		m_ArrayData.reserve (256);

		m_CurrentValue[0].Int = 0;
		m_CurrentValue[1].Int = 0;
		m_CurrentValue[2].Int = 0;
		m_CurrentValue[3].Int = 0;
	}

	void GlimArrayData::PushElement (void)
	{
		switch (m_DataType)
		{
		// 4 byte data
		case GLIM_1I:
		case GLIM_1F:
		case GLIM_4UB:
			m_ArrayData.push_back (m_CurrentValue[0]);
			return;

		// 8 byte data
		case GLIM_2I:
		case GLIM_2F:
			m_ArrayData.push_back (m_CurrentValue[0]);
			m_ArrayData.push_back (m_CurrentValue[1]);
			return;

		// 12 byte data
		case GLIM_3I:
		case GLIM_3F:
			m_ArrayData.push_back (m_CurrentValue[0]);
			m_ArrayData.push_back (m_CurrentValue[1]);
			m_ArrayData.push_back (m_CurrentValue[2]);
			return;

		// 16 byte data
		case GLIM_4I:
		case GLIM_4F:
			m_ArrayData.push_back (m_CurrentValue[0]);
			m_ArrayData.push_back (m_CurrentValue[1]);
			m_ArrayData.push_back (m_CurrentValue[2]);
			m_ArrayData.push_back (m_CurrentValue[3]);
			return;

		default:
			// throws an exception
			GLIM_CHECK (false, "GlimArrayData::PushElement: Data-Type is invalid.");
			return;
		}
	}

	glimBatchData::glimBatchData ()
	{
    m_bUploadedToGPU = false;
		Reset ();
	}

	glimBatchData::~glimBatchData ()
	{
		Reset ();	
	}

	void glimBatchData::Reset (void)
	{
		m_State = STATE_EMPTY;

		m_Attributes.clear ();
		m_PositionData.clear ();
		m_PositionData.reserve (64 * 3);

		m_IndexBuffer_Points.clear ();
		m_IndexBuffer_Points.reserve (16);
		m_IndexBuffer_Lines.clear ();
		m_IndexBuffer_Lines.reserve (32);
		m_IndexBuffer_Triangles.clear ();
		m_IndexBuffer_Triangles.reserve (128);


		if (m_bUploadedToGPU)
		{
			m_bUploadedToGPU = false;

			glDeleteBuffers (1, &m_uiVertexBufferID);
			glDeleteBuffers (1, &m_uiElementBufferID_Points);
			glDeleteBuffers (1, &m_uiElementBufferID_Lines);
			glDeleteBuffers (1, &m_uiElementBufferID_Triangles);
		}

		m_fMinX = 99999999.0f;
		m_fMaxX =-99999999.0f;
		m_fMinY = 99999999.0f;
		m_fMaxY =-99999999.0f;
		m_fMinZ = 99999999.0f;
		m_fMaxZ =-99999999.0f;
	}

	unsigned int glimBatchData::AddVertex (float x, float y, float z)
	{
		if (x < m_fMinX)
			m_fMinX = x;
		if (x > m_fMaxX)
			m_fMaxX = x;

		if (y < m_fMinY)
			m_fMinY = y;
		if (y > m_fMaxY)
			m_fMaxY = y;

		if (z < m_fMinZ)
			m_fMinZ = z;
		if (z > m_fMaxZ)
			m_fMaxZ = z;

		m_PositionData.push_back (x);
		m_PositionData.push_back (y);
		m_PositionData.push_back (z);

		std::map<std::string, GlimArrayData>::iterator it, itend;
		itend = m_Attributes.end ();

		for (it = m_Attributes.begin (); it != itend; ++it)
			it->second.PushElement ();

		return ((m_PositionData.size () / 3) - 1);
	}

	unsigned int glimBatchData::getVertexDataSize (void) const
	{
		unsigned int uiVertexDataSize = sizeof (float) * 3;

		std::map<std::string, GlimArrayData>::const_iterator it, itend;
		itend = m_Attributes.end ();

		for (it = m_Attributes.begin (); it != itend; ++it)
		{
			switch (it->second.m_DataType)
			{
			// 4 byte data
			case GLIM_1I:
			case GLIM_1F:
			case GLIM_4UB:
				uiVertexDataSize += sizeof (int) * 1;
				break;

			// 8 byte data
			case GLIM_2I:
			case GLIM_2F:
				uiVertexDataSize += sizeof (int) * 2;
				break;

			// 12 byte data
			case GLIM_3I:
			case GLIM_3F:
				uiVertexDataSize += sizeof (int) * 3;
				break;

			// 16 byte data
			case GLIM_4I:
			case GLIM_4F:
				uiVertexDataSize += sizeof (int) * 4;
				break;

			default:
				// throws an exception
				GLIM_CHECK (false, "glimBatchData::Upload: Data-Type is invalid.");
				break;
			}
		}

		return (uiVertexDataSize);
	}

	int glimBatchData::getVertexAttribBindPoint (unsigned int uiCurrentProgram, const char* szAttribName) const
	{
		int iLocation = glGetAttribLocation (uiCurrentProgram, szAttribName);

		return (iLocation);
	}

	void glimBatchData::Unbind (void)
	{
		// make sure no VAO is bound, such that GLIM does not accidently change it's state

		// if the pointer is != NULL, the extension should be available
		if (glBindVertexArray)
		{
			glBindVertexArray (0);
		}

		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

		for (int i = 0; i < 16; ++i)
			glDisableVertexAttribArray (i);
	}

	void glimBatchData::Bind (unsigned int uiCurrentProgram)
	{
		if (!m_bUploadedToGPU)
			return;

		// make sure no VAO is bound, such that GLIM does not accidently change it's state

		// if the pointer is != NULL, the extension should be available
		if (glBindVertexArray)
		{
			glBindVertexArray (0);
		}

		glBindBuffer (GL_ARRAY_BUFFER, m_uiVertexBufferID);

		std::map<std::string, GlimArrayData>::iterator it, itend;
		itend = m_Attributes.end ();

		for (it = m_Attributes.begin (); it != itend; ++it)
		{
			const int iAttributeArray = getVertexAttribBindPoint (uiCurrentProgram, it->first.c_str ());

			if (iAttributeArray < 0)
				continue;

			glEnableVertexAttribArray (iAttributeArray);

			switch (it->second.m_DataType)
			{
			case GLIM_1F:
				glVertexAttribPointer (iAttributeArray, 1, GL_FLOAT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_2F:
				glVertexAttribPointer (iAttributeArray, 2, GL_FLOAT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_3F:
				glVertexAttribPointer (iAttributeArray, 3, GL_FLOAT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_4F:
				glVertexAttribPointer (iAttributeArray, 4, GL_FLOAT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_1I:
				glVertexAttribPointer (iAttributeArray, 1, GL_INT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_2I:
				glVertexAttribPointer (iAttributeArray, 2, GL_INT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_3I:
				glVertexAttribPointer (iAttributeArray, 3, GL_INT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_4I:
				glVertexAttribPointer (iAttributeArray, 4, GL_INT, false, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			case GLIM_4UB:
				glVertexAttribPointer (iAttributeArray, 4, GL_UNSIGNED_BYTE, true, 0, BUFFER_OFFSET (it->second.m_uiBufferOffset));
				break;
			}
		}

		// set the pointer for position last
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (0, 3, GL_FLOAT, false, 0, BUFFER_OFFSET (0));
	}

	void glimBatchData::Upload (void)
	{
		if (m_bUploadedToGPU)
			return;

		if (m_PositionData.empty ())
			return;

		m_bUploadedToGPU = true;


		// make sure no VAO is bound, such that GLIM does not accidently change it's state

		// if the pointer is != NULL, the extension should be available
		if (glBindVertexArray)
		{
			glBindVertexArray (0);
		}


		const unsigned int uiVertexDataSize = getVertexDataSize ();
		const unsigned int uiVertices = m_PositionData.size () / 3;


		glGenBuffers (1, &m_uiVertexBufferID);
		glBindBuffer (GL_ARRAY_BUFFER, m_uiVertexBufferID);
		glBufferData (GL_ARRAY_BUFFER, uiVertices * uiVertexDataSize, NULL, GL_STATIC_DRAW);

		// first upload the position data
		unsigned int uiOffset = uiVertices * sizeof (float) * 3;

		// offset is zero, but can use uiOffset as size
		glBufferSubData (GL_ARRAY_BUFFER, 0, uiOffset, &m_PositionData[0]);
		// the buffer in RAM can be cleared now
		m_PositionData.clear ();


		// now upload each attribute array one after another

		std::map<std::string, GlimArrayData>::iterator it, itend;
		itend = m_Attributes.end ();

		for (it = m_Attributes.begin (); it != itend; ++it)
		{
			const unsigned int uiAttributeSize = it->second.m_ArrayData.size () * 4; // already includes the number of vertices

			// upload the data
			glBufferSubData (GL_ARRAY_BUFFER, uiOffset, uiAttributeSize, &it->second.m_ArrayData[0].Float);

			// free the temporary buffer in RAM, the data is now in a better place (the GPU)
			it->second.m_ArrayData.clear ();

			// store the buffer offset for later use
			it->second.m_uiBufferOffset = uiOffset;

			// increase the buffer offset
			uiOffset += uiAttributeSize;
		}

		m_uiPointElements = 0;
		m_uiLineElements = 0;
		m_uiTriangleElements = 0;

		// upload the index buffer for the points
		glGenBuffers (1, &m_uiElementBufferID_Points);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_uiElementBufferID_Points);

		if (!m_IndexBuffer_Points.empty ())
		{
			m_uiPointElements = m_IndexBuffer_Points.size ();
			glBufferData (GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer_Points.size () * 4, &m_IndexBuffer_Points[0], GL_STATIC_DRAW);
			m_IndexBuffer_Points.clear ();
		}
			
		// upload the index buffer for the lines
		glGenBuffers (1, &m_uiElementBufferID_Lines);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_uiElementBufferID_Lines);

		if (!m_IndexBuffer_Lines.empty ())
		{
			m_uiLineElements = m_IndexBuffer_Lines.size ();
			glBufferData (GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer_Lines.size () * 4, &m_IndexBuffer_Lines[0], GL_STATIC_DRAW);
			m_IndexBuffer_Lines.clear ();
		}


		// upload the index buffer for the triangles
		glGenBuffers (1, &m_uiElementBufferID_Triangles);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_uiElementBufferID_Triangles);

		if (!m_IndexBuffer_Triangles.empty ())
		{
			m_uiTriangleElements = m_IndexBuffer_Triangles.size ();
			glBufferData (GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer_Triangles.size () * 4, &m_IndexBuffer_Triangles[0], GL_STATIC_DRAW);
			m_IndexBuffer_Triangles.clear ();
		}
	}
}



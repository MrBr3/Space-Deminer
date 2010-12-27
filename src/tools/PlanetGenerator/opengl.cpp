#include "opengl.hpp"

#include <GL/glx.h>
#include <GL/glxext.h>

PFNGLGENBUFFERSARBPROC glGenBuffersARB  = nullptr;
PFNGLBINDBUFFERARBPROC glBindBufferARB  = nullptr;
PFNGLBUFFERDATAARBPROC glBufferDataARB  = nullptr;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB  = nullptr;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB  = nullptr;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB  = nullptr;

#define glGetProcAddress(what) glXGetProcAddress((const GLubyte*)what)

void init_opengl_stuff(int argc, char **argv)
{
  Gtk::GL::init(argc, argv);

	glGenBuffersARB           = (PFNGLGENBUFFERSARBPROC)          glGetProcAddress("glGenBuffersARB");
	glBindBufferARB           = (PFNGLBINDBUFFERARBPROC)          glGetProcAddress("glBindBufferARB");
	glBufferDataARB           = (PFNGLBUFFERDATAARBPROC)          glGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB        = (PFNGLDELETEBUFFERSARBPROC)       glGetProcAddress("glDeleteBuffersARB");
	glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)       glGetProcAddress("glActiveTextureARB");
	glClientActiveTextureARB  = (PFNGLCLIENTACTIVETEXTUREARBPROC) glGetProcAddress("glClientActiveTextureARB");
}
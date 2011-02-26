/*
** GLIM - OpenGL Immediate Mode
** Copyright Jan Krassnigg (Jan@Krassnigg.de)
** For more details, see the included Readme.txt.
*/

#include "Main.h"
#include <iostream>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace NS_GLIM
{
	using namespace std;

	GLIM_CALLBACK GLIM_Interface::s_StateChangeCallback = NULL;

	glimException::glimException (const string &err) : runtime_error (err)
	{
		//printf (err.c_str ());
		cerr << err;

#ifdef WIN32
		MessageBox (NULL, err.c_str (), "GLIM - Error", MB_ICONERROR);
#endif
	}



}

#include <base.hpp>
#include <GL/glew.h>
#include <gtkglmm.h>

#undef glGenBuffersARB
#undef glBindBufferARB
#undef glBufferDataARB
#undef glDeleteBuffersARB
#undef glActiveTextureARB
#undef glClientActiveTextureARB

extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;

void init_opengl_stuff(int argc, char **argv);
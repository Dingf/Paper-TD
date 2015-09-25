#ifndef INC_OPEN_GL_H
#define INC_OPEN_GL_H

#if ((defined(__MACH__)) && (defined(__APPLE__)))
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glext.h>

#pragma comment (lib, "glew32.lib")
#pragma comment (linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include "glmacro.h"

#endif
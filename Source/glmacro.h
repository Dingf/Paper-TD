#ifndef INC_GL_MACRO_H
#define INC_GL_MACRO_H

#include "opengl.h"
#include "vector.h"
#include "color.h"

void gluLookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up);
void glTranslatev(const Vector3D& v);

void glColor(ColorARGB color);

void Enable2D();
void Enable3D();

void SetOrthographic();
void SetPerspective();

bool CheckGLExtension(const char * extension);

#endif
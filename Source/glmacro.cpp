#include "opengl.h"
#include "type.h"
#include "vector.h"
#include "color.h"
#include "glmacro.h"
#include "render.h"

void gluLookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up)
{
	gluLookAt(eye.GetX(), eye.GetY(), eye.GetZ(), 
		      center.GetX(), center.GetY(), center.GetZ(),
			  up.GetX(), up.GetY(), up.GetZ());
}

void glTranslatev(const Vector3D& v)
{
	glTranslated(v.GetX(), v.GetY(), v.GetZ());
}

void glColor(ColorARGB color)
{
	glColor4f(color.GetRed()/255.0f, color.GetGreen()/255.0f, color.GetBlue()/255.0f, color.GetAlpha()/255.0f);
}

void Enable2D()
{
	glDisable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);
	SetOrthographic();
}

void Enable3D()
{
	SetPerspective();
	glEnable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);
}

void SetOrthographic()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(0.0, -screenHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspective()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

bool CheckGLExtension(const char * extension)
{
	const char * extString = (const char *)glGetString(GL_EXTENSIONS);

	if ((strchr(extension, ' ') != false) || (extString == NULL))
	{
		return false;
	}

	const char * beginning = extString;

	for (;;)
	{
		const char * result = strstr(beginning, extension);
		if (result == NULL)
		{
			break;
		}

		const char * end = result + strlen(extension);
		if ((result == beginning) || (*(result - 1) == ' '))
		{
			if ((*end == ' ') || (*end == '\0'))
			{
				return true;
			}
		}
		beginning = end;
	}

	return false;
}
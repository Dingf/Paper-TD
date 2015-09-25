#include "opengl.h"
#include "type.h"
#include "utility.h"
#include "rendergame.h"
#include "rendergui.h"
#include "render.h"
#include "game.h"

int32 screenWidth;
int32 screenHeight;

void DisplayInit()
{
	InitFreeType();
	GameDisplayInit();
	GUIDisplayInit();
}

void RenderFrame() 
{
	Enable2D();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (WorldGame.GetScreen())
	{
		case SCREEN_GAMEPLAY:
		{
			DrawGameplay();
			break;
		}
	}

	glutSwapBuffers();
}

void ResizeWindow(int32 width, int32 height)
{
	//Prevents a divide by 0 when calculating ratio
	height = MinFit(height, 1);

	screenWidth = width;
	screenHeight = height;
	Float ratio = 1.0f * screenWidth/screenHeight;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, screenWidth, screenHeight);

	// Set the clipping volume
	
	//gluPerspective(45, ratio, 0.1, 1000);

	// setting the camera now
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//UpdateView();
}

void DisplayUnload()
{
	GUIDisplayUnload();
	GameDisplayUnload();
	UnloadFreeType();
}
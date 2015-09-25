#include <stdlib.h>
#include <process.h>
#include "opengl.h"
#include "type.h"
#include "main.h"
#include "rand.h"
#include "keyboard.h"
#include "mouse.h"
#include "render.h"
#include "game.h"

//Delete me soon
#include "tower.h"
#include "script.h"

/*Debugging Memory Leaks:
-----------------------------------------
To debug memory leaks, first uncomment the header file, <crtdbg.h>, below. Then, go to Project>
Paper TD Properties>Configuration Properties>C/C++>Preprocessor. Under the preprocessor definitions,
add _DEBUG and rebuild the solution. Then go to Code Generation and change the Runtime Library to
/MTd if it is not so already. Uncomment _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
from main() and run the program in debug mode.
*/
#include <crtdbg.h>



void UpdateStates()
{
	UpdateGame();
	HandleSpecialKeyPressed();
	HandleNormalKeyPressed();
}

void ProcessFrame()
{
	UpdateStates();	
	RenderFrame();
}

void GLInit() 
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Paper Tower Defense");
	
	glutGameModeString("1024x768:32@60");
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE) == true)
	{
		//glutEnterGameMode();
	}
	else 
	{
		//Exit();
	}

	glutIgnoreKeyRepeat(true);
	glutKeyboardFunc(HandleNormalKeyDown);
	glutSpecialFunc(HandleSpecialKeyDown);
	glutSpecialUpFunc(HandleSpecialKeyUp);
	glutPassiveMotionFunc(HandlePassiveMouseMotion);
	glutMotionFunc(HandleMouseMotion);
	glutMouseFunc(HandleMouseEvent);
	glutDisplayFunc(ProcessFrame);
	glutIdleFunc(ProcessFrame);
	glutReshapeFunc(ResizeWindow);

	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WorldInit()
{
	TimeSeedGenerator();
	DisplayInit();
	InitGame();
	LoadGame("Scenarios\\default.xml");
}

int main(int argc, char **argv)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	glutInit(&argc, argv);
	glewInit();

	GLInit();
	WorldInit();

	glutMainLoop();

	return 0;
}

void Exit()
{
	glutLeaveGameMode();
	DisplayUnload();
	UnloadGame();

	exit(0);
}
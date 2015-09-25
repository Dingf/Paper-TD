#include "main.h"
#include "keyboard.h"
#include "keyboardhandler.h"

using namespace KeyboardNamespace;

bool KeyboardNamespace::TakeNoAction()
{
	return true;
}

bool KeyboardNamespace::ExitKey()
{
	Exit();
	return true;
}
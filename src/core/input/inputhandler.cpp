#include "core/input/inputhandler.h"
#include "core/input/inputcontext.h"




InputHandler::~InputHandler() {

	if (context) {
		context->unlinkHandler();
	}

}
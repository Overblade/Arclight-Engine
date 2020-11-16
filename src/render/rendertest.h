#pragma once

#include "render/gle/gle.h"


class RenderTest {

public:

	void create();
	void run();
	void destroy();

	void resizeWindowFB(u32 w, u32 h);

private:
	GLE::VertexBuffer vertexBuffer;
	GLE::VertexArray vertexArray;
	GLE::ShaderProgram shader;

};
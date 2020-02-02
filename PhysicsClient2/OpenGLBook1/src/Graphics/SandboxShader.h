#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class SandboxShader {
public:
	GLuint Program;

	SandboxShader(const GLchar* vertexSorucePath, const GLchar* fragmentSourcePath);

	void Use();
};

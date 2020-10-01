#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

/// <summary>
/// Class that will saves a vertex shader and fragment shader
/// and attaches them to a OpenGL program for rendering
/// </summary>
class Shader {
public:
	/// <summary>
	/// The openGL program made by glCreateProgram that is 
	/// used for rendering
	/// </summary>
	GLuint Program;

	/// <summary>
	/// Takes in paths to the shaders to load and attach to OpenGL program
	/// </summary>
	Shader(const GLchar* vertexSorucePath, const GLchar* fragmentSourcePath);

	/// <summary>
	/// Calls glUseProgram on the Program. TODO: see what this does.
	/// </summary>
	void Use();
};

#pragma once
#include<fstream>
#include<string>
#include<sstream>
#include<iostream>
#include<GLAD/glad.h>

#define VERTEX_SHADER 0
#define FRAGMENT_SHADER 1

class ShaderClass
{
	struct ShaderSource
	{
		std::string Vertex;
		std::string Fragment;
	};
	ShaderSource shaderSource;
	GLuint vertexShader, fragmentShader;
	ShaderSource ReadShader(const std::string& filePath);
	void MakeShader(const char type);
	void MakeProgram();

public:
	GLuint ID;
	ShaderClass(const std::string& filePaht);
	~ShaderClass();
	void Activate();
	void Delete();
};


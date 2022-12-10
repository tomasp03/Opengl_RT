#include "ShaderClass.h"

ShaderClass::ShaderSource ShaderClass::ReadShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;

			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[int(type)] << line << '\n';
		}
	}

	return { ss[0].str(),ss[1].str() };
}

ShaderClass::ShaderClass(const std::string& filePath)
{
	shaderSource = ReadShader(filePath);
	MakeShader(VERTEX_SHADER);
	MakeShader(FRAGMENT_SHADER);
	MakeProgram();
}

ShaderClass::~ShaderClass()
{
	Delete();
}

void ShaderClass::MakeShader(const char type)
{
	if (type == VERTEX_SHADER)
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* cs = shaderSource.Vertex.c_str();
		glShaderSource(vertexShader, 1, &cs, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else if (type == FRAGMENT_SHADER)
	{
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* cv = shaderSource.Fragment.c_str();
		glShaderSource(fragmentShader, 1, &cv, NULL);
		glCompileShader(fragmentShader);

		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
}

void ShaderClass::MakeProgram()
{
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void ShaderClass::Activate()
{
	glUseProgram(ID);
}

void ShaderClass::Delete()
{
	glDeleteProgram(ID);
}


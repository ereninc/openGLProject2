#include "shaderprogram.hpp"
#include <glad/glad.h>
#include <fstream>

using namespace std;

ShaderProgram::ShaderProgram() {
	m_ProgramId = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_ProgramId);
}

void ShaderProgram::link() {
	glLinkProgram(m_ProgramId);
}

void ShaderProgram::use() {
	glUseProgram(m_ProgramId);
}

void ShaderProgram::addUniform(const string& varName) {
	m_UniformVars[varName] = glGetUniformLocation(m_ProgramId, varName.c_str());
}

void ShaderProgram::setFloat(const string& varName, float value) {
	glUniform1f(m_UniformVars[varName], value);
}

void ShaderProgram::attachShader(const char* fileName, unsigned int shaderType) {
	unsigned int shaderId = glCreateShader(shaderType);
	string sourceCode = getShaderFromFile(fileName);
	const char* chSourceCode = &sourceCode[0];
	glShaderSource(shaderId, 1, &chSourceCode,0);
	glCompileShader(shaderId);
	glAttachShader(m_ProgramId, shaderId);

	glDeleteShader(shaderId);
}

string ShaderProgram::getShaderFromFile(const char* fileName) {
	ifstream file(fileName);
	string data;
	
	if (file.is_open())
	{
		char readChar;
		while ((readChar = file.get()) != EOF)
		{
			data += readChar;
		}
		file.close();
	}
	return data;
}
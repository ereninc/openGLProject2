#pragma once
#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <string>
#include <map>

using namespace std;
class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();
	void attachShader(const char* fileName, unsigned int shaderType);

	void link();

	void use();

	void addUniform(const string& varName);

	void setFloat(const string& varName, float value);

private:
	string getShaderFromFile(const char* fileName);

	map<string, unsigned int> m_UniformVars;

private:
	unsigned int m_ProgramId;
};

#endif
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//This class will compile build and link our Shaders

class Shader
{
public:
	//Program ID
	unsigned int ID;

	//Constructor, which reads and builds the shaders
	Shader(const char* vertexPath, const char* fragmentPath) {
		//Retriver vertex and fragment source from the file path
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//read the file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//close file handlers
			vShaderFile.close();
			fShaderFile.close();
			//convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//Compile the shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		//vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		//print errors
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILE_ERROR of type \n" << infoLog << std::endl;
		}
		//fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		//print errors
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILE_ERROR of type: \n" << infoLog << std::endl;
		}
		//Create Shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//print errors
		glGetShaderiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED of type \n" << infoLog << std::endl;
		}
		//delete Shaders once linked to the program
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	// use/activate the shaders
	void use() {
		glUseProgram(ID);
	}
	//Basic uniform functions integration
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
};
#endif


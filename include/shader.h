#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int id;
	
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::fstream vertexFile;
		std::fstream fragmentFile;
		
		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		
		try {
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);
			std::stringstream vertexStream;
			std::stringstream fragmentStream;
			
			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();
			
			vertexFile.close();
			fragmentFile.close();
			
			vertexCode = vertexStream.str();
			fragmentCode = fragmentStream.str();
		} catch(std::ifstream::failure e) {
			std::cout << "Error: Shader file could not be loaded" << std::endl;
		}
		
		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();
		
		unsigned int vertex;
		unsigned int fragment;
		int success;
		char infoLog[512];
		
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "Error: Vertex shader compilation failed\n" << infoLog << std::endl;
		}
		
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "Error: Fragment shader compilation failed\n" << infoLog << std::endl;
		}
		
		id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "Error: Shader program linking failed\n" << infoLog << std::endl;
		}
		
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	
	void Use() {
		glUseProgram(id);
	}
	
	void SetBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}
	
	void SetInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}
	
	void SetFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}
	
	void SetVec2(const std::string &name, float value[2]) const {
		glUniform2f(glGetUniformLocation(id, name.c_str()), value[0], value[1]);
	}
	
	void SetVec3(const std::string &name, float value[3]) const {
		glUniform3f(glGetUniformLocation(id, name.c_str()), value[0], value[1], value[2]);
	}
	
	void SetVec4(const std::string &name, float value[4]) const {
		glUniform4f(glGetUniformLocation(id, name.c_str()), value[0], value[1], value[2], value[3]);
	}
	
	void SetMat4(const std::string &name, glm::mat4 value) {
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
};
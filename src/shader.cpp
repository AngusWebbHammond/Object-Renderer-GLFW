#pragma once

#include "shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Public
namespace ObjectRenderer {

    Shader::Shader()
    {
        m_trans = glm::mat4(1.0f);
    }

    Shader::~Shader()
    {
        deleteShaders();
    }

    void Shader::init(const char* vertexPath, const char* fragmentPath)
    {
        createShaderFromFile(vertexPath, fragmentPath);
        generateShaderLogs();
    }

    void Shader::use()
    {
        glUseProgram(m_shaderProgram);
        m_trans = glm::rotate(m_trans, glm::radians(0.1f), glm::vec3(0.0, 1.0, 0.0));
        m_trans = glm::rotate(m_trans, glm::radians(0.03f), glm::vec3(1.0, 0.0, 0.0));
        // m_trans = glm::rotate(m_trans, glm::radians(0.1f), glm::vec3(0.0, 0.0, 0.0));
        unsigned int transformLoc = glGetUniformLocation(m_shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_trans));
    }
}

// Private
namespace ObjectRenderer {

    void Shader::generateShaderLogs() {
        m_logLengthVert, m_logLengthFrag = 0;
        glGetShaderInfoLog(m_vertexShader, 1024, &m_logLengthVert, m_messageVert);
        glGetShaderInfoLog(m_fragmentShader, 1024, &m_logLengthFrag, m_messageFrag);

        std::cout << m_messageVert << std::endl;
        std::cout << m_messageFrag << std::endl;
    }

    void Shader::deleteShaders() {
        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);
    }

    void Shader::createShaderFromFile(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vertexShaderCode = vertexCode.c_str();
        const char* fragmentShaderCode = fragmentCode.c_str();

        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(m_vertexShader);

        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(m_fragmentShader);

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, m_vertexShader);
        glAttachShader(m_shaderProgram, m_fragmentShader);
        glLinkProgram(m_shaderProgram);
    }
}
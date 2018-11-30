#pragma once

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>


// Utitlity class to load, compile and attach a vertex- and fragment shader to a program
class ShaderUtil
{

private:
    unsigned int mProgramId;

    unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source) {
        unsigned int shader_id = glCreateShader(shader_type);

        const char* c_source = shader_source.c_str();
        glShaderSource(shader_id, 1, &c_source, nullptr);
        glCompileShader(shader_id);

        GLint result;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

            GLchar* strInfoLog = new GLchar[length + 1];
            glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

            fprintf(stderr, "Compilation error in shader: %s\n", strInfoLog);
            delete[] strInfoLog;
        }

        return shader_id;
    }

public:
    ShaderUtil() {}
    ~ShaderUtil() {}

    // Load a vertex and a fragment shader from file
    bool Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
        std::ifstream is_vs(vertexShaderFile);
        const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

        std::ifstream is_fs(fragmentShaderFile);
        const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

        mProgramId = glCreateProgram();

        unsigned int vs = GetCompiledShader(GL_VERTEX_SHADER, f_vs);
        unsigned int fs = GetCompiledShader(GL_FRAGMENT_SHADER, f_fs);

        glAttachShader(mProgramId, vs);
        glAttachShader(mProgramId, fs);

        glLinkProgram(mProgramId);
        glValidateProgram(mProgramId);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return true;
    }

    // Use the program
    void Use() {
        glUseProgram(mProgramId);
    }

    // Delete the program
    void Delete() {
        glDeleteProgram(mProgramId);
    }
};

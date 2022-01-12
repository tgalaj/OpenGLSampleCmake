/** 
 * Copyright (C) 2018 Tomasz Ga³aj
 **/

#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>
#include <helpers/RootDir.h>

Shader::Shader(const std::string & vertexShaderFilename,
               const std::string & fragmentShaderFilename,
               const std::string & geometryShaderFilename, 
               const std::string & tessellationControlShaderFilename, 
               const std::string & tessellationEvaluationShaderFilename) 
               : program_id(0), 
                 isLinked(false)
{
    const std::string shaderCodes[5] = { loadFile(vertexShaderFilename), 
                                         loadFile(fragmentShaderFilename), 
                                         loadFile(geometryShaderFilename),
                                         loadFile(tessellationControlShaderFilename),
                                         loadFile(tessellationEvaluationShaderFilename) };

    const std::string filenames[5] = { vertexShaderFilename, 
                                       fragmentShaderFilename, 
                                       geometryShaderFilename,
                                       tessellationControlShaderFilename,
                                       tessellationEvaluationShaderFilename };

    program_id = glCreateProgram();

    if (program_id == 0)
    {
        fprintf(stderr, "Error while creating program object.\n");
        printf("Press any key to continue...\n");
        getchar();
        return;
    }

    for (int i = 0; i < sizeof(shaderCodes) / sizeof(std::string); ++i)
    {
        if (shaderCodes[i].empty())
        {
            continue;
        }

        GLuint shaderType = 0;
        
        if (i == 0)
            shaderType = GL_VERTEX_SHADER;
        else
        if (i == 1)
            shaderType = GL_FRAGMENT_SHADER;
        else
        if (i == 2)
            shaderType = GL_GEOMETRY_SHADER;
        else
        if (i == 3)
            shaderType = GL_TESS_CONTROL_SHADER;
        else
        if (i == 4)
            shaderType = GL_TESS_EVALUATION_SHADER;

        if (shaderType == 0)
        {
            fprintf(stderr, "Error! Wrong shader type.\n");
            continue;
        }

        GLuint shaderObject = glCreateShader(shaderType);

        if (shaderObject == 0)
        {
            fprintf(stderr, "Error while creating %s.\n", filenames[i].c_str());
            continue;
        }

        const char *shaderCode[1] = { shaderCodes[i].c_str() };

        glShaderSource (shaderObject, 1, shaderCode, nullptr);
        glCompileShader(shaderObject);

        GLint result;
        glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE)
        {
            fprintf(stderr, "%s compilation failed!\n", filenames[i].c_str());

            GLint logLen;
            glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLen);

            if (logLen > 0)
            {
                char * log = (char *)malloc(logLen);

                GLsizei written;
                glGetShaderInfoLog(shaderObject, logLen, &written, log);

                fprintf(stderr, "Shader log: \n%s", log);
                free(log);
            }

            continue;
        }

        glAttachShader(program_id, shaderObject);
        glDeleteShader(shaderObject);
    }

    link();
}

Shader::~Shader()
{
    if (program_id != 0)
    {
        glDeleteProgram(program_id);
        program_id = 0;
    }
}

bool Shader::link()
{
    glLinkProgram(program_id);

    GLint status;
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        fprintf(stderr, "Failed to link shader program!\n");

        GLint logLen;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &logLen);

        if (logLen > 0)
        {
            char* log = (char*)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog(program_id, logLen, &written, log);

            fprintf(stderr, "Program log: \n%s", log);
            free(log);
        }
    }
    else
    {
        isLinked = true;
    }

    return isLinked;
}

void Shader::apply()
{
    if (program_id != 0 && isLinked)
    {
        glUseProgram(program_id);
    }
}

bool Shader::getUniformLocation(const std::string & uniform_name)
{
    GLint uniform_location = glGetUniformLocation(program_id, uniform_name.c_str());
    
    if (uniform_location != -1)
    {
        uniformsLocations[uniform_name] = uniform_location;
        return true;
    }
    else
    {
        fprintf(stderr, "Error! Can't find uniform %s\n", uniform_name.c_str());
        return false;
    }
}

std::string Shader::loadFile(const std::string & filename)
{
    if (filename.empty())
    {
        return "";
    }

    std::string filetext;
    std::string line;

    std::ifstream inFile(ROOT_DIR "res/shaders/" + filename);

    if (!inFile)
    {
        fprintf(stderr, "Could not open file %s", filename.c_str());
        inFile.close();

        return "";
    }
    else
    {
        while (getline(inFile, line))
        {
            filetext.append(line + "\n");
        }

        inFile.close();

        return filetext;
    }
}

void Shader::setUniform1f(const std::string & uniformName, float value)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform1f(program_id, uniformsLocations[uniformName], value);
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniform1f(program_id, uniformsLocations[uniformName], value);
    }
}

void Shader::setUniform1i(const std::string & uniformName, int value)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform1i(program_id, uniformsLocations[uniformName], value);
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniform1i(program_id, uniformsLocations[uniformName], value);
    }
}

void Shader::setUniform1ui(const std::string & uniformName, unsigned int value)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform1ui(program_id, uniformsLocations.at(uniformName), value);
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniform1ui(program_id, uniformsLocations[uniformName], value);
    }
}

void Shader::setUniform1fv(const std::string & uniformName, GLsizei count, float * value)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform1fv(program_id, uniformsLocations[uniformName], count, value);
    }
    else
    {
        if (getUniformLocation(uniformName))
        {
            glProgramUniform1fv(program_id, uniformsLocations[uniformName], count, value);
        }
    }
}

void Shader::setUniform1iv(const std::string & uniformName, GLsizei count, int * value)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform1iv(program_id, uniformsLocations[uniformName], count, value);
    }
    else
    {
        if (getUniformLocation(uniformName))
        {
            glProgramUniform1iv(program_id, uniformsLocations[uniformName], count, value);
        }
    }
}

void Shader::setUniform2fv(const std::string & uniformName, const glm::vec2 & vector)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform2fv(program_id, uniformsLocations[uniformName], 1, glm::value_ptr(vector));
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniform2fv(program_id, uniformsLocations[uniformName], 1, glm::value_ptr(vector));
    }
}

void Shader::setUniform3fv(const std::string & uniformName, const glm::vec3 & vector)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform3fv(program_id, uniformsLocations[uniformName], 1, glm::value_ptr(vector));
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniform3fv(program_id, uniformsLocations[uniformName], 1, glm::value_ptr(vector));
    }
}

void Shader::setUniform4fv(const std::string & uniformName, const glm::vec4 & vector)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniform4fv(program_id, uniformsLocations[uniformName], 1, glm::value_ptr(vector));
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniform4fv(program_id, uniformsLocations[uniformName], 1, glm::value_ptr(vector));
    }
}

void Shader::setUniformMatrix3fv(const std::string & uniformName, const glm::mat3 & matrix)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniformMatrix3fv(program_id, uniformsLocations[uniformName], 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniformMatrix3fv(program_id, uniformsLocations[uniformName], 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::setUniformMatrix4fv(const std::string & uniformName, const glm::mat4 & matrix)
{
    if (uniformsLocations.count(uniformName))
    {
        glProgramUniformMatrix4fv(program_id, uniformsLocations[uniformName], 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if (getUniformLocation(uniformName))
    {
        glProgramUniformMatrix4fv(program_id, uniformsLocations[uniformName], 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

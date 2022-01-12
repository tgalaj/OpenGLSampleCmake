/** 
 * Copyright (C) 2018 Tomasz Ga³aj
 **/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <string>

class Shader
{
public:
    Shader(const std::string & vertexShaderFilename,
           const std::string & fragmentShaderFilename,
           const std::string & geometryShaderFilename               = "",
           const std::string & tessellationControlShaderFilename    = "",
           const std::string & tessellationEvaluationShaderFilename = "");

    virtual ~Shader();

    void setUniform1f       (const std::string & uniformName, float value);
    void setUniform1i       (const std::string & uniformName, int value);
    void setUniform1ui      (const std::string & uniformName, unsigned int value);
    void setUniform1fv      (const std::string & uniformName, GLsizei count, float * value);
    void setUniform1iv      (const std::string & uniformName, GLsizei count, int * value);
    void setUniform2fv      (const std::string & uniformName, const glm::vec2 & vector);
    void setUniform3fv      (const std::string & uniformName, const glm::vec3 & vector);
    void setUniform4fv      (const std::string & uniformName, const glm::vec4 & vector);
    void setUniformMatrix3fv(const std::string & uniformName, const glm::mat3 & matrix);
    void setUniformMatrix4fv(const std::string & uniformName, const glm::mat4 & matrix);
    
    void apply();

private:
    std::map<std::string, GLint> uniformsLocations;

    GLuint program_id;
    bool isLinked;

    bool link();
    bool getUniformLocation(const std::string & uniform_name);
    std::string loadFile(const std::string & filename);
};


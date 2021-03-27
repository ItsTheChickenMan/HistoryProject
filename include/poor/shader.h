/* date = January 25th 2021 5:08 pm */

#ifndef SHADER_H
#define SHADER_H

#include "poor/types.h"
#include <glad/glad.h>

u32 generate_shader_program(const char* vertex_source, const char* frag_source)
{
    s32 success;
    char info[512];
    
    u32 vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_source, 0);
    glCompileShader(vs);
    
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, 0, info);
        printf("Vertex Shader didnt compile:\n%s\n", info);
    }
    
    u32 fs; 
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag_source, 0);
    glCompileShader(fs);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, 0, info);
        printf("Fragment Shader didnt compile:\n%s\n", info);
    }
    
    u32 sprogram = glCreateProgram();
    
    glAttachShader(sprogram, vs);
    glAttachShader(sprogram, fs);
    glLinkProgram(sprogram);
    
    glGetProgramiv(sprogram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(sprogram, 512, NULL, info);
        printf("Shader Program didnt link:\n%s\n", info);
    }
    
    glDeleteShader(fs);
    glDeleteShader(vs);
    return sprogram;
}

#endif //SHADER_H

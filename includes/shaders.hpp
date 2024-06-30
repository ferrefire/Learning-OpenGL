#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <iostream>
#include "glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include "errors.hpp"
#include "utilities.hpp"

long compileShader(int type, const char *path);
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int createShaderProgram(unsigned int computeShader);
unsigned int createShaderProgram(const char *vertexPath, const char *fragmentPath);
unsigned int createShaderProgram(const char *computePath);

#endif
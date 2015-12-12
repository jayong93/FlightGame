#pragma once

#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <string>
#include <Windows.h>

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info);

int GetWindowWidth();
int GetWindowHeight();
float* GetFovValue();

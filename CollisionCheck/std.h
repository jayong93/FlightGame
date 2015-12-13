#pragma once

#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <string>
#include <Windows.h>

enum StageState { INTRO, MAIN, ENDING };

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info);

int GetWindowWidth();
int GetWindowHeight();
float* GetFovValue();
StageState* GetStageState();

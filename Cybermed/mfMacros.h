#pragma once
#include <assert.h>

//basic definitions for mf classes.

#define MF_FLAGS_TYPE ids
#define MF_FLAGS_INMESH (sizeof(MF_FLAGS_TYPE)*8-1)

#define MF_ERRO 1.0e-15
#define MF_ERRO_INCIRCLE 1.0e-20

#define MAX_BUFFER 255 
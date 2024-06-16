#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <emscripten.h>
#include <string.h>
typedef struct {
    char* key;
    char* value;
} Data; // Example structure, replace with your actual structures.

EMSCRIPTEN_KEEPALIVE
char* processData(const char* input) {
    
    return strdup(input);
}
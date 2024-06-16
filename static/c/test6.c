#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #define EMSCRIPTEN_KEEPALIVE ;
#endif


const int MAX_LEN = 100;

int c_get_str(char* s) {
    srand(time(NULL));
    int r = rand() % MAX_LEN;
    int i;
    for(i = 0; i < r; i++) {
        s[i] = 'M';
    }
    return r;
}


EMSCRIPTEN_KEEPALIVE
int getRandomStr(char* s) {
    return c_get_str(s);
}


#ifndef __EMSCRIPTEN__
int main() {
    char* src = (char*)malloc(MAX_LEN);
    int len = getRandomStr(src);
    char* dest = (char*)malloc(len + 1);
    strcpy(dest, src);
    free(src);
    dest[len] = '\n';
    printf("%s", dest);
    free(dest);
    return 0;
}
#endif
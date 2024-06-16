#include <stdio.h>
#include <stdlib.h>

int main() {
    system("cd static/c && make");
    system("npm install");
    system("npm run dev -- --open");
    return 0;
}
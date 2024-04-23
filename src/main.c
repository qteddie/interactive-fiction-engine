#include <emscripten.h>
#include <jansson.h>
#include <stdio.h>
#include <string.h>


json_t *gameData;

EMSCRIPTEN_KEEPALIVE
void initGame() {
    json_error_t error;
    gameData = json_load_file("static/json/output.json", 0, &error);
    if (!gameData) {
        printf("Error parsing JSON: %s\n", error.text);
        return;
    }
}

EMSCRIPTEN_KEEPALIVE
char* processInput(char* input) {
    if (strcmp(input, "start") == 0) {
        json_t *startEvent = json_object_get(gameData, "event.start");
        if (startEvent) {
            const char *scene = json_string_value(json_object_get(startEvent, "scene"));
            const char *dialogue = json_string_value(json_object_get(startEvent, "dialogue"));
            printf("Scene: %s\nDialogue: %s\n", scene, dialogue);
        }
    }
    return NULL;
}
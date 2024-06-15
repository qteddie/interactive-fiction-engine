#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <emscripten/emscripten.h>

typedef struct {
    char *next;
} Option;

typedef struct {
    Option *options;
    size_t option_count;
} GameData;

Option parse_option(cJSON *json);
GameData parse_game_data(const char *json_str);
void free_game_data(GameData *gameData);

Option parse_option(cJSON *json) {
    Option option;
    cJSON *next = cJSON_GetObjectItemCaseSensitive(json, "next");
    if (cJSON_IsString(next) && (next->valuestring != NULL)) {
        option.next = strdup(next->valuestring);
    } else {
        option.next = NULL;
    }
    return option;
}

GameData parse_game_data(const char *json_str) {
    GameData gameData;
    cJSON *json = cJSON_Parse(json_str);
    cJSON *dialogue = cJSON_GetObjectItemCaseSensitive(json, "dialogue");
    size_t option_count = cJSON_GetArraySize(dialogue);
    gameData.options = (Option*) malloc(option_count * sizeof(Option));
    gameData.option_count = option_count;

    for (size_t i = 0; i < option_count; ++i) {
        cJSON *option_json = cJSON_GetArrayItem(dialogue, i);
        gameData.options[i] = parse_option(option_json);
    }

    cJSON_Delete(json);
    return gameData;
}

char *process_game_data(const char *json_str, const char *currentKey) {
    GameData gameData = parse_game_data(json_str);
    cJSON *resultJson = cJSON_CreateObject();
    int found = 0;

    for (size_t i = 0; i < gameData.option_count; ++i) {
        if (strcmp(gameData.options[i].next, currentKey) == 0) {
            cJSON_AddStringToObject(resultJson, "dialogue", gameData.options[i].next);
            cJSON_AddNumberToObject(resultJson, "dialogueIndex", i);
            found = 1;
            break;
        }
    }

    if (!found) {
        cJSON_AddNullToObject(resultJson, "dialogue");
        cJSON_AddNumberToObject(resultJson, "dialogueIndex", -1);
    }

    char *response = cJSON_Print(resultJson);
    cJSON_Delete(resultJson);
    free_game_data(&gameData);

    return response;
}

void free_game_data(GameData *gameData) {
    for (size_t i = 0; i < gameData->option_count; ++i) {
        free(gameData->options[i].next);
    }
    free(gameData->options);
}

EMSCRIPTEN_KEEPALIVE
char *get_game_data_response(const char *json_str, const char *currentKey) {
    return process_game_data(json_str, currentKey);
}

EMSCRIPTEN_KEEPALIVE
void free_memory(char *ptr) {
    free(ptr);
}
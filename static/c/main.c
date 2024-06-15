#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

typedef struct {
    char *next;
} Option;

typedef struct {
    Option *options;
    size_t option_count;
} GameData;

typedef struct {
    char *dialogue;
    int dialogueIndex;
} Result;

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
    gameData.options = malloc(option_count * sizeof(Option));
    gameData.option_count = option_count;

    for (size_t i = 0; i < option_count; ++i) {
        cJSON *option_json = cJSON_GetArrayItem(dialogue, i);
        gameData.options[i] = parse_option(option_json);
    }

    cJSON_Delete(json);
    return gameData;
}
Result process_option(GameData gameData, const char *currentKey) {
    Result result;
    result.dialogue = NULL;
    result.dialogueIndex = -1;

    for (size_t i = 0; i < gameData.option_count; ++i) {
        if (strcmp(gameData.options[i].next, currentKey) == 0) {
            result.dialogue = strdup(gameData.options[i].next);
            result.dialogueIndex = (int)i;
            break;
        }
    }

    return result;
}

void free_game_data(GameData *gameData) {
    for (size_t i = 0; i < gameData->option_count; ++i) {
        free(gameData->options[i].next);
    }
    free(gameData->options);
}

void free_result(Result *result) {
    free(result->dialogue);
}

#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE
Result process_game_data(const char *json_str, const char *currentKey) {
    GameData gameData = parse_game_data(json_str);
    Result result = process_option(gameData, currentKey);
    free_game_data(&gameData);
    return result;
}
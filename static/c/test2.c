#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <emscripten.h>

typedef struct {
    char *name;
    char *background;
} Scene;

typedef struct {
    char *name;
    char *avatar;
    char *tachie;
} Character;


typedef struct {
    char *text;
    char *next;
    char *event;
} Option;

typedef struct {
    char *character;
    char *text;
    Option *options; // 假設每個對話最多有 10 個選項
    int options_count;
} Dialogue;

typedef struct {
    char *name;
    char *description;
    char *icon;
} Item;

typedef struct {
    char *scene;
    char *dialogue;
} Event;

Scene parse_scene(cJSON *scene_json) {
    Scene scene = {0}; // 初始化為0
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(scene_json, "name");
    cJSON *backgroundItem = cJSON_GetObjectItemCaseSensitive(scene_json, "background");

    scene.name = nameItem && cJSON_IsString(nameItem) ? strdup(nameItem->valuestring) : NULL;
    scene.background = backgroundItem && cJSON_IsString(backgroundItem) ? strdup(backgroundItem->valuestring) : NULL;

    return scene;
}

Scene *parse_scenes(cJSON *root, int *scene_count) {
    int count = cJSON_GetArraySize(root);
    *scene_count = count;
    printf("Parsing %d scenes\n", count);

    Scene *scenes = malloc(sizeof(Scene) * count);
    if (!scenes) {
        fprintf(stderr, "Failed to allocate memory for scenes\n");
        *scene_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        cJSON *scene_json = cJSON_GetArrayItem(root, i);
        scenes[i] = parse_scene(scene_json); // 直接調用 parse_scene 並賦值
    }

    return scenes;
}
Character parse_character(cJSON *character_json) {
    Character character = {0}; // 初始化為0
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(character_json, "name");
    cJSON *avatarItem = cJSON_GetObjectItemCaseSensitive(character_json, "avatar");

    character.name = nameItem && cJSON_IsString(nameItem) ? strdup(nameItem->valuestring) : NULL;
    character.avatar = avatarItem && cJSON_IsString(avatarItem) ? strdup(avatarItem->valuestring) : NULL;

    return character;
}

Character *parse_characters(cJSON *root, int *character_count) {
    int count = cJSON_GetArraySize(root);
    *character_count = count;
    printf("Parsing %d characters\n", count);

    Character *characters = malloc(sizeof(Character) * count);
    if (!characters) {
        fprintf(stderr, "Failed to allocate memory for characters\n");
        *character_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        cJSON *character_json = cJSON_GetArrayItem(root, i);
        characters[i] = parse_character(character_json); // 直接調用 parse_character 並賦值
    }

    return characters;
}

// 提前聲明 parse_dialogue 函數
Dialogue *parse_dialogue(cJSON *dialogue_json);
// 解析 JSON 並存儲為 Dialogue 物件陣列
Dialogue *parse_dialogues(cJSON *root, int *dialogue_count) {
    int count = cJSON_GetArraySize(root);
    *dialogue_count = count;
    printf("Parsing %d dialogues\n", count);

    Dialogue *dialogues = malloc(sizeof(Dialogue) * count);
    if (!dialogues) {
        fprintf(stderr, "Failed to allocate memory for dialogues\n");
        *dialogue_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        cJSON *dialogue_json = cJSON_GetArrayItem(root, i);
        printf("Parsing dialogue %d\n", i + 1);
        // 使用指針來接收 parse_dialogue 的返回值
        Dialogue *dialogue_ptr = parse_dialogue(dialogue_json);
        if (dialogue_ptr) {
            dialogues[i] = *dialogue_ptr; // 解引用指針來賦值
            free(dialogue_ptr); // 釋放分配的記憶體
        }
    }

    return dialogues;
}

Dialogue *parse_dialogue(cJSON *dialogue_json) {
    Dialogue *dialogue = malloc(sizeof(Dialogue));
    if (!dialogue) {
        fprintf(stderr, "Failed to allocate memory for dialogue\n");
        return NULL;
    }
    *dialogue = (Dialogue){0}; // 初始化
    printf("Initialized dialogue\n");

    cJSON *character_json = cJSON_GetObjectItemCaseSensitive(dialogue_json, "character");
    if (character_json && cJSON_IsString(character_json)) {
        dialogue->character = strdup(character_json->valuestring);
        printf("Character: %s\n", dialogue->character);
    } else {
        fprintf(stderr, "Character not found or is not a string\n");
    }

    cJSON *text_json = cJSON_GetObjectItemCaseSensitive(dialogue_json, "text");
    if (text_json && cJSON_IsString(text_json)) {
        dialogue->text = strdup(text_json->valuestring);
        printf("Text: %s\n", dialogue->text);
    } else {
        fprintf(stderr, "Text not found or is not a string\n");
    }

    cJSON *options_json = cJSON_GetObjectItemCaseSensitive(dialogue_json, "options");
    if (options_json && cJSON_IsArray(options_json)) {
        int options_count = cJSON_GetArraySize(options_json);
        dialogue->options = malloc(sizeof(Option) * options_count);
        dialogue->options_count = options_count;
        printf("Parsing %d options\n", options_count);

        for (int i = 0; i < options_count; i++) {
            cJSON *option_json = cJSON_GetArrayItem(options_json, i);
            cJSON *text_json = cJSON_GetObjectItemCaseSensitive(option_json, "text");
            cJSON *next_json = cJSON_GetObjectItemCaseSensitive(option_json, "next");
            cJSON *event_json = cJSON_GetObjectItemCaseSensitive(option_json, "event");

            if (text_json && cJSON_IsString(text_json)) {
                dialogue->options[i].text = strdup(text_json->valuestring);
                printf("Option %d text: %s\n", i + 1, dialogue->options[i].text);
            } else {
                fprintf(stderr, "Option text not found or is not a string at index %d\n", i);
            }

            if (next_json && cJSON_IsString(next_json)) {
                dialogue->options[i].next = strdup(next_json->valuestring);
                printf("Option %d next: %s\n", i + 1, dialogue->options[i].next);
            } else {
                fprintf(stderr, "Option next not found or is not a string at index %d\n", i);
            }

            if (event_json && cJSON_IsString(event_json)) {
                dialogue->options[i].event = strdup(event_json->valuestring);
                printf("Option %d event: %s\n", i + 1, dialogue->options[i].event);
            } else {
                fprintf(stderr, "Option event not found or is not a string at index %d\n", i);
            }
        }
    } else {
        fprintf(stderr, "Options not found or is not an array\n");
    }

    return dialogue;
}


Event parse_event(cJSON *event_json) {
    Event event = {0}; // 初始化為0
    cJSON *sceneItem = cJSON_GetObjectItemCaseSensitive(event_json, "scene");
    cJSON *dialogueItem = cJSON_GetObjectItemCaseSensitive(event_json, "dialogue");

    event.scene = sceneItem && cJSON_IsString(sceneItem) ? strdup(sceneItem->valuestring) : NULL;
    event.dialogue = dialogueItem && cJSON_IsString(dialogueItem) ? strdup(dialogueItem->valuestring) : NULL;

    return event;
}
Event *parse_events(cJSON *root, int *event_count) {
    int count = cJSON_GetArraySize(root);
    *event_count = count;
    printf("Parsing %d events\n", count);

    Event *events = malloc(sizeof(Event) * count);
    if (!events) {
        fprintf(stderr, "Failed to allocate memory for events\n");
        *event_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        cJSON *event_json = cJSON_GetArrayItem(root, i);
        // printf("Parsing event %d\n", i + 1);
        events[i] = parse_event(event_json); // 直接調用 parse_event 並賦值
    }

    return events;
}

Item parse_item(cJSON *item_json) {
    Item item = {0}; // 初始化為0
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(item_json, "name");
    cJSON *descriptionItem = cJSON_GetObjectItemCaseSensitive(item_json, "description");
    cJSON *iconItem = cJSON_GetObjectItemCaseSensitive(item_json, "icon");

    item.name = nameItem && cJSON_IsString(nameItem) ? strdup(nameItem->valuestring) : NULL;
    item.description = descriptionItem && cJSON_IsString(descriptionItem) ? strdup(descriptionItem->valuestring) : NULL;
    item.icon = iconItem && cJSON_IsString(iconItem) ? strdup(iconItem->valuestring) : NULL;

    return item;
}
Item *parse_items(cJSON *root, int *item_count) {
    int count = cJSON_GetArraySize(root);
    *item_count = count;
    printf("Parsing %d items\n", count);

    Item *items = malloc(sizeof(Item) * count);
    if (!items) {
        fprintf(stderr, "Failed to allocate memory for items\n");
        *item_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        cJSON *item_json = cJSON_GetArrayItem(root, i);
        // printf("Parsing item %d\n", i + 1);
        items[i] = parse_item(item_json); // 直接調用 parse_item 並賦值
    }

    return items;
}


char* read_json_file(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char*)malloc(length + 1);
    if (data) {
        fread(data, 1, length, file);
        data[length] = '\0';
    }
    fclose(file);
    return data;
}

int main() {
    const char *filename = "output3.json";
    char *json_data = read_json_file(filename);
    if (!json_data) {
        fprintf(stderr, "Failed to read JSON file\n");
        return EXIT_FAILURE;
    }

    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) fprintf(stderr, "Error before: %s\n", error_ptr);
        free(json_data);
        return EXIT_FAILURE;
    }
    // 解析 scene
    cJSON *scene_json = cJSON_GetObjectItemCaseSensitive(root, "scene");
    int scene_count = 0;
    Scene *scenes = parse_scenes(scene_json, &scene_count);
    // 解析 character
    cJSON *character_json = cJSON_GetObjectItemCaseSensitive(root, "character");
    int character_count = 0;
    Character *characters = parse_characters(character_json, &character_count);
    // 解析 dialogue
    cJSON *dialogue_json = cJSON_GetObjectItemCaseSensitive(root, "dialogue");
    int dialogue_count = 0;
    Dialogue *dialogue = parse_dialogues(dialogue_json, &dialogue_count);
    // 解析 item
    cJSON *item_json = cJSON_GetObjectItemCaseSensitive(root, "item");
    int item_count = 0;
    Item *items = parse_items(item_json, &item_count);
    for (int i = 0; i < item_count; i++) {
        printf("Item %d name: %s, description: %s, icon: %s\n", i + 1, items[i].name, items[i].description, items[i].icon);
    }
    // 解析 event
    cJSON *event_json = cJSON_GetObjectItemCaseSensitive(root, "event");
    int event_count = 0;
    Event *events = parse_events(event_json, &event_count);
    for (int i = 0; i < event_count; i++) {
        printf("Event %d scene: %s, dialogue: %s\n", i + 1, events[i].scene, events[i].dialogue);
    }

    // 釋放記憶體

    free(scenes->name);
    scenes->name = NULL;
    free(scenes->background);
    scenes->background = NULL;

    free(characters->name);
    characters->name = NULL;
    free(characters->avatar);
    characters->avatar = NULL;
    free(characters->tachie);
    characters->tachie = NULL;

    free(dialogue->options);
    dialogue->options = NULL;

    free(items->name);
    items->name = NULL;
    free(items->description);
    items->description = NULL;
    free(items->icon);
    items->icon = NULL;

    free(events->scene);
    events->scene = NULL;
    free(events->dialogue);
    events->dialogue = NULL;

    cJSON_Delete(root);
    free(json_data);
    json_data = NULL;

    return EXIT_SUCCESS;
}


EMSCRIPTEN_KEEPALIVE
char* getGameDataAsJson(Scene *scene, int scene_count, Character *character, int character_count , Dialogue *dialogue, int dialogue_count, Item *items, int item_count, Event *events, int event_count) {
    cJSON *gameData = cJSON_CreateObject();
    cJSON *scenesArray = cJSON_CreateArray();
    cJSON *charactersArray = cJSON_CreateArray();
    cJSON *dialoguesArray = cJSON_CreateArray();
    cJSON *itemsArray = cJSON_CreateArray();
    cJSON *eventsArray = cJSON_CreateArray();

    // 添加 scenes
    for (size_t i = 0; i < scene_count; i++) {
        cJSON *sceneJson = cJSON_CreateObject();
        cJSON_AddItemToObject(sceneJson, "name", cJSON_CreateString(scene[i].name));
        cJSON_AddItemToObject(sceneJson, "background", cJSON_CreateString(scene[i].background));
        cJSON_AddItemToArray(scenesArray, sceneJson);
    }
    cJSON_AddItemToObject(gameData, "scenes", scenesArray);

    // 添加 characters
    for (size_t i = 0; i < character_count; i++) {
        cJSON *characterJson = cJSON_CreateObject();
        cJSON_AddItemToObject(characterJson, "name", cJSON_CreateString(character[i].name));
        cJSON_AddItemToObject(characterJson, "avatar", cJSON_CreateString(character[i].avatar));
        cJSON_AddItemToArray(charactersArray, characterJson);
    }
    cJSON_AddItemToObject(gameData, "characters", charactersArray);

    // 添加 dialogues
    for (int i = 0; i < dialogue_count; i++) {
        cJSON *dialogueJson = cJSON_CreateObject();
        cJSON_AddItemToObject(dialogueJson, "character", cJSON_CreateString(dialogue[i].character));
        cJSON_AddItemToObject(dialogueJson, "text", cJSON_CreateString(dialogue[i].text));
        // 假設 options 是一個簡單的字符串數組
        cJSON *optionsArray = cJSON_CreateArray();
        for (int j = 0; j < dialogue[i].options_count; j++) {
            cJSON_AddItemToArray(optionsArray, cJSON_CreateString(dialogue[i].options[j].text));
        }
        cJSON_AddItemToObject(dialogueJson, "options", optionsArray);
        cJSON_AddItemToArray(dialoguesArray, dialogueJson);
    }
    cJSON_AddItemToObject(gameData, "dialogue", dialoguesArray);

    // 添加 items
    for (int i = 0; i < item_count; i++) {
        cJSON *itemJson = cJSON_CreateObject();
        cJSON_AddItemToObject(itemJson, "name", cJSON_CreateString(items[i].name));
        cJSON_AddItemToObject(itemJson, "description", cJSON_CreateString(items[i].description));
        cJSON_AddItemToObject(itemJson, "icon", cJSON_CreateString(items[i].icon));
        cJSON_AddItemToArray(itemsArray, itemJson);
    }
    cJSON_AddItemToObject(gameData, "items", itemsArray);

    // 添加 events
    for (int i = 0; i < event_count; i++) {
        cJSON *eventJson = cJSON_CreateObject();
        cJSON_AddItemToObject(eventJson, "scene", cJSON_CreateString(events[i].scene));
        cJSON_AddItemToObject(eventJson, "dialogue", cJSON_CreateString(events[i].dialogue));
        cJSON_AddItemToArray(eventsArray, eventJson);
    }
    cJSON_AddItemToObject(gameData, "events", eventsArray);

    char *jsonString = cJSON_Print(gameData);
    printf("%s\n", jsonString);
    cJSON_Delete(gameData);

    return jsonString;
}

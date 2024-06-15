#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

typedef struct {
    char *id;
    char *name;
    char *background;
} Scene;

typedef struct {
    Scene *scenes;
    size_t num_scenes;
} Scenes;

typedef struct {
    char *name;
    char *avatar;
    char *tachie;
    char *location;
} Character;

typedef struct {
    Character *characters;
    size_t num_characters;
} Characters;

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

Scene parse_scene(cJSON *scene_json) {
    Scene scene = {0}; // 初始化為0
    cJSON *idItem = cJSON_GetObjectItemCaseSensitive(scene_json, "id");
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(scene_json, "name");
    cJSON *backgroundItem = cJSON_GetObjectItemCaseSensitive(scene_json, "background");

    scene.id = idItem && cJSON_IsString(idItem) ? strdup(idItem->valuestring) : NULL;
    scene.name = nameItem && cJSON_IsString(nameItem) ? strdup(nameItem->valuestring) : NULL;
    scene.background = backgroundItem && cJSON_IsString(backgroundItem) ? strdup(backgroundItem->valuestring) : NULL;

    return scene;
}

Characters parse_characters(cJSON *characters_json) {
    size_t num_characters = cJSON_GetArraySize(characters_json);
    Character *characters = malloc(num_characters * sizeof(Character));
    if (!characters) {
        // 處理記憶體分配失敗
        fprintf(stderr, "Failed to allocate memory for characters.\n");
        exit(1); // 或適當的錯誤處理
    }

    size_t i = 0;
    cJSON *character_json = NULL;
    cJSON_ArrayForEach(character_json, characters_json) {
        cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(character_json, "name");
        cJSON *avatarItem = cJSON_GetObjectItemCaseSensitive(character_json, "avatar");
        cJSON *tachieItem = cJSON_GetObjectItemCaseSensitive(character_json, "tachie");
        cJSON *locationItem = cJSON_GetObjectItemCaseSensitive(character_json, "location");

        characters[i].name = nameItem ? strdup(nameItem->valuestring) : NULL;
        characters[i].avatar = avatarItem ? strdup(avatarItem->valuestring) : NULL;
        characters[i].tachie = tachieItem ? strdup(tachieItem->valuestring) : NULL;
        // 因為 "location" 鍵可能不存在，所以需要額外檢查
        characters[i].location = locationItem ? strdup(locationItem->valuestring) : NULL;
        i++;
    }

    Characters all_characters = {characters, num_characters};
    return all_characters;
}
Scenes parse_scenes(cJSON *root) {
    cJSON *scenes_json = cJSON_GetObjectItemCaseSensitive(root, "scene");
    if (!scenes_json) {
        return (Scenes){NULL, 0};  // Return an empty Scenes structure if no "scene" key is found
    }

    size_t num_scenes = cJSON_GetArraySize(scenes_json);
    Scene *scenes = malloc(num_scenes * sizeof(Scene));
    if (!scenes) {
        fprintf(stderr, "Memory allocation failed for scenes\n");
        return (Scenes){NULL, 0};  // Return an empty Scenes structure on allocation failure
    }

    size_t i = 0;
    cJSON *scene_json = NULL;
    cJSON_ArrayForEach(scene_json, scenes_json) {
        scenes[i] = parse_scene(scene_json);
        if (scenes[i].name == NULL) {  // Error check for each scene
            // Cleanup already allocated scenes
            while (i > 0) {
                i--;
                free(scenes[i].name);
                free(scenes[i].background);
                free(scenes[i].id);
            }
            free(scenes);
            return (Scenes){NULL, 0};
        }
        i++;
    }

    return (Scenes){scenes, num_scenes};
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

    Scenes scenes = parse_scenes(root);
    Characters characters = parse_characters(cJSON_GetObjectItemCaseSensitive(root, "character"));
    if (scenes.scenes && characters.characters) {
        for (size_t i = 0; i < scenes.num_scenes; i++) {
            printf("Scene name: %s, background: %s\n", scenes.scenes[i].name, scenes.scenes[i].background);
        }
        printf("First character name: %s, avatar: %s\n", characters.characters[0].name, characters.characters[0].avatar);
    } else {
        fprintf(stderr, "Failed to parse scenes or characters correctly.\n");
    }

    // 解析 dialogue
    cJSON *dialogue_json = cJSON_GetObjectItemCaseSensitive(root, "dialogue");
    // printf("Dialogue JSON: %s\n", cJSON_Print(dialogue_json));
    int dialogue_count = 0;
    Dialogue *dialogue = parse_dialogues(dialogue_json, &dialogue_count);
    // printf("Character: %s\n", dialogue->character);
    // printf("Text: %s\n", dialogue->text);
    // for (int i = 0; i < dialogue->options_count; i++) {
    //     printf("Option %d: %s -> %s\n", i + 1, dialogue->options[i].text, dialogue->options[i].next);
    // }
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
    // Cleanup code with additional checks and setting pointers to NULL after free
    for (size_t i = 0; i < scenes.num_scenes; i++) {
        free(scenes.scenes[i].name);
        scenes.scenes[i].name = NULL;
        free(scenes.scenes[i].background);
        scenes.scenes[i].background = NULL;
        free(scenes.scenes[i].id);
        scenes.scenes[i].id = NULL;
    }
    free(scenes.scenes);
    scenes.scenes = NULL;

    for (size_t i = 0; i < characters.num_characters; i++) {
        free(characters.characters[i].name);
        characters.characters[i].name = NULL;
        free(characters.characters[i].avatar);
        characters.characters[i].avatar = NULL;
        free(characters.characters[i].tachie);
        characters.characters[i].tachie = NULL;
        free(characters.characters[i].location);
        characters.characters[i].location = NULL;
    }
    free(characters.characters);
    characters.characters = NULL;

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
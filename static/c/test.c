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
    size_t i = 0;

    cJSON *character_json = NULL;
    cJSON_ArrayForEach(character_json, characters_json) {
        characters[i].name = strdup(cJSON_GetObjectItemCaseSensitive(character_json, "name")->valuestring);
        characters[i].avatar = strdup(cJSON_GetObjectItemCaseSensitive(character_json, "avatar")->valuestring);
        characters[i].tachie = strdup(cJSON_GetObjectItemCaseSensitive(character_json, "tachie")->valuestring);
        characters[i].location = strdup(cJSON_GetObjectItemCaseSensitive(character_json, "location")->valuestring);
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

    // Cleanup code with additional checks
    for (size_t i = 0; i < scenes.num_scenes; i++) {
        free(scenes.scenes[i].name);
        free(scenes.scenes[i].background);
        free(scenes.scenes[i].id);
    }
    free(scenes.scenes);

    for (size_t i = 0; i < characters.num_characters; i++) {
        free(characters.characters[i].name);
        free(characters.characters[i].avatar);
        free(characters.characters[i].tachie);
        free(characters.characters[i].location);
    }
    free(characters.characters);

    cJSON_Delete(root);
    free(json_data);

    return EXIT_SUCCESS;
}
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
char* processData() {
    // cJSON *json = cJSON_Parse(jsonStr);
    // if (json == NULL) {
    //     const char *error_ptr = cJSON_GetErrorPtr();
    //     if (error_ptr != NULL) {
    //         fprintf(stderr, "Error before: %s\n", error_ptr);
    //     }
    //     return EXIT_FAILURE;
    // }

    // // Assuming 'Data' is one of your structures
    // Data data;
    // cJSON *item = cJSON_GetObjectItemCaseSensitive(json, "key");
    // if (cJSON_IsString(item) && (item->valuestring != NULL)) {
    //     data.key = strdup(item->valuestring);
    //     printf("Key: %s\n", data.key);
    // }

    // cJSON_Delete(json);
    // free(data.key); // Remember to free any allocated memory
    // return EXIT_SUCCESS;
    // printf("Processing data: %s\n", jsonStr);
    // char *result = (char*) malloc(strlen(jsonStr) + 1);
    // strcpy(result, jsonStr);
    return "processed";
}

EMSCRIPTEN_KEEPALIVE
char* hello(const char* input) {
    // printf("Hello, %s!\n", input);
    return strdup(input);  // Duplicate the string to keep it in memory
}
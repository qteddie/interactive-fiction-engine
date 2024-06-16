#include <stdio.h>
#include <stdlib.h>
// #include "cJSON.h"
#include <emscripten.h>
#include <string.h>

EMSCRIPTEN_KEEPALIVE
char* processData(const char* input) {

    return strdup(input);
}
// EMSCRIPTEN_KEEPALIVE
// char* getNextEncounter(const char* jsonString) {
//     cJSON *json = cJSON_Parse(jsonString);
//     if (json == NULL) {
//         const char *error_ptr = cJSON_GetErrorPtr();
//         if (error_ptr != NULL) {
//             fprintf(stderr, "Error before: %s\n", error_ptr);
//         }
//         return NULL;
//     }

//     char* result = NULL;
//     cJSON *next = cJSON_GetObjectItemCaseSensitive(json, "next");
//     if (cJSON_IsString(next) && (next->valuestring != NULL)) {
//         result = strdup(next->valuestring);  // 复制字符串以供返回
//     }

//     cJSON_Delete(json);  // 清理 cJSON 对象
//     return result;  // 返回复制的字符串
// }
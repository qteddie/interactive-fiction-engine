#include <emscripten.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// 假设最大对话和事件数量为 100，最大字符数量为 50
#define MAX_DIALOGUES 100
#define MAX_CHARACTERS 100
#define MAX_EVENTS 100
#define MAX_STRING_LENGTH 50

typedef struct {
    char next[MAX_STRING_LENGTH];
    char event[MAX_STRING_LENGTH];
} Option;

typedef struct {
    char text[500];
    char character[MAX_STRING_LENGTH];
    Option options[10];
    int optionCount;
} Dialogue;

typedef struct {
    char scene[MAX_STRING_LENGTH];
    char dialogue[MAX_STRING_LENGTH];
} Event;

typedef struct {
    char avatar[MAX_STRING_LENGTH];
    char tachie[MAX_STRING_LENGTH];
} Character;

typedef struct {
    Dialogue dialogues[MAX_DIALOGUES];
    Event events[MAX_EVENTS];
    Character characters[MAX_CHARACTERS];
    char scenes[MAX_EVENTS][MAX_STRING_LENGTH];
} GameData;

GameData gameData;
int currentDialogueIndex = 0;
Dialogue currentDialogue;
char currentScene[MAX_STRING_LENGTH];
Character currentCharacter;
bool isTransitioning = false;

// 辅助函数来查找字符串在数组中的索引
int findDialogueIndexByName(const char* name) {
    for (int i = 0; i < MAX_DIALOGUES; ++i) {
        if (strcmp(gameData.dialogues[i].text, name) == 0) {
            return i;
        }
    }
    return -1; // 未找到
}

int findEventIndexByName(const char* name) {
    for (int i = 0; i < MAX_EVENTS; ++i) {
        if (strcmp(gameData.events[i].dialogue, name) == 0) {
            return i;
        }
    }
    return -1; // 未找到
}

int findCharacterIndexByName(const char* name) {
    for (int i = 0; i < MAX_CHARACTERS; ++i) {
        if (strcmp(gameData.characters[i].avatar, name) == 0) {
            return i;
        }
    }
    return -1; // 未找到
}

EMSCRIPTEN_KEEPALIVE
void nextDialogue(const char* optionNext, const char* optionEvent) {
    int dialogueIndex = findDialogueIndexByName(optionNext);
    int eventIndex = findEventIndexByName(optionEvent);

    if (dialogueIndex != -1) {
        currentDialogue = gameData.dialogues[dialogueIndex];
        currentDialogueIndex = dialogueIndex;
    }
    if (eventIndex != -1) {
        currentDialogue = gameData.dialogues[findDialogueIndexByName(gameData.events[eventIndex].dialogue)];
        currentDialogueIndex = findDialogueIndexByName(gameData.events[eventIndex].dialogue);
        if (eventIndex != -1) {
            isTransitioning = true;
            emscripten_sleep(1000);
            strcpy(currentScene, gameData.scenes[eventIndex]);
            isTransitioning = false;
        }
    }

    if (!currentDialogue.optionCount) {
        // endGame();
        return;
    }

    int characterIndex = findCharacterIndexByName(currentDialogue.character);
    if (characterIndex != -1) {
        currentCharacter = gameData.characters[characterIndex];
    }
}
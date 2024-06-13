all:
	emcc main.c -o main.js -s EXPORTED_FUNCTIONS='["_nextDialogue", "_getCurrentDialogueText"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -s ASYNCIFY -s 'ASYNCIFY_IMPORTS=["emscripten_sleep"]'
clean:
	rm -f main.js main.wasm
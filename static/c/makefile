all:
	emcc main.c -o main.js -s EXPORTED_FUNCTIONS='["_nextDialogue"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -s ASYNCIFY
clean:
	rm -f main.js main.wasm
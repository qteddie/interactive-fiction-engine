all:
	emcc -O1 test.c ./cJSON/cJSON.c -s EXPORTED_FUNCTIONS='["_malloc","_processData", "_free"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString"]' -s WASM=1 -o test.js -I./cJSON --no-entry
clean:
	rm -f test.js test.wasm
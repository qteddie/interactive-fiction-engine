all:
	emcc main.c ./cJSON/cJSON.c -o main.js \
	-s EXPORTED_FUNCTIONS='["_process_game_data"]' \
	-s ASYNCIFY \
	-s 'ASYNCIFY_IMPORTS=["emscripten_sleep"]' \
	-I./cJSON

clean:
	rm -f main.js main.wasm
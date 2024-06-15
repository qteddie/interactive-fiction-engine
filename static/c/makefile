all:
	emcc main.c ./cJSON/cJSON.c -o main.js \
	-s EXPORTED_FUNCTIONS='["_get_game_data_response", "_free_memory"]' \
	-s ASYNCIFY \
	-I./cJSON
	gcc test.c ./cJSON/cJSON.c -o test -I./cJSON
clean:
	rm -f main.js main.wasm
all:
	# emcc main.c ./cJSON/cJSON.c -o main.js \
	# -s EXPORTED_FUNCTIONS='["_get_game_data_response", "_free_memory"]' \
	# -s ASYNCIFY \
	# -I./cJSON
	# gcc test.c ./cJSON/cJSON.c -o test -I./cJSON
	emcc -O1 test.c ./cJSON/cJSON.c -s EXPORTED_FUNCTIONS='["_getGameDataAsJson"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s WASM=1 -o test.js -I./cJSON
	gcc test4.c ./cJSON/cJSON.c -o test4 -I./cJSON
clean:
	rm -f test.js test.wasm
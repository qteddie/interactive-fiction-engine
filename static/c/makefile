all:
	# emcc main.c ./cJSON/cJSON.c -o main.js \
	# -s EXPORTED_FUNCTIONS='["_get_game_data_response", "_free_memory"]' \
	# -s ASYNCIFY \
	# -I./cJSON
	# gcc test.c ./cJSON/cJSON.c -o test -I./cJSON
	# emcc -O1 test_last.c ./cJSON/cJSON.c -s EXPORTED_FUNCTIONS='["_getGameDataAsJson", "_hello"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString"]' -s WASM=1 -o test_last.js -I./cJSON
	# gcc test4.c ./cJSON/cJSON.c -o test4 -I./cJSON
	emcc -O1 test.c ./cJSON/cJSON.c -s EXPORTED_FUNCTIONS='["_malloc","_processData", "_free"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString"]' -s WASM=1 -o test.js -I./cJSON --no-entry -sMODULARIZE
	# gcc test.c ./cJSON/cJSON.c -o test -I./cJSON
	# emcc test6.c \
	# 	-o test6.js \
	# 	-sMODULARIZE \
	# 	-sEXPORTED_RUNTIME_METHODS=UTF8ToString \
	# 	-sEXPORTED_FUNCTIONS=["_malloc","_free"]
clean:
	rm -f test.js test.wasm
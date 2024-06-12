all:
	emcc main.c -s WASM=1 -s EXPORTED_FUNCTIONS='[""]' -s -o main.js	
clean:
	rm -f main.js main.wasm
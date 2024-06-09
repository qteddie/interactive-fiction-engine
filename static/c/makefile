all:
	emcc add.c -s WASM=1 -s EXPORTED_FUNCTIONS='["_add"]' -s -o add.js	
clean:
	rm -f add.js add.wasm
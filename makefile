all:
	emcc -O3 -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -I./libs/jansson/src -I./libs/tomlc99 src/main.c ./libs/tomlc99/toml.c -o main.js -L./libs/jansson/.libs -ljansson
clean:
	rm -f main.js
    # gcc -I./libs/jansson -I./libs/tomlc99 src/main.c ./libs/tomlc99/toml.c -o main -ljansson
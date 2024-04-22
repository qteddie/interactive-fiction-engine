all:
	emcc -O3 -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -I./libs/libyaml/include -I./libs/tomlc99 src/main.c ./libs/tomlc99/toml.c -o main.js -lyaml
clean:
	rm -f main
	# gcc -I./libs/libyaml -I./libs/tomlc99 src/main.c ./libs/tomlc99/toml.c -o main -lyaml
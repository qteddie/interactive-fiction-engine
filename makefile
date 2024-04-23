all:
	emcc -O3 \
	-s WASM=1 \
	-s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' \
	-s TOTAL_MEMORY=2147483648 \
	-I./libs/jansson/src \
	-I./libs/libyaml/include \
	-I./libs/tomlc99 \
	src/main.c ./libs/tomlc99/toml.c \
	-o main.js \
	-L./libs/jansson/src/.libs \
	-L./libs/libyaml/src/.libs \
	-ljansson \
	-libyaml
clean:
	rm -f main.js
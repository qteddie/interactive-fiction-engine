all:
	emcc -Oz \
	-s WASM=1 \
	-s EXPORTED_RUNTIME_METHODS='["cwrap"]' \
	-s TOTAL_MEMORY=4294967296 \
	-I./libs/jansson/src \
	-I./libs/tomlc99 \
	src/main.c ./libs/tomlc99/toml.c \
	-o main.js \
	-L./libs/jansson/src/.libs \
	-ljansson \
	--preload-file static/toml/eddie.toml
clean:
	rm -f main.js
	# -lyaml
	# -L./libs/libyaml/src/.libs \
	# -I./libs/libyaml/include \
all:
	gcc -I./libs/tomlc99 -I./libs/jansson/src src/main.c ./libs/tomlc99/toml.c -o main -ljansson
clean:
	rm -f main
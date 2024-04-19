all:
	gcc -I./libs/tomlc99 -I./libs/jansson/src src/main.c src/simple_parse.c ./libs/tomlc99/toml.c -o main
clean:
	rm -f main
all:
	gcc -I./libs/tomlc99 src/main.c ./libs/tomlc99/toml.c -o main
clean:
	rm -f main
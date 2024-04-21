all:
	gcc -I./libs/libyaml -I./libs/tomlc99 src/main.c ./libs/tomlc99/toml.c -o main -lyaml
clean:
	rm -f main
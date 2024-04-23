all:
	gcc \
	-I../../libs/jansson-gcc/src \
	-I../../libs/tomlc99 \
	main.c \
	../../libs/tomlc99/toml.c \
	-o main \
	-L../../libs/jansson-gcc/src/.libs \
	-ljansson
clean:
	rm -f main
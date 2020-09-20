all: wish.c
	gcc -Wall -Werror wish.c -o wish
debug: wish.c
	gcc -Wall -Werror -ggdb3 wish.c -o wish
clean:
	rm wish
	rm -rf tests-out/

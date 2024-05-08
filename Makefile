vpath %.c src
vpath % build

life: src life.c
	gcc src/life.c -lncurses -o build/life

.PHONY: test
test: life
	build/life

NOM = Traini

FLAGS = -Wall -Wextra -pedantic -std=gnu99 -O3
#FLAGS = -Wall -Wextra -pedantic -std=gnu99 -Wno-unused-parameter -O3
LIBS = -lgc
GCC = gcc $(FLAGS)
#GCC = clang $(FLAGS)

HEADERS = regex.h
SOURCES = utils.c tp3-$(NOM).c
OBJECTS = $(SOURCES:.c=.o)



current: regex


%.o: %.c $(HEADERS)
	$(GCC) -c $<

regex: $(HEADERS) main.c $(OBJECTS)
	$(GCC) main.c $(OBJECTS) -o regex $(LIBS)

clean:
	rm -f regex a.out gmon.out *.o


.PHONY: current clean

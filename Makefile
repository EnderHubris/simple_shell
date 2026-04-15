CC_FLAGS = -Wall -Wextra

all: build clean_o
	./smsh

splitter_o:
	gcc -c $(CC_FLAGS) splitter.c -o splitter.o
tokenizer_o:
	gcc -c $(CC_FLAGS) tokenizer.c -o tokenizer.o
smsh_environ_o:
	gcc -c $(CC_FLAGS) environ.c -o smsh_environ.o
build: splitter_o tokenizer_o smsh_environ_o
	gcc $(CC_FLAGS) smsh.c \
	splitter.o \
	tokenizer.o \
	smsh_environ.o \
	-o smsh

clean_o:
	rm -f *.o
clean: clean_o
	rm -f smsh

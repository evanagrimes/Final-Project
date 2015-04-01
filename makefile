# I am a comment, and I want to say that the variable CC will be
# # the compiler to use.
CC=gcc
# # Hey!, I am comment number 2. I want to say that CFLAGS will be the
# # options I'll pass to the compiler.
FPATH=$(HOME)/Library/Frameworks
FILES=basic.c engine.c
CFLAGS=-Wall -g --std=c99
OSFLAV=

ifeq ($(shell uname), Linux)
    RUN := run
    OSFLAV := -lSDL2 -o run
else ifeq ($(shell uname -s), Darwin)
    RUN := run
    OSFLAV := -F$(FPATH) -framework SDL2 -o run
else ifeq ($(shell uname -o), Cygwin)
    RUN := run.exe
    OSFLAV :=  -L/usr/local/lib -lcygwin -lSDL2main -lSDL2 -mwindows -o run.exe
endif


all:
	$(CC) $(CFLAGS) $(FILES) $(OSFLAV)

clean:
	rm -rf *.o $(RUN)


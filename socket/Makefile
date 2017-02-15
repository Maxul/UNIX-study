CC = gcc
CFLAG = -g -Wall -O2

BIN = $(patsubst %.c,%,$(wildcard *.c)) 

all : $(BIN)

$(BIN) : % : %.c
	$(CC) $(CFLAG) $< -o $@

clean :
	-rm $(BIN)
	
test :
	bash ../test/test.sh


SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %, $(SRC))

TAR = server

$(TAR) : $(SRC)
	-gcc $^ -o $@ -levent

.PHONY: clean
clean: 
	rm -rf $(TAR)


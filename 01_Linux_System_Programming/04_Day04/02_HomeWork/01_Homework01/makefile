SRC=$(wildcard *.c)
OUTS=$(patsubst %.c, %.out, $(SRC))

$(OUTS): $(SRC) 
	$(CC) $(word 1, $(SRC)) -o $(word 1, $(OUTS))
	$(CC) $(word 2, $(SRC)) -o $(word 2, $(OUTS))

.PHONY: clean
clean:
	rm -rf *.out

SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

$(TARGET): *.c
	echo $(TARGET)
	gcc -g $^ -o $@ 

.PHONY: clean
clean:
	rm -rf $(TARGET)

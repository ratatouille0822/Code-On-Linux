SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

$(TARGET): *.c
	echo $(TARGET)
	gcc -g $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm -rf $(TARGET)

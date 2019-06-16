SRC = $(wildcard *.c)
TARGET = mutex_add

$(TARGET): *.c
	gcc $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm -rf $(TARGET)

TARGET = mutex

$(TARGET): *.c
	gcc $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm -rf $(TARGET)

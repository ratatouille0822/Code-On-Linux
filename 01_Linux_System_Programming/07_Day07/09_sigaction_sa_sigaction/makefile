SRC=$(wildcard *.c)
TARGET=$(patsubst %.c, %, $(SRC))

$(TARGET): $(SRC) 
	$(CC) -g $< -o $@
	chmod u+x $@

.PHONY: clean
clean:
	rm -rf $(TARGET)


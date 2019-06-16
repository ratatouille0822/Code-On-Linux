SRC=$(wildcard *.c)
TARGET=$(patsubst %.c, %, $(SRC))

all: $(TARGET)

%: %.c 
	$(CC) -g $< -o $@ -lpthread
	chmod u+x $@

.PHONY: clean
clean:
	rm -rf $(TARGET) 


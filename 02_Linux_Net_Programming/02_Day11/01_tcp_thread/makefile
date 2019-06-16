all: server client

%:%.c
	gcc -g $< -o $@ -pthread

.PHONY: clean
clean:
	rm -rf server client

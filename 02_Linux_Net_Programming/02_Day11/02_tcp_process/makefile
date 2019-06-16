all: server client

%:%.c
	gcc -g $< -o $@

.PHONY: clean
clean:
	rm -rf server client

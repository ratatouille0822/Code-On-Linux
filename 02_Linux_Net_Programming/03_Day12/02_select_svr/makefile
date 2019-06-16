all: server client

%:%.c
	gcc -g $< -o $@

.PNONY: clean
clean:
	rm -rf server client


all: server

%:%.c
	gcc -g $< -o $@ -levent

.PNONY: clean
clean:
	rm -rf server
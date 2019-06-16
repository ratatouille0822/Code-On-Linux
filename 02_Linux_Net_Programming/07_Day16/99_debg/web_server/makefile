all: server
server: pub.c 01web_sever.c
	gcc -g $^ -o $@

.PHONY: clean
clean:
	rm -rf server

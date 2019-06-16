all: server client

%:%.c
	gcc -g $< -o $@ 

.PHONY: clean
clean:
	rm -rf sever client

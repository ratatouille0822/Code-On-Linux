all: libevent

%:%.c
	gcc -g $< -o $@ -levent

.PHONY:clean

clean:
	rm -rf libevent

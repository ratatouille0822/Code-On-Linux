all: libevent_demo

%:%.c
	gcc -g $< -o $@ -levent

.PHONY:clean

clean:
	rm -rf libevent_demo

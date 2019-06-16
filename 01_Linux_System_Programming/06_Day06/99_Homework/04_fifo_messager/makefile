all: Tom Jerry

Tom: fifo_messager.c
	gcc -g $< -o $@
	chmod u+x $@
Jerry: fifo_messanger_part2.c
	gcc -g $< -o $@
	chmod u+x $@

.PHONY: clean
clean:
	rm Tom Jerry fifo_1 fifo_2



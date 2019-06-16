all:  test
test:*.c
	gcc -g $< -o $@

.PHONY: clean

clean:
	rm -rf test

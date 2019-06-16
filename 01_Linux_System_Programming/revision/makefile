SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
TARGET = list

$(TARGET): $(OBJ)
	gcc -g $^ -o $@ 

%.o: %.c
	gcc -c $< -g

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET)

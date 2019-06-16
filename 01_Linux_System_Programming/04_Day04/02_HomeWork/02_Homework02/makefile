SRCS=$(wildcard ./src/*.c)
INCS=$(patsubst ./src/%.c, ./inc/%.h, $(SRCS))
OBJS=$(patsubst ./src/%.c, ./obj/%.o, $(SRCS))

OBJ_PATH=./obj/
SRC_PATH=./src/
CFLAGS=-I./inc
TARGET=./bin/test

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
.PHONY: clean
clean:
	rm $(OBJS) $(TARGET)

	


	

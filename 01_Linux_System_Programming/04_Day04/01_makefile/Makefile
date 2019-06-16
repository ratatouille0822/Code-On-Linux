SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
TARGET=test

$(TARGET): $(OBJS)
	@echo -e '$$^:----1---------------------'
	@echo $^
	@echo -e '$$<:----2---------------------'
	@echo $<
	@echo -e '$$@:----3---------------------'
	@echo $@
	@echo -e '$$(OBJS):----4---------------------'
	@echo $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $<
.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)

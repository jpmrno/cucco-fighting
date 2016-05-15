include ../makevars

LIBS = ini pipes random serial mqueue

all:
	$(foreach LIB, $(LIBS), $(MAKE) -C $(LIB) all;)

clean:
	$(foreach LIB, $(LIBS), $(MAKE) -C $(LIB) clean;)

.PHONY: all clean

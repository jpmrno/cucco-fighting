include ../makevars

SRC_C = .
SRC_HEADERS = .
SRC_LIBS =

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
OBJECTS = $(SOURCES:.c=.o)

HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

LIBS = $(foreach LIB,$(SRC_LIBS),-l $(LIB))

all: $(OBJECTS)

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) $(LIBS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

.PHONY: all clean

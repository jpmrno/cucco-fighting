include ../makevars

TARGET = client.app
SRC_C = src
SRC_HEADERS = include
SRC_LIBS =

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
OBJECTS = $(SOURCES:.c=.o)

SRC_HEADERS += ../$(SHARED_HEADERS)
HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

LIBS = $(foreach LIB,$(SRC_LIBS),-l $(LIB))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(GCC) $(GCC_FLAGS) $(HEADERS) -o $@ $^ $(LIBS)

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
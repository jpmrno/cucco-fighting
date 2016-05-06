include ../makevars

# --[ Vars ]------------------ #
TARGET = client.app
SRC_C = src
SRC_HEADERS = include ../comapi/include
SRC_LIBS = ../comapi
LIBS = comapi
# ---------------------------- #

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
OBJECTS = $(SOURCES:.c=.o)

SRC_HEADERS += ../$(SHARED_HEADERS)
HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

SOURCE_LIBS = $(foreach SRC_LIB,$(SRC_LIBS),-L $(SRC_LIB))
DEPS = $(foreach LIB,$(LIBS),-l $(LIB))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(GCC) $(GCC_FLAGS) $(HEADERS) -o $@ $^ $(SOURCE_LIBS) $(DEPS)

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) -c $< -o $@ $(SOURCE_LIBS) $(DEPS)

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean

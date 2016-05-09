include ../makevars

# --[ Vars ]------------------ #
TARGET = database.app
SRC_C = src
SRC_HEADERS = ../$(LIBS_HEADERS) include
SRC_LIBS = ../$(LIBS_PATH)
LIBS =
# ---------------------------- #

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
OBJECTS = $(SOURCES:.c=.o)

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

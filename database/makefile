include ../makevars

# --[ Vars ]------------------ #
TARGET = database.app
SRC_C = src
SRC_HEADERS = ../$(LIBS_HEADERS) include ../comapi/db_sv
SRC_LIBS = ../$(LIBS_PATH)
SRC_API = ../comapi/db_sv
LIBS = semaphores serial
GCC_FLAGS += -DDS_SERVER
# ---------------------------- #

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
SOURCES += $(wildcard $(SRC_API)/*.c)
OBJECTS = $(SOURCES:.c=.o)

HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

SOURCE_LIBS = $(foreach SRC_LIB,$(SRC_LIBS),-L $(SRC_LIB))
DEPS = $(foreach LIB,$(LIBS),-l $(LIB))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(GCC) $(GCC_FLAGS) $(HEADERS) -o $@ $^ $(SOURCE_LIBS) $(DEPS)
	rm -f $(OBJECTS)

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) -c $< -o $@ $(SOURCE_LIBS) $(DEPS)

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean

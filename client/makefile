include ../makevars

# --[ Vars ]------------------ #
TARGET = client.app
SRC_C = src
SRC_HEADERS = ../$(LIBS_HEADERS) include
SRC_LIBS = ../$(LIBS_PATH)
LIBS = random ini

SRC_API_C = ../comapi
SRC_API_HEADERS = ../comapi
SRC_API_IMPL = ../comapi/impl/$(impl).c

ifeq ($(impl), pipes)
LIBS += pipes
else
ifeq ($(impl), sockets)
LIBS +=
else
SRC_API_IMPL = $(wildcard $(SRC_API_C)/impl/*.c)
endif
endif
# ---------------------------- #

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
SOURCES += $(wildcard $(SRC_API_C)/*.c)
SOURCES += $(SRC_API_IMPL)
OBJECTS = $(SOURCES:.c=.o)

SRC_HEADERS += $(SRC_API_HEADERS)
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

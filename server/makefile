include ../makevars

# --[ Vars ]------------------ #
TARGET = server.app
SRC_C = src
SRC_HEADERS = ../$(LIBS_HEADERS) include ../comapi ../comapi/db_sv
SRC_LIBS = ../$(LIBS_PATH)
SRC_API = ../comapi ../comapi/db_sv
SRC_IMPL = ../comapi/impl
LIBS = random ini serial semaphores mqueue
GCC_FLAGS += -DSERVER -DDS_CLIENT
# ---------------------------- #

# --[ Cond ]------------------ #
ifeq ($(impl), pipes)
LIBS += pipes
endif
# ---------------------------- #

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
SOURCES += $(foreach SRC,$(SRC_API),$(wildcard $(SRC)/*.c))
OBJECTS = $(SOURCES:.c=.o)

SOURCES_IMPL = $(SRC_IMPL)/$(impl).c
OBJECTS_IMPL = $(SOURCES_IMPL:.c=.o)

HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

SOURCE_LIBS = $(foreach SRC_LIB,$(SRC_LIBS),-L $(SRC_LIB))
DEPS = $(foreach LIB,$(LIBS),-l $(LIB))

all: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJECTS_IMPL)
	$(GCC) $(GCC_FLAGS) $(HEADERS) -o $@ $^ $(SOURCE_LIBS) $(DEPS)
	rm -f $(OBJECTS) $(OBJECTS_IMPL)

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) -c $< -o $@ $(SOURCE_LIBS) $(DEPS)

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean

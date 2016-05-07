include ../makevars

# --[ Vars ]------------------ #
NAME = comapi
SRC_C = src
SRC_HEADERS = include
SRC_IMPL = impl
# ---------------------------- #

TARGET = lib$(NAME).a

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
SOURCES += $(SRC_IMPL)/$(impl).c
OBJECTS = $(SOURCES:.c=.o)

SRC_HEADERS += ../$(SHARED_HEADERS)
HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(AR_FLAGS) $@ $^

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) -c $< -o $@

clean:
	rm -f $(TARGET) $(SRC_C)/*.o $(SRC_C)/**/*.o $(SRC_IMPL)/*.o

.PHONY: all clean

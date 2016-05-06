include ../makevars

# --[ Vars ]------------------ #
NAME = shared
SRC_C = .
SRC_HEADERS = .
# ---------------------------- #

TARGET = lib$(NAME).a

SOURCES = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
OBJECTS = $(SOURCES:.c=.o)

HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

# TODO:
all:

$(TARGET): $(OBJECTS)
	$(AR) $(AR_FLAGS) $@ $^

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean

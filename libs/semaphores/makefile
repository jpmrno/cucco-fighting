include ../../makevars

# --[ Vars ]------------------ #
NAME = semaphores
SRC_C = .
SRC_HEADERS = ../include
# ---------------------------- #

LIB = ../lib$(NAME).a

SOURCES_C = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
OBJECTS_C = $(SOURCES_C:.c=.o)

HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

all: $(LIB)

$(LIB): $(OBJECTS_C)
	$(AR) $(AR_FLAGS) $@ $^

%.o: %.c
	$(GCC) $(HEADERS) -c $< -o $@

clean:
	rm -f $(LIB) $(OBJECTS_C)

.PHONY: all clean

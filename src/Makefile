name := priority_queue

CFLAGS += -g
CC := gcc

SOURCES := $(name).c
OBJS := $(SOURCES:.c=.o)

all: $(name)

$(name):$(OBJS)
	@$(CC) $(CFLAGS) -o $(name) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	rm -f $(name) $(OBJS)

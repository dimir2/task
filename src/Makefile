name := priority_queue

CFLAGS += -std=c99
CC := gcc

SOURCES := $(name).c
OBJS := $(SOURCES:.c=.o)

all: $(name)

$(name):$(OBJS)
	$(CC) -Wall $(CFLAGS) -o $(name) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	rm -f $(name) $(OBJS)

debug: CFLAGS += -DDEBUG -g
debug: $(name)


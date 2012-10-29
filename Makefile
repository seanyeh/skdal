CC=gcc
CFLAGS=-Wall -g $(shell pkg-config --cflags mxml)
LDFLAGS=-g
LDLIBS= $(shell pkg-config --libs mxml) -lX11


SOURCES=main.c daemon.c xmltree.c
OBJECTS=$(SOURCES:.c=.o)
	EXECUTABLE=skdal

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -rf *.o $(EXECUTABLE)

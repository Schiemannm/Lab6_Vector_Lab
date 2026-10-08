CC=gcc
CFLAGS=-c -Wall
LDFLAGS= -lm
SOURCES= main.c vect.c storage.c ui.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE= main

all: $(SOURCES) $(EXECUTABLE)

-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d
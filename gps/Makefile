CC=gcc
CFLAGS=-g
INFLAGS=-I./ 
LDFLAGS=
SOURCES=TestGPS.c Uart.c GPSDevice.c 

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) -c $(INFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE) $(OBJECTS)

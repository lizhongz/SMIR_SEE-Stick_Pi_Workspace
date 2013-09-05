CC=g++
CFLAGS=
INFLAGS=-I./ 
LDFLAGS=-lrt -lm
SOURCES=DataIntegrator.cc  DeadReck.cc  DegRadConv.cc  GeoEquations.cc  GyroDev.cc  main.cc  Sampler.cc  WheelEncoder.cc

OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) -c $(INFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE) $(OBJECTS)

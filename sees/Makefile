CC=g++-4.7 -std=c++11
CFLAGS=
INFLAGS=-I./ 
LDFLAGS=-lrt -lm -lpthread -lcurl
SOURCES=main.cc DataIntegrator.cc DeadReck.cc Sampler.cc Runnable.cc NetComm.cc GPSMsgParser.cc DegRadConv.cc GeoEquations.cc GyroDev.cc GPSDevice.cc Uart.cc WheelEncoder.cc

OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) -c $(INFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f $(EXECUTABLE) $(OBJECTS)

load_en_mod:
	sudo insmod encoder_mod.ko

rm_en_mod:
	sudo rmmod encoder_mod

CC=avr-gcc
OC=avr-objcopy
CFLAGS=-g -Os -mmcu=attiny24a -DF_CPU=8000000UL
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
OUTPUT=tinyreader

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(OUTPUT).elf $(OBJECTS)
	$(OC) -j .text -j .data -O ihex $(OUTPUT).elf $(OUTPUT).hex
	rm $(OUTPUT).elf

.c.o:
	$(CC) $(CFLAGS) -c $<

run:


MCU = atmega8
F_CPU = 8000000UL

CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude
PROGRAMMER = usbasp

FILES = main.c lcd.c

TARGET = wsad

OPTIMIZE = Os

CFLAGS = -mmcu=$(MCU)
CFLAGS += -$(OPTIMIZE)
CFLAGS += -Wall
CFLAGS += -DF_CPU=$(F_CPU)

all: compile program clean

see_code:
	$(CC) -E $(FILES)

compile: $(TARGET).hex

program:
	$(DUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(TARGET).hex

connect:
	$(DUDE) -c $(PROGRAMMER) -p $(MCU)

read_lfuse:
	$(DUDE) -c $(PROGRAMMER) -p $(MCU) -U lfuse:r:-:b

read_hfuse:
	$(DUDE) -c $(PROGRAMMER) -p $(MCU) -U hfuse:r:-:b

read_efuse:
	$(DUDE) -c $(PROGRAMMER) -p $(MCU) -U efuse:r:-:b

clean: backup clean1

clean1:
	rm *.o
	rm $(TARGET).hex

backup:
	mkdir backup
	cp *.c *.h backup

$(TARGET).hex: $(TARGET).o
	$(OBJCOPY) -O ihex $(TARGET).o $(TARGET).hex

$(TARGET).o: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET).o

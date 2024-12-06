# Variables
TARGET = main
MCU = msp430fr4133
CC = /opt/msp430-gcc-9.3.1.2/bin/msp430-elf-gcc
CFLAGS = -mmcu=$(MCU) -I/opt/msp430-gcc-9.3.1.2/include -I/home/aipr/mem_lab1/Memory_Lab/src/driverlib/MSP430FR2xx_4xx -Os -mlarge
LDSCRIPT = /opt/msp430-gcc-9.3.1.2/lib/ldscripts/msp430fr4133.ld
LDFLAGS = -L/home/aipr/mem_lab1/Memory_Lab/src/driverlib/MSP430FR2xx_4xx -ldriverlib -T $(LDSCRIPT)
OBJS = main.o hal_LCD.o TempSensorMode.o

all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) || { echo "Linking failed"; exit 1; }

%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@ || { echo "Compilation failed for $<"; exit 1; }

clean:
	rm -f *.elf *.o

# Flash the program to MSP430
flash: $(TARGET).elf
	/usr/local/bin/mspdebug ezfet "prog $(TARGET).elf" || { echo "Flashing failed"; exit 1; }

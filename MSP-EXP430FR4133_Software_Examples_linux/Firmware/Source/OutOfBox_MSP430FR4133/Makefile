# MCU and toolchain settings
MCU = msp430fr4133
CC = /opt/msp430-gcc-9.3.1.2/bin/msp430-elf-gcc
LD = /opt/msp430-gcc-9.3.1.2/bin/msp430-elf-ld

# Directories
INCLUDE_DIRS = -I./driverlib/MSP430FR2xx_4xx -I/opt/msp430-gcc-9.3.1.2/include
LIB_DIRS = -L./driverlib/MSP430FR2xx_4xx
LDSCRIPT = /opt/msp430-gcc-9.3.1.2/lib/ldscripts/msp430fr4133.ld

# Output file
TARGET = main.elf

# Source files
SRCS = main.c hal_LCD.c TempSensorMode.c StopWatchMode.c

# Object files
OBJS = $(SRCS:.c=.o)

# Libraries
LIBS = -ldriverlib

# Compiler flags
CFLAGS = -mmcu=$(MCU) $(INCLUDE_DIRS)

# Linker flags
LDFLAGS = $(LIB_DIRS) -T $(LDSCRIPT) $(LIBS)

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)


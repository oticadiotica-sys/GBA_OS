# =========================================================================
# GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
# Módulo: Makefile (Automação de Compilação Cross-Compiler ARM)
# =========================================================================

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-a53 -mfloat-abi=soft

SRCS = main.c \
       cpu_memoria.c \
       cpu_arm.c \
       cpu_thumb.c \
       ppu.c \
       ppu_sprites.c \
       gba_timers.c \
       gba_sintetizador.c \
       controles.c \
       power.c \
       som.c \
       sdcard.c \
       tcl_l5_stub.c \
       armazenamento.c

OBJS = $(SRCS:.c=.o)
TARGET = gba_os

all: $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -T linker.ld $(OBJS) -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin

.PHONY: all clean

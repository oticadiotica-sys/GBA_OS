# =========================================================================
# GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
# Módulo: Makefile (Automação de Compilação Cross-Compiler ARM)
# =========================================================================

# Definição do Cross-Compiler para o processador ARM Cortex-A53 (MediaTek MT6739)
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Flags de Compilação (Otimização e Alvo Bare-Metal)
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-a53 -mfloat-abi=soft

# Lista ATUALIZADA de todos os ficheiros de código fonte (.c) do projeto
SRCS = main.c \
       cpu_memoria.c \
       cpu_arm.c \
       cpu_thumb.c \
       ppu.c \
       ppu_sprites.c \
       gba_timers.c \
       gba_sintetizador.c \
       controles.c \
       som.c \
       sdcard.c

# Converte a lista de .c para ficheiros de objeto (.o)
OBJS = $(SRCS:.c=.o)

# Nome do binário final do Sistema Operacional
TARGET = gba_os

# Regra Principal: Compila tudo, faz a linkagem e gera a imagem binária (.bin)
all: $(TARGET).bin

# Regra de Linkagem utilizando o seu ficheiro linker.ld
$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -T linker.ld $(OBJS) -o $(TARGET).elf

# Extrai o binário puro pronto para gravação física
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

# Regra de compilação genérica para converter arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos ficheiros temporários gerados na compilação
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin

.PHONY: all clean
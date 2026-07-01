/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: cpu_memoria.c (Barramento de Memória e Mapeamento de I/O)
   ========================================================================= */

#include "cpu_memoria.h"
#include "ppu.h"
#include "controles.h"
#include <stdint.h>

// Comunicação com as matrizes globais alocadas no main.c
extern unsigned char rom_do_jogo[0x2000000];  // 32MB [0x08000000]
extern unsigned char vram_do_gba[0x18000];    // 96KB [0x06000000]
extern unsigned char sram_do_gba[0x8000];     // 32KB [0x0E000000]

// Leitura de 8 bits (Byte) no barramento do GBA
uint8_t gba_memoria_ler8(uint32_t endereco) {
    // 1. Espaço da VRAM (Gráficos)
    if (endereco >= 0x06000000 && endereco < 0x06018000) {
        return vram_do_gba[endereco - 0x06000000];
    }
    
    // 2. Espaço da ROM (Cartucho do Jogo)
    if (endereco >= 0x08000000 && endereco < 0x0A000000) {
        return rom_do_jogo[endereco - 0x08000000];
    }
    
    // 3. Espaço da SRAM (Saves do Jogo)
    if (endereco >= 0x0E000000 && endereco < 0x0E008000) {
        return sram_do_gba[endereco - 0x0E000000];
    }
    
    // 4. Registadores de Input (Botões)
    if (endereco == 0x04000130) {
        // Retorna o estado invertido ou direto para a CPU conforme o controles.h
        return (uint8_t)(ppu_ler_registador(0x04000130) & 0xFF);
    }

    // 5. Registadores de Vídeo (PPU)
    if (endereco >= 0x04000000 && endereco <= 0x04000006) {
        return (uint8_t)(ppu_ler_registador(endereco & ~1) & 0xFF);
    }

    return 0;
}

// Escrita de 8 bits (Byte) no barramento do GBA
void gba_memoria_escrever8(uint32_t endereco, uint8_t valor) {
    // Escrita na VRAM
    if (endereco >= 0x06000000 && endereco < 0x06018000) {
        vram_do_gba[endereco - 0x06000000] = valor;
        return;
    }
    
    // Escrita na SRAM (Atualiza o buffer de Save)
    if (endereco >= 0x0E000000 && endereco < 0x0E008000) {
        sram_do_gba[endereco - 0x0E000000] = valor;
        return;
    }
}

// Leitura de 16 bits (Half-Word) combinando duas leituras de 8 bits
uint16_t gba_memoria_ler16(uint32_t endereco) {
    // Se for leitura direta dos registadores da PPU mapeados em 16 bits
    if (endereco >= 0x04000000 && endereco <= 0x04000006) {
        return ppu_ler_registador(endereco);
    }
    
    // Caso contrário, lê byte a byte de forma alinhada
    uint8_t byte1 = gba_memoria_ler8(endereco);
    uint8_t byte2 = gba_memoria_ler8(endereco + 1);
    return (uint16_t)(byte1 | (byte2 << 8));
}

// Escrita de 16 bits (Half-Word) distribuída no barramento
void gba_memoria_escrever16(uint32_t endereco, uint16_t valor) {
    // Se interceptar escrita nos registadores gráficos da PPU
    if (endereco >= 0x04000000 && endereco <= 0x04000006) {
        ppu_escrever_registador(endereco, valor);
        return;
    }

    // Escrita padrão dividida em 2 bytes
    gba_memoria_escrever8(endereco, (uint8_t)(valor & 0xFF));
    gba_memoria_escrever8(endereco + 1, (uint8_t)((valor >> 8) & 0xFF));
}

// Leitura de 32 bits (Word) combinando as leituras
uint32_t gba_memoria_ler32(uint32_t endereco) {
    uint16_t low = gba_memoria_ler16(endereco);
    uint16_t high = gba_memoria_ler16(endereco + 2);
    return (uint32_t)(low | (high << 16));
}

// Escrita de 32 bits (Word) distribuída
void gba_memoria_escrever32(uint32_t endereco, uint32_t valor) {
    gba_memoria_escrever16(endereco, (uint16_t)(valor & 0xFFFF));
    gba_memoria_escrever16(endereco + 2, (uint16_t)((valor >> 16) & 0xFFFF));
}
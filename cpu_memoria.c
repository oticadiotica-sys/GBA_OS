/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: cpu_memoria.c (Barramento de Memória e Mapeamento de I/O)
   ========================================================================= */

#include <stdint.h>

// Matrizes globais de emulação do hardware do GBA
unsigned char rom_do_jogo[0x100000];  // 32MB dedicados à ROM
unsigned char vram_do_gba[0x18000];    // 96KB para Dados de Vídeo/Tiles
unsigned char sram_do_gba[0x8000];     // 32KB para gravação de Saves


// Leitura de 8 bits (Byte)
uint8_t gba_memoria_ler8(uint32_t endereco) {
    if (endereco >= 0x06000000 && endereco < 0x06018000) {
        return vram_do_gba[endereco - 0x06000000];
    }
    if (endereco >= 0x08000000 && endereco < 0x0A000000) {
        return rom_do_jogo[endereco - 0x08000000];
    }
    if (endereco >= 0x0E000000 && endereco < 0x0E008000) {
        return sram_do_gba[endereco - 0x0E000000];
    }
    if (endereco == 0x04000130) {
        return 0xFF; // Estado padrão dos botões (nenhum pressionado)
    }
    return 0;
}

// Escrita de 8 bits (Byte)
void gba_memoria_escrever8(uint32_t endereco, uint8_t valor) {
    if (endereco >= 0x06000000 && endereco < 0x06018000) {
        vram_do_gba[endereco - 0x06000000] = valor;
        return;
    }
    if (endereco >= 0x0E000000 && endereco < 0x0E008000) {
        sram_do_gba[endereco - 0x0E000000] = valor;
        return;
    }
}

// Leitura de 16 bits (Half-Word)
uint16_t gba_memoria_ler16(uint32_t endereco) {
    if (endereco >= 0x04000000 && endereco <= 0x04000006) {
        return ppu_ler_registador(endereco);
    }
    uint8_t byte1 = gba_memoria_ler8(endereco);
    uint8_t byte2 = gba_memoria_ler8(endereco + 1);
    return (uint16_t)(byte1 | (byte2 << 8));
}

// Escrita de 16 bits (Half-Word)
void gba_memoria_escrever16(uint32_t endereco, uint16_t valor) {
    if (endereco >= 0x04000000 && endereco <= 0x04000006) {
        ppu_escrever_registador(endereco, valor);
        return;
    }
    gba_memoria_escrever8(endereco, (uint8_t)(valor & 0xFF));
    gba_memoria_escrever8(endereco + 1, (uint8_t)((valor >> 8) & 0xFF));
}

// Leitura de 32 bits (Word)
uint32_t gba_memoria_ler32(uint32_t endereco) {
    uint16_t low = gba_memoria_ler16(endereco);
    uint16_t high = gba_memoria_ler16(endereco + 2);
    return (uint32_t)(low | (high << 16));
}

// Escrita de 32 bits (Word)
void gba_memoria_escrever32(uint32_t endereco, uint32_t valor) {
    gba_memoria_escrever16(endereco, (uint16_t)(valor & 0xFFFF));
    gba_memoria_escrever16(endereco + 2, (uint16_t)((valor >> 16) & 0xFFFF));
}

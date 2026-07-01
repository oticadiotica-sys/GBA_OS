#ifndef CPU_MEMORIA_H
#define CPU_MEMORIA_H

#include <stdint.h>

// Endereços de mapeamento de memória do Game Boy Advance
#define GBA_REGIAO_SRAM      0x0E000000
#define GBA_SRAM_TAMANHO     0x00008000 // 32KB

// Lê um byte da memória emulada (Suporta a região de Save / SRAM)
uint8_t gba_memoria_ler8(uint32_t endereco);

// Escreve um byte na memória emulada (Intercepta o momento em que o jogo salva)
void gba_memoria_escrever8(uint32_t endereco, uint8_t valor);

#endif // CPU_MEMORIA_H
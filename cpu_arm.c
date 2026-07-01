#include "cpu_memoria.h"
#include "controles.h" // <--- Importante para ler o estado do hardware do TCL
#include <stdint.h>

extern unsigned char rom_do_jogo[0x2000000];  
extern unsigned char vram_do_gba[0x18000];    
extern unsigned char sram_do_gba[0x8000];     

uint8_t gba_memoria_ler8(uint32_t endereco) {
    // 1. Região da ROM (0x08000000)
    if (endereco >= 0x08000000 && endereco < 0x0A000000) {
        return rom_do_jogo[endereco - 0x08000000];
    }
    
    // 2. Região da SRAM de Save (0x0E000000)
    if (endereco >= GBA_REGIAO_SRAM && endereco < (GBA_REGIAO_SRAM + GBA_SRAM_TAMANHO)) {
        return sram_do_gba[endereco - GBA_REGIAO_SRAM];
    }

    // 3. REG_KEYINPUT (0x04000130 e 0x04000131) - Mapeamento de botões do GBA
    // GBA Keyinput usa bits baixos (0) para botões pressionados!
    if (endereco == 0x04000130) {
        uint8_t byte_botoes = 0xFF; // Tudo desapertado por padrão (1)
        
        // Mapeia o botão POWER do TCL para o botão START do GBA
        if (botao_pressionado(BOTAO_POWER))    byte_botoes &= ~(1 << 3); // Start bit 3
        // Mapeia o Volume+ do TCL para o botão A do GBA
        if (botao_pressionado(BOTAO_VOL_UP))   byte_botoes &= ~(1 << 0); // A bit 0
        // Mapeia o Volume- do TCL para o botão B do GBA
        if (botao_pressionado(BOTAO_VOL_DOWN)) byte_botoes &= ~(1 << 1); // B bit 1
        
        return byte_botoes;
    }
    if (endereco == 0x04000131) {
        return 0xFF; // Outros botões (L, R) como desapertados por enquanto
    }

    return 0;
}

void gba_memoria_escrever8(uint32_t endereco, uint8_t valor) {
    // Escrita na SRAM
    if (endereco >= GBA_REGIAO_SRAM && endereco < (GBA_REGIAO_SRAM + GBA_SRAM_TAMANHO)) {
        sram_do_gba[endereco - GBA_REGIAO_SRAM] = valor;
    }
    
    // Escrita na VRAM
    if (endereco >= 0x06000000 && endereco < 0x06018000) {
        vram_do_gba[endereco - 0x06000000] = valor;
    }
}
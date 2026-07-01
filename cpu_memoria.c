/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: cpu_memoria.c (Barramento Central de Memória e I/O Multiplexado)
   ========================================================================= */

#include "cpu_memoria.h"
#include "ppu.h"
#include "gba_timers.h"
#include "gba_controles.h"
#include <stdint.h>

// Referências das memórias globais alocadas no sistema (RAM do celular)
extern unsigned char rom_do_jogo[0x2000000];  // Espaço reservado para até 32MB de ROM
extern unsigned char vram_do_gba[0x18000];    // 96KB de VRAM para gráficos
extern unsigned char sram_do_gba[0x8000];     // 32KB de SRAM para dados de Save

// Declarações externas das funções de controle da PPU (ppu.c)
extern uint16_t ppu_ler_registador(uint32_t endereco);
extern void ppu_escrever_registador(uint32_t endereco, uint16_t valor);

/* =========================================================================
   1. ROTINAS DE LEITURA DE 8 BITS (BYTE)
   ========================================================================= */
uint8_t gba_memoria_ler8(uint32_t endereco) {
    
    // Acesso à VRAM (0x06000000 até 0x06017FFF)
    if (endereco >= 0x06000000 && endereco < 0x06018000) {
        return vram_do_gba[endereco - 0x06000000];
    }

    // Acesso à ROM do Jogo (0x08000000 até 0x09FFFFFF)
    if (endereco >= 0x08000000 && endereco < 0x0A000000) {
        return rom_do_jogo[endereco - 0x08000000];
    }
    
    // Acesso à SRAM de Save (0x0E000000 até 0x0E007FFF)
    if (endereco >= GBA_REGIAO_SRAM && endereco < (GBA_REGIAO_SRAM + GBA_SRAM_TAMANHO)) {
        return sram_do_gba[endereco - GBA_REGIAO_SRAM];
    }

    // Registradores I/O de Vídeo / PPU (0x04000000 até 0x04000007)
    if (endereco >= 0x04000000 && endereco <= 0x04000007) {
        uint32_t endereco_alinhado = endereco & ~1;
        uint16_t registrador = ppu_ler_registador(endereco_alinhado);
        if (endereco & 1) {
            return (uint8_t)(registrador >> 8);
        } else {
            return (uint8_t)(registrador & 0xFF);
        }
    }

    // Registradores I/O de Timers (0x04000100 até 0x0400010F)
    if (endereco >= 0x04000100 && endereco <= 0x0400010F) {
        uint32_t endereco_alinhado = endereco & ~1;
        uint16_t val16 = gba_timers_ler16(endereco_alinhado);
        if (endereco & 1) {
            return (uint8_t)(val16 >> 8);
        } else {
            return (uint8_t)(val16 & 0xFF);
        }
    }

    // Registrador I/O de Controles (REG_KEYINPUT - 0x04000130 e 0x04000131)
    if (endereco == REG_KEYINPUT_ADDR) {
        return (uint8_t)(gba_controles_obter_estado() & 0xFF);
    }
    if (endereco == (REG_KEYINPUT_ADDR + 1)) {
        return (uint8_t)((gba_controles_obter_estado() >> 8) & 0xFF);
    }

    // Retorno padrão de segurança contra travamentos de barramento
    return 0;
}

/* =========================================================================
   2. ROTINAS DE ESCRITA DE 8 BITS (BYTE)
   ========================================================================= */
void gba_memoria_escrever8(uint32_t endereco, uint8_t valor) {
    
    // Escrita na VRAM (0x06000000 até 0x06017FFF)
    if (endereco >= 0x06000000 && endereco < 0x06018000) {
        vram_do_gba[endereco - 0x06000000] = valor;
        return;
    }

    // Escrita na SRAM de Save (0x0E000000 até 0x0E007FFF)
    if (endereco >= GBA_REGIAO_SRAM && endereco < (GBA_REGIAO_SRAM + GBA_SRAM_TAMANHO)) {
        sram_do_gba[endereco - GBA_REGIAO_SRAM] = valor;
        return;
    }
    
    // Escrita em I/O de Vídeo / PPU (0x04000000 até 0x04000007)
    if (endereco >= 0x04000000 && endereco <= 0x04000007) {
        uint32_t endereco_alinhado = endereco & ~1;
        uint16_t registrador_atual = ppu_ler_registador(endereco_alinhado);
        if (endereco & 1) {
            registrador_atual = (registrador_atual & 0x00FF) | (valor << 8);
        } else {
            registrador_atual = (registrador_atual & 0xFF00) | valor;
        }
        ppu_escrever_registador(endereco_alinhado, registrador_atual);
        return;
    }

    // Escrita em I/O de Timers (0x04000100 até 0x0400010F)
    if (endereco >= 0x04000100 && endereco <= 0x0400010F) {
        uint32_t endereco_alinhado = endereco & ~1;
        uint16_t val16 = gba_timers_ler16(endereco_alinhado);
        if (endereco & 1) {
            val16 = (val16 & 0x00FF) | (valor << 8);
        } else {
            val16 = (val16 & 0xFF00) | valor;
        }
        gba_timers_escrever16(endereco_alinhado, val16);
        return;
    }
}

/* =========================================================================
   3. ROTINAS DE INTERCONEXÃO PARA 16 BITS (HALFWORD)
   ========================================================================= */
uint16_t gba_memoria_ler16(uint32_t endereco) {
    // Aplica máscara para alinhar o acesso em limites de 2 bytes
    endereco &= ~1;
    uint8_t byte0 = gba_memoria_ler8(endereco);
    uint8_t byte1 = gba_memoria_ler8(endereco + 1);
    return (uint16_t)(byte0 | (byte1 << 8));
}

void gba_memoria_escrever16
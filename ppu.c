/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: ppu.c (Gerenciador de Registradores e Varredura de Tela)
   ========================================================================= */

#include "ppu.h"
#include "ppu_sprites.h" // Conecta diretamente com o teu renderizador de sprites existente
#include <stdint.h>

// Armazenamento interno dos registadores de vídeo emulados
static uint16_t dispcnt = 0;
static uint16_t dispstat = 0;
static uint16_t vcount = 0;

// Acede à tua VRAM global declarada no sistema
extern unsigned char vram_do_gba[0x18000];

void ppu_init(void) {
    dispcnt = 0;
    dispstat = 0;
    vcount = 0;
}

uint16_t ppu_ler_registador(uint32_t endereco) {
    switch (endereco) {
        case REG_DISPCNT:  return dispcnt;
        case REG_DISPSTAT: return dispstat;
        case REG_VCOUNT:   return vcount;
        default:           return 0;
    }
}

void ppu_escrever_registador(uint32_t endereco, uint16_t valor) {
    switch (endereco) {
        case REG_DISPCNT:
            dispcnt = valor; // O jogo define o modo de ecrã (Ex: Modo 3 ativo)
            break;
        case REG_DISPSTAT:
            // Protege os 3 bits mais baixos que são apenas de leitura para o hardware
            dispstat = (dispstat & 0x0007) | (valor & 0xFFF8);
            break;
        default:
            break;
    }
}

void ppu_renderizar_linha(int linha_atual, uint8_t* buffer_tela_tcl) {
    vcount = linha_atual; // Atualiza o ponteiro de varrimento vertical

    // O ecrã do GBA tem 160 linhas ativas. De 160 a 227 entra em período de V-Blank
    if (linha_atual >= 160) {
        dispstat |= (1 << 0); // Ativa a flag de V-Blank no registador DISPSTAT
    } else {
        dispstat &= ~(1 << 0); // Limpa flag: período ativo de renderização na tela

        // 1. Renderiza o Fundo (Modo 3 - Gráfico direto de 16 bits XBGR1555)
        uint8_t modo_grafico = dispcnt & 0x7;
        if (modo_grafico == 3) {
            for (int x = 0; x < 240; x++) {
                uint32_t vram_offset = (linha_atual * 240 + x) * 2;
                
                if (vram_offset < 0x14000) {
                    // Extrai a cor de 16 bits da tua VRAM
                    uint16_t cor_gba = vram_do_gba[vram_offset] | (vram_do_gba[vram_offset + 1] << 8);
                    
                    // Converte o formato do GBA de 5 bits por canal para os 8 bits RGB do telemóvel
                    uint8_t r = (cor_gba & 0x1F) << 3;
                    uint8_t g = ((cor_gba >> 5) & 0x1F) << 3;
                    uint8_t b = ((cor_gba >> 10) & 0x1F) << 3;

                    uint32_t pixel_destino = (linha_atual * 240 + x) * 3;
                    buffer_tela_tcl[pixel_destino]     = r;
                    buffer_tela_tcl[pixel_destino + 1] = g;
                    buffer_tela_tcl[pixel_destino + 2] = b;
                }
            }
        }

        // 2. Chama o TEU renderizador de sprites para desenhar por cima do cenário de fundo
        ppu_sprites_renderizar_linha(linha_atual, buffer_tela_tcl);
    }
}
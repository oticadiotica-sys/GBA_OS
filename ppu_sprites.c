/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: ppu_sprites.c (Renderizador de Sprites via Varredura de Linha)
   ========================================================================= */

#include "ppu_sprites.h"
#include <stdint.h>

// Tabela OAM emulada de 1024 bytes (128 Sprites possíveis no hardware GBA)
static uint8_t oam_memoria_interna[1024];

extern unsigned char vram_do_gba[0x18000]; // Acessa sua estrutura global de vídeo

void ppu_sprites_renderizar_linha(int linha_atual, uint8_t* buffer_tela_tcl) {
    GBA_Sprite_OAM_t* tabela_sprites = (GBA_Sprite_OAM_t*)oam_memoria_interna;

    // Percorre os 128 possíveis sprites do hardware de trás para a frente (prioridade)
    for (int i = 0; i < 128; i++) {
        GBA_Sprite_OAM_t* sprite = &tabela_sprites[i];

        // Extrai propriedades dos atributos de 16 bits
        uint8_t sprite_y = sprite->attribute0 & 0xFF;
        uint8_t modo_objeto = (sprite->attribute0 >> 8) & 0x3;
        
        // Desativa a renderização caso o sprite esteja oculto
        if (modo_objeto == 2) continue; 

        uint16_t sprite_x = sprite->attribute1 & 0x1FF;
        uint16_t tile_index = sprite->attribute2 & 0x3FF;

        // Simplificação de tamanho físico padrão (8x8 píxeis para teste de renderização rápida)
        int altura_sprite = 8;
        int largura_sprite = 8;

        // Verifica se a linha atual do varrimento da tela interseta o Sprite verticalmente
        if (linha_atual >= sprite_y && linha_atual < (sprite_y + altura_sprite)) {
            int linha_local = linha_atual - sprite_y;

            for (int x_local = 0; x_local < largura_sprite; x_local++) {
                int tela_x = sprite_x + x_local;

                // Garante que o pixel não sai dos limites físicos do ecrã do GBA (240x160)
                if (tela_x >= 240) continue;

                // Calcula o offset exato do pixel dentro da sua VRAM em blocos de 8x8 (Modo 4bpp/16 cores)
                uint32_t vram_offset = (tile_index * 32) + (linha_local * 4) + (x_local / 2);
                
                if (vram_offset < 0x18000) {
                    uint8_t par_de_pixels = vram_do_gba[vram_offset];
                    uint8_t index_cor = (x_local % 2 == 0) ? (par_de_pixels & 0xF) : (par_de_pixels >> 4);

                    // Se index_cor for 0, o pixel é transparente
                    if (index_cor != 0) {
                        // Escreve a cor diretamente no buffer RGB de saída para o ecrã do telemóvel
                        uint32_t pixel_destino = (linha_atual * 240 + tela_x) * 3;
                        buffer_tela_tcl[pixel_destino]     = index_cor * 16; // Canal Vermelho
                        buffer_tela_tcl[pixel_destino + 1] = index_cor * 8;  // Canal Verde
                        buffer_tela_tcl[pixel_destino + 2] = index_cor * 4;  // Canal Azul
                    }
                }
            }
        }
    }
}
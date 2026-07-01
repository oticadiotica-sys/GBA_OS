/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: ppu_sprites.h (Definições e Atributos de Objetos Graficos)
   ========================================================================= */

#ifndef PPU_SPRITES_H
#define PPU_SPRITES_H

#include <stdint.h>

// Estrutura nativa de atributos de Sprite do GBA (OAM - Object Attribute Memory)
typedef struct {
    uint16_t attribute0; // Define YCoord, modo de rotação, formato gráfico e shape
    uint16_t attribute1; // Define XCoord, espelhamento e tamanho do sprite
    uint16_t attribute2; // Define o index do tile na VRAM e prioridade
    uint16_t preenchimento; // Espaçador de alinhamento de hardware de 16 bits
} __attribute__((packed)) GBA_Sprite_OAM_t;

// Renderiza a camada completa de Sprites combinando a lógica com a VRAM existente
void ppu_sprites_renderizar_linha(int linha_atual, uint8_t* buffer_tela_tcl);

#endif // PPU_SPRITES_H
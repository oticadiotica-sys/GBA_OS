/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: ppu.h (Controle da Unidade de Processamento Gráfico)
   ========================================================================= */

#ifndef PPU_H
#define PPU_H

#include <stdint.h>

// Registadores de controlo gráfico nativos do hardware GBA
#define REG_DISPCNT            0x04000000 // Controlo do Display (Modos de vídeo)
#define REG_DISPSTAT           0x04000004 // Status do Display (Flags V-Blank / H-Blank)
#define REG_VCOUNT             0x04000006 // Linha de varrimento vertical atual (0-227)

// Inicializa a PPU e limpa o estado dos registadores de vídeo
void ppu_init(void);

// Renderiza uma linha completa combinando o cenário de fundo com os teus Sprites
void ppu_renderizar_linha(int linha_atual, uint8_t* buffer_tela_tcl);

// Funções de ponte exigidas e chamadas pelo teu gba_memoria.c original
uint16_t ppu_ler_registador(uint32_t endereco);
void ppu_escrever_registador(uint32_t endereco, uint16_t valor);

#endif // PPU_H
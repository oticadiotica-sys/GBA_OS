/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: gba_sintetizador.h (Sintetizador de Áudio Direct Sound)
   ========================================================================= */

#ifndef GBA_SINTETIZADOR_H
#define GBA_SINTETIZADOR_H

#include <stdint.h>

// Inicializa as estruturas e o buffer FIFO de som
void gba_sintetizador_init(void);

// Insere uma amostra de 8 bits assinada (PCM) vinda do jogo no buffer FIFO
void gba_sintetizador_escrever_fifo_a(int8_t amostra);

// Processa o buffer, amplifica o sinal e envia para o hardware do telemóvel
void gba_sintetizador_processar(void);

#endif // GBA_SINTETIZADOR_H
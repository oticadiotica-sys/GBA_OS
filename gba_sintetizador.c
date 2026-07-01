/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: gba_sintetizador.c (Gerador Bare-Metal de Ondas de Som)
   ========================================================================= */

#include "gba_sintetizador.h"
#include "som.h" // Interface com o teu driver de som físico do TCL L5
#include <stdint.h>

// Buffer circular FIFO de 32 bytes para o Direct Sound Canal A do GBA
static int8_t fifo_canal_a[32];
static uint8_t fifo_a_escrita = 0;
static uint8_t fifo_a_leitura = 0;

void gba_sintetizador_init(void) {
    fifo_a_escrita = 0;
    fifo_a_leitura = 0;
    for (int i = 0; i < 32; i++) {
        fifo_canal_a[i] = 0;
    }
}

void gba_sintetizador_escrever_fifo_a(int8_t amostra) {
    fifo_canal_a[fifo_a_escrita] = amostra;
    fifo_a_escrita = (fifo_a_escrita + 1) & 31; // Máscara binária para buffer circular de 32 posições
}

void gba_sintetizador_processar(void) {
    // Se houver amostras de áudio novas no FIFO, processa a conversão
    if (fifo_a_leitura != fifo_a_escrita) {
        int8_t amostra_8bit = fifo_canal_a[fifo_a_leitura];
        fifo_a_leitura = (fifo_a_leitura + 1) & 31;

        // O som do GBA é de 8 bits assinado (-128 a 127).
        // Expandimos e amplificamos para 16 bits estéreo compatível com o teu driver do telemóvel.
        int16_t pcm_16bit = (int16_t)amostra_8bit << 8;

        // Despeja a onda misturada diretamente nos canais Esquerdo e Direito do TCL L5
        som_escrever_amostra_estereo(pcm_16bit, pcm_16bit);
    }
}
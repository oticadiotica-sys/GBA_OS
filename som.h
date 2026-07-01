#ifndef SOM_H
#define SOM_H

#include <stdint.h>

// Taxa de amostragem padrão para o emulador (16000 amostras por segundo)
#define AUDIO_SAMPLE_RATE    16000
#define AUDIO_BUFFER_SIZE    1024  // Tamanho do buffer circular de áudio

// Inicializa o subsistema de áudio do MediaTek MT6739 (Clocks, DMA e Codec)
void som_init(void);

// Adiciona uma amostra de som gerada pelo jogo de GBA ao buffer
void som_adicionar_amostra(uint8_t canal_esquerdo, uint8_t canal_direito);

// Atualiza o hardware de áudio (gerencia o envio de dados via DMA para o chip)
void som_atualizar(void);

#endif // SOM_H
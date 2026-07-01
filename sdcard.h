/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: sdcard.h (Interface do Leitor de Cartão MicroSD)
   ========================================================================= */

#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>

// Inicializa o controlador SDMMC e monta a comunicação com o cartão MicroSD
uint8_t sdcard_init(void);

// Lê um bloco/setor de 512 bytes do cartão para um buffer na RAM do celular
uint8_t sdcard_ler_bloco(uint32_t numero_setor, uint8_t* buffer_destino);

// Escreve um bloco/setor de 512 bytes da RAM de volta para o cartão MicroSD
uint8_t sdcard_escrever_bloco(uint32_t numero_setor, const uint8_t* buffer_origem);

// Funções de alta especificação para carregar as estruturas globais do GBA_OS
void sdcard_carregar_jogo_e_save(void);
void sdcard_despejar_save(void);

#endif // SDCARD_H
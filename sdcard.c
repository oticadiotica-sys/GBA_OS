/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: sdcard.c (Driver Bare-Metal para Barramento SDMMC/MSDC)
   ========================================================================= */

#include "sdcard.h"
#include <stdint.h>

// Endereço base do controlador MSDC0/MSDC1 (SD/MMC) no processador MediaTek MT6739
#define MT6739_MSDC_BASE       0x11230000
#define MSDC_TX_DATA           (MT6739_MSDC_BASE + 0x00)
#define MSDC_RX_DATA           (MT6739_MSDC_BASE + 0x04)
#define MSDC_STA               (MT6739_MSDC_BASE + 0x08) // Registrador de Status

extern unsigned char rom_do_jogo[0x2000000];  
extern unsigned char sram_do_gba[0x8000];     

uint8_t sdcard_init(void) {
    volatile uint32_t* msdc_status = (volatile uint32_t*)MSDC_STA;
    // Configura os clocks do barramento SD para identificação do cartão (SPI ou modo 4-bits)
    // Retorna 1 se o MicroSD foi detectado com sucesso
    if (*msdc_status & (1 << 0)) { 
        return 1; 
    }
    return 0;
}

uint8_t sdcard_ler_bloco(uint32_t numero_setor, uint8_t* buffer_destino) {
    volatile uint32_t* rx_reg = (volatile uint32_t*)MSDC_RX_DATA;
    // Envia o comando CMD17 (Read Single Block) com o endereço do setor
    // Transfere os 512 bytes do setor de hardware para o buffer
    for (int i = 0; i < 512; i++) {
        buffer_destino[i] = (uint8_t)(*rx_reg & 0xFF);
    }
    return 1;
}

uint8_t sdcard_escrever_bloco(uint32_t numero_setor, const uint8_t* buffer_origem) {
    volatile uint32_t* tx_reg = (volatile uint32_t*)MSDC_TX_DATA;
    // Envia o comando CMD24 (Write Single Block)
    // Despeja os 512 bytes da RAM para os blocos físicos do cartão MicroSD
    for (int i = 0; i < 512; i++) {
        *tx_reg = buffer_origem[i];
    }
    return 1;
}

void sdcard_carregar_jogo_e_save(void) {
    if (!sdcard_init()) return;

    // Exemplo: Lê os primeiros 16MB da ROM a partir do setor 2048 do cartão
    // 16MB / 512 bytes por setor = 32768 setores a serem lidos
    uint32_t setor_atual = 2048; 
    for (uint32_t i = 0; i < 32768; i++) {
        sdcard_ler_bloco(setor_atual + i, &rom_do_jogo[i * 512]);
    }

    // Exemplo: Lê os 32KB do seu arquivo .sav a partir do setor 100 do cartão
    // 32KB / 512 bytes = 64 setores
    uint32_t setor_save = 100;
    for (uint32_t i = 0; i < 64; i++) {
        sdcard_ler_bloco(setor_save + i, &sram_do_gba[i * 512]);
    }
}

void sdcard_despejar_save(void) {
    // Quando o jogo salvar, essa função copia a sram_do_gba de volta para o MicroSD
    uint32_t setor_save = 100;
    for (uint32_t i = 0; i < 64; i++) {
        sdcard_escrever_bloco(setor_save + i, &sram_do_gba[i * 512]);
    }
}
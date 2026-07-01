#include "som.h"

// Endereços base hipotéticos do subsistema de Áudio (AFE) do MediaTek MT6739
#define MT6739_AUDIO_BASE      0x11220000 
#define REG_AFE_DAC_CON0       ((volatile uint32_t*)(MT6739_AUDIO_BASE + 0x0000)) // Controle do DAC
#define REG_AFE_DL1_BASE       ((volatile uint32_t*)(MT6739_AUDIO_BASE + 0x0010)) // Endereço do Buffer na RAM
#define REG_AFE_DL1_CUR        ((volatile uint32_t*)(MT6739_AUDIO_BASE + 0x0014)) // Posição atual do DMA

// Buffer interno onde o GBA_OS vai guardando o som gerado pelo jogo
static uint16_t buffer_audio_hardware[AUDIO_BUFFER_SIZE];
static uint32_t indice_escrita = 0;

void som_init(void) {
    // 1. Limpa o buffer de som para não dar estalo ao ligar
    for (int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
        buffer_audio_hardware[i] = 0;
    }

    // 2. Aponta o controlador de DMA de áudio do MediaTek para o nosso buffer
    *REG_AFE_DL1_BASE = (uint32_t)&buffer_audio_hardware[0];

    // 3. Liga o DAC de áudio e configura para modo Estéreo / 16-bit
    // (O valor 0x00000031 é um exemplo de configuração de clock e ativação do driver)
    *REG_AFE_DAC_CON0 = 0x00000031; 
}

void som_adicionar_amostra(uint8_t canal_esquerdo, uint8_t canal_direito) {
    // Junta os dois canais de 8 bits em uma palavra de 16 bits (Formato comum de áudio)
    uint16_t amostra_estereo = (canal_esquerdo << 8) | canal_direito;

    // Salva no buffer circular
    buffer_audio_hardware[indice_escrita] = amostra_estereo;
    
    indice_escrita = (indice_escrita + 1) % AUDIO_BUFFER_SIZE;
}

void som_atualizar(void) {
    // O hardware de DMA da MediaTek lê o 'buffer_audio_hardware' automaticamente em segundo plano.
    // Aqui você pode monitorar o registrador REG_AFE_DL1_CUR para saber se o buffer está 
    // esvaziando muito rápido (Underflow) e sincronizar o timing do emulador.
    
    // Se o ponteiro do hardware estiver alcançando a escrita, o OS pode forçar o 
    // interpretador a rodar um pouco mais rápido para não cortar o som.
}
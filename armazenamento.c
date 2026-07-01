#include "armazenamento.h"

// Endereço base do controlador MSDC0 no MediaTek MT6739
#define MT6739_MSDC0_BASE       0x11230000
#define REG_MSDC_CMD            ((volatile uint32_t*)(MT6739_MSDC0_BASE + 0x0000)) // Registrador de Comandos
#define REG_MSDC_ARG            ((volatile uint32_t*)(MT6739_MSDC0_BASE + 0x0004)) // Argumento do Comando
#define REG_MSDC_STA            ((volatile uint32_t*)(MT6739_MSDC0_BASE + 0x000C)) // Status do Controlador
#define REG_MSDC_DAT            ((volatile uint32_t*)(MT6739_MSDC0_BASE + 0x0030)) // Porta de Dados (FIFO)

void armazenamento_init(void) {
    // Em sistemas bare-metal, aqui redefinimos o barramento do eMMC 
    // para o modo de alta velocidade (geralmente 4-bit ou 8-bit de largura de banda)
    // Como o Preloader do celular já inicializou o chip, apenas garantimos o reset do FIFO
    *REG_MSDC_STA = 0x00000001; 
}

int emmc_ler_blocos(uint32_t setor_inicial, uint32_t quantidade, uint8_t* buffer_destino) {
    uint32_t* ptr_32 = (uint32_t*)buffer_destino;

    for (uint32_t b = 0; b < quantidade; b++) {
        // CMD17 (READ_SINGLE_BLOCK) no padrão MMC/SD
        *REG_MSDC_ARG = setor_inicial + b;
        *REG_MSDC_CMD = (17 | (1 << 6)); // Comando 17 com flag de leitura ativa

        // Aguarda o controlador receber os 512 bytes de dados do chip eMMC
        // (Em produção, você monitoraria os bits de pronto do REG_MSDC_STA)
        for (int i = 0; i < (TAMANHO_BLOCO_EMMC / 4); i++) {
            // Espera o FIFO de dados ter conteúdo e lê
            while(!(*REG_MSDC_STA & (1 << 4))); 
            *ptr_32 = *REG_MSDC_DAT;
            ptr_32++;
        }
    }
    return 1; // Sucesso
}

int emmc_escrever_blocos(uint32_t setor_inicial, uint32_t quantidade, const uint8_t* buffer_origem) {
    const uint32_t* ptr_32 = (const uint32_t*)buffer_origem;

    for (uint32_t b = 0; b < quantidade; b++) {
        // CMD24 (WRITE_BLOCK) no padrão MMC/SD
        *REG_MSDC_ARG = setor_inicial + b;
        *REG_MSDC_CMD = (24 | (1 << 7)); // Comando 24 com flag de escrita ativa

        // Despeja os 512 bytes do bloco no registrador de dados do eMMC
        for (int i = 0; i < (TAMANHO_BLOCO_EMMC / 4); i++) {
            // Espera o FIFO de escrita liberar espaço
            while(!(*REG_MSDC_STA & (1 << 5))); 
            *REG_MSDC_DAT = *ptr_32;
            ptr_32++;
        }
    }
    return 1; // Sucesso
}

// --- LOGICA DO SISTEMA DE SAVE ---

void save_carregar_do_disco(uint8_t* sram_emulada, uint32_t setor_save) {
    // O chip de SRAM clássico do GBA tem 32KB. 
    // 32KB / 512 bytes por bloco = 64 blocos no eMMC.
    uint32_t blocos_para_ler = 64; 
    
    emmc_ler_blocos(setor_save, blocos_para_ler, sram_emulada);
}

void save_sincronizar_para_disco(const uint8_t* sram_emulada, uint32_t setor_save) {
    uint32_t blocos_para_escrever = 64; 
    
    emmc_escrever_blocos(setor_save, blocos_para_escrever, sram_emulada);
}
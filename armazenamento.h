#ifndef ARMAZENAMENTO_H
#define ARMAZENAMENTO_H

#include <stdint.h>

#define TAMANHO_BLOCO_EMMC    512
#define ENDERECO_SAVE_SRAM    0x0E000000 // Endereço padrão da SRAM na arquitetura do GBA

// Inicializa o controlador eMMC (MSDC) do MediaTek MT6739
void armazenamento_init(void);

// Lê blocos de dados brutos diretamente do eMMC do celular
int emmc_ler_blocos(uint32_t setor_inicial, uint32_t quantidade, uint8_t* buffer_destino);

// Escreve blocos de dados brutos no eMMC do celular (usado para salvar o progresso)
int emmc_escrever_blocos(uint32_t setor_inicial, uint32_t quantidade, const uint8_t* buffer_origem);

// --- SISTEMA DE SAVE ---
// Carrega o progresso salvo do eMMC para a memória de save emulada do GBA
void save_carregar_do_disco(uint8_t* sram_emulada, uint32_t setor_save);

// Sincroniza a memória de save emulada de volta para o eMMC (pode ser chamado periodicamente)
void save_sincronizar_para_disco(const uint8_t* sram_emulada, uint32_t setor_save);

#endif // ARMAZENAMENTO_H
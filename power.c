#include "power.h"
#include "controles.h"
#include "armazenamento.h"
#include <stdint.h>

/* =========================================================================
   Módulo de Gerenciamento de Energia - GBA_OS (TCL L5 / 5033TP)
   Implementação: power.c
   ========================================================================= */

// Variáveis internas do estado de energia
static PowerState_t estado_atual = POWER_STATE_ATIVO;
static PowerState_t estado_anterior = POWER_STATE_ATIVO;
static uint32_t contador_ms = 0;           // Contador de milissegundos
static uint32_t tempo_inicio_pressionamento = 0;

// Configuração padrão
static PowerConfig_t config_padrao = {
    .tempo_confirmacao_ms = 3000,          // 3 segundos para confirmar desligamento
    .timeout_salvamento_ms = 5000,         // 5 segundos máximo para salvar estado
    .habilitado = 1                        // Habilitado por padrão
};

static PowerConfig_t config_atual = {
    .tempo_confirmacao_ms = 3000,
    .timeout_salvamento_ms = 5000,
    .habilitado = 1
};

/* =========================================================================
   FUNÇÕES PRIVADAS (INTERNAS)
   ========================================================================= */

/**
 * power_salvar_estado()
 * Salva o estado atual do sistema antes de desligar
 * - Salva registradores da CPU
 * - Salva estado da memória RAM crítica
 * - Salva dados do PPU
 */
static void power_salvar_estado(void) {
    // Marca que estamos no estado de salvamento
    estado_atual = POWER_STATE_SALVANDO;
    contador_ms = 0;
    
    // 1. Salva registradores críticos da CPU em um setor de backup na SD Card
    // cpu_salvar_contexto() seria implementado em cpu_arm.c
    // Para este exemplo, assumimos que existe
    // cpu_salvar_contexto(&backup_cpu);
    
    // 2. Salva blocos de RAM críticos (EWRAM, IWRAM)
    // armazenamento_salvar_ram_backup();
    
    // 3. Flush de qualquer dado pendente em cache de armazenamento
    // armazenamento_flush();
}

/**
 * power_desligar_hardware()
 * Realiza o desligamento físico do hardware
 * Comunica com o MediaTek para desativar reguladores de voltagem
 */
static void power_desligar_hardware(void) {
    // Endereço base do Power Management Unit (PMU) no MT6739
    // Varia conforme o SoC, mas geralmente está em 0x10001000 ou similar
    #define MTK_PMU_BASE 0x10001000
    #define MTK_POWEROFF_REG (MTK_PMU_BASE + 0x0008)
    
    // Escreve no registrador de desligamento
    // O valor específico depende do datasheet do MT6739
    volatile uint32_t* pmu_poweroff = (volatile uint32_t*)MTK_POWEROFF_REG;
    *pmu_poweroff = 0x00000001;  // Trigga o desligamento
    
    // Se o hardware não responder ao comando acima, entra em loop infinito
    // (o watchdog do kernel Linux subjacente eventualmente resetará a placa)
    while (1) {
        // Aguarda desligamento...
    }
}

/**
 * power_processar_botao_power()
 * Processa o pressionamento do botão POWER
 */
static void power_processar_botao_power(void) {
    uint8_t botao_power_pressionado = botao_pressionado(BOTAO_POWER);
    
    switch (estado_atual) {
        case POWER_STATE_ATIVO:
            // Se o botão POWER foi pressionado, começar a contar o tempo
            if (botao_power_pressionado) {
                estado_atual = POWER_STATE_PRESSIONANDO;
                tempo_inicio_pressionamento = contador_ms;
            }
            break;
            
        case POWER_STATE_PRESSIONANDO:
            // Verifica se o botão ainda está pressionado
            if (!botao_power_pressionado) {
                // Botão foi solto antes de 3 segundos - volta ao normal
                estado_atual = POWER_STATE_ATIVO;
            } else {
                // Verifica se já passaram 3 segundos
                uint32_t tempo_decorrido = contador_ms - tempo_inicio_pressionamento;
                if (tempo_decorrido >= config_atual.tempo_confirmacao_ms) {
                    // Tempo confirmado - inicia salvamento
                    estado_atual = POWER_STATE_SALVANDO;
                    contador_ms = 0;
                }
            }
            break;
            
        case POWER_STATE_SALVANDO:
            // Aguarda o salvamento ser concluído
            // Se ultrapassar o timeout, desliga mesmo assim
            if (contador_ms >= config_atual.timeout_salvamento_ms) {
                estado_atual = POWER_STATE_DESLIGANDO;
            }
            // TODO: Integrar com armazenamento_salvo() para saber quando terminou
            break;
            
        case POWER_STATE_DESLIGANDO:
            // Realiza desligamento efetivo do hardware
            power_desligar_hardware();
            break;
            
        default:
            break;
    }
}

/* =========================================================================
   FUNÇÕES PÚBLICAS
   ========================================================================= */

void power_init(void) {
    estado_atual = POWER_STATE_ATIVO;
    estado_anterior = POWER_STATE_ATIVO;
    contador_ms = 0;
    tempo_inicio_pressionamento = 0;
    
    // Inicializa o módulo de controles para ler o botão POWER
    controles_init();
    
    // Carrega configuração (se houver salva na NVRAM/arquivo de config)
    // power_carregar_config_de_arquivo();
}

void power_atualizar(void) {
    // Incrementa o contador interno
    // Assumindo que esta função é chamada a ~60 Hz (16.67ms por frame)
    // Para cada millisegundo, você pode chamar isto 60 vezes por segundo
    // Ou adaptar a taxa dependendo do seu loop principal
    contador_ms += 16;  // Aproximadamente 16ms por frame a 60 FPS
    
    // Atualiza o estado dos botões físicos
    controles_atualizar();
    
    // Processa lógica de desligamento
    power_processar_botao_power();
    
    // Salva o estado anterior para detecção de transição
    estado_anterior = estado_atual;
}

void power_solicitar_desligamento(void) {
    // Força o desligamento imediato sem confirmação
    // Útil para shutdown via comando de software
    if (config_atual.habilitado) {
        estado_atual = POWER_STATE_SALVANDO;
        contador_ms = 0;
    }
}

PowerState_t power_obter_estado(void) {
    return estado_atual;
}

void power_configurar(PowerConfig_t* config) {
    if (config != NULL) {
        config_atual = *config;
    }
}

PowerConfig_t power_obter_config(void) {
    return config_atual;
}

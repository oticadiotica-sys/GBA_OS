#ifndef POWER_H
#define POWER_H

#include <stdint.h>

/* =========================================================================
   Módulo de Gerenciamento de Energia - GBA_OS (TCL L5 / 5033TP)
   
   Responsável por:
   - Detectar pressionamento prolongado do botão POWER
   - Gerenciar sequência de desligamento do sistema
   - Salvar estado antes de desligar
   - Comunicação com hardware do MTK (MediaTek)
   ========================================================================= */

// Estados do processo de desligamento
typedef enum {
    POWER_STATE_ATIVO = 0,           // Sistema operacional normal
    POWER_STATE_PRESSIONANDO = 1,    // Botão POWER sendo pressionado
    POWER_STATE_CONFIRMACAO = 2,     // Esperando 3 segundos para confirmar
    POWER_STATE_SALVANDO = 3,        // Salvando estado antes de desligar
    POWER_STATE_DESLIGANDO = 4       // Processando desligamento final
} PowerState_t;

// Configuração de temporização para o desligamento
typedef struct {
    uint32_t tempo_confirmacao_ms;   // Tempo de pressionamento para confirmar (3000ms padrão)
    uint32_t timeout_salvamento_ms;  // Timeout para salvar estado (5000ms máximo)
    uint8_t habilitado;              // Flag para habilitar/desabilitar sistema de power
} PowerConfig_t;

/* =========================================================================
   FUNÇÕES PÚBLICAS
   ========================================================================= */

/**
 * power_init()
 * Inicializa o módulo de gerenciamento de energia
 * - Reseta os timers internos
 * - Configura os valores padrão
 */
void power_init(void);

/**
 * power_atualizar()
 * Deve ser chamada a cada frame (60Hz) no loop principal
 * Processa o estado do botão POWER e gerencia a sequência de desligamento
 */
void power_atualizar(void);

/**
 * power_solicitar_desligamento()
 * Função para forçar o desligamento imediato (sem confirmação)
 * Útil para shutdown via sistema de arquivos ou falhas críticas
 */
void power_solicitar_desligamento(void);

/**
 * power_obter_estado()
 * Retorna o estado atual do sistema de energia
 */
PowerState_t power_obter_estado(void);

/**
 * power_configurar(config)
 * Permite ajustar os parâmetros de desligamento
 * 
 * Exemplo:
 *   PowerConfig_t cfg;
 *   cfg.tempo_confirmacao_ms = 2000;  // 2 segundos em vez de 3
 *   cfg.habilitado = 1;
 *   power_configurar(&cfg);
 */
void power_configurar(PowerConfig_t* config);

/**
 * power_obter_config()
 * Retorna a configuração atual do módulo de poder
 */
PowerConfig_t power_obter_config(void);

#endif // POWER_H

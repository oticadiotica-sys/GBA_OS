#ifndef CONTROLES_H
#define CONTROLES_H

#include <stdint.h>

// Definição dos botões baseada no mapeamento típico do Keypad da MediaTek
typedef enum {
    BOTAO_POWER   = (1 << 0),
    BOTAO_VOL_UP  = (1 << 1),
    BOTAO_VOL_DOWN = (1 << 2),
    BOTAO_NENHUM   = 0
} Botao_t;

// Inicializa o periférico de Keypad/GPIOs dos botões
void controles_init(void);

// Atualiza o estado atual dos botões (deve ser chamado no loop principal ou via interrupção)
void controles_atualizar(void);

// Verifica se um botão específico está pressionado neste momento
uint8_t botao_pressionado(Botao_t botao);

// Verifica se um botão acabou de ser pressionado (borda de descida/subida)
uint8_t botao_pressionado_clique(Botao_t botao);

#endif // CONTROLES_H
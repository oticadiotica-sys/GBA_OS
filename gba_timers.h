#ifndef GBA_TIMERS_H
#define GBA_TIMERS_H

#include <stdint.h>

// Endereço base dos Registradores de Timer do GBA
#define REG_TM0CNT_L        0x04000100 // Timer 0 - Valor Atual / Inicial
#define REG_TM0CNT_H        0x04000102 // Timer 0 - Controle

typedef struct {
    uint16_t valor_inicial;
    uint32_t contador_atual;
    uint16_t controle;
    uint8_t ativo;
    uint16_t frequencia_shift;
} GBA_Timer_t;

// Inicializa os sub-sistemas de temporização
void gba_timers_init(void);

// Atualiza os timers com base nos ciclos de clock que a CPU executou
void gba_timers_atualizar(uint32_t ciclos_passados);

// Funções de leitura e escrita para o cpu_memoria.c
uint16_t gba_timers_ler16(uint32_t endereco);
void gba_timers_escrever16(uint32_t endereco, uint16_t valor);

#endif // GBA_TIMERS_H
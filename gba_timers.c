#include "gba_timers.h"
#include "som.h" // Para disparar o DMA de som quando o timer estourar

static GBA_Timer_t timers[4];

void gba_timers_init(void) {
    for (int i = 0; i < 4; i++) {
        timers[i].valor_inicial = 0;
        timers[i].contador_atual = 0;
        timers[i].controle = 0;
        timers[i].ativo = 0;
        timers[i].frequencia_shift = 0;
    }
}

void gba_timers_atualizar(uint32_t ciclos_passados) {
    for (int i = 0; i < 4; i++) {
        if (!timers[i].ativo) continue;

        // Avança o contador interno com base nos ciclos da CPU escalados pela frequência
        uint32_t ticks = ciclos_passados >> timers[i].frequencia_shift;
        timers[i].contador_atual += ticks;

        // No GBA, o timer é de 16 bits (estoura ao passar de 0xFFFF / 65535)
        if (timers[i].contador_atual > 0xFFFF) {
            timers[i].contador_atual = timers[i].valor_inicial + (timers[i].contador_atual - 0x10000);
            
            // Se for o Timer 0 ou Timer 1, eles costumam alimentar o Direct Sound do GBA
            if (i == 0 || i == 1) {
                // Aqui avisamos o driver de áudio para empurrar a próxima amostra PCM
                som_atualizar(); 
            }
        }
    }
}

uint16_t gba_timers_ler16(uint32_t endereco) {
    uint8_t timer_idx = (endereco - REG_TM0CNT_L) / 4;
    if (timer_idx > 3) return 0;

    if ((endereco & 2) == 0) {
        return (uint16_t)timers[timer_idx].contador_atual;
    } else {
        return timers[timer_idx].controle;
    }
}

void gba_timers_escrever16(uint32_t endereco, uint16_t valor) {
    uint8_t timer_idx = (endereco - REG_TM0CNT_L) / 4;
    if (timer_idx > 3) return;

    if ((endereco & 2) == 0) {
        timers[timer_idx].valor_inicial = valor;
    } else {
        timers[timer_idx].controle = valor;
        timers[timer_idx].ativo = (valor >> 7) & 1;

        // Define o divisor de frequência (Prescaler)
        uint8_t freq = valor & 0x3;
        switch (freq) {
            case 0: timers[timer_idx].frequencia_shift = 0; break; // Clock padrão / 1
            case 1: timers[timer_idx].frequencia_shift = 6; break; // Clock / 64
            case 2: timers[timer_idx].frequencia_shift = 8; break; // Clock / 256
            case 3: timers[timer_idx].frequencia_shift = 10; break; // Clock / 1024
        }

        if (timers[timer_idx].ativo) {
            timers[timer_idx].contador_atual = timers[timer_idx].valor_inicial;
        }
    }
}
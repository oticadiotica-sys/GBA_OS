#ifndef CPU_ARM_H
#define CPU_ARM_H

#include <stdint.h>

// Definição da estrutura interna da CPU do GBA
typedef struct {
    uint32_t registradores[16]; // R0-R12 (Gerais), R13 (SP), R14 (LR), R15 (PC)
    uint32_t cpsr;              // Current Program Status Register (Flags Z, N, C, V)
} GBA_CPU_t;

// Declaração global da instância da CPU manipulada pelo sistema
extern GBA_CPU_t cpu;

// Inicializa os registradores da CPU para o estado padrão de boot do GBA
void cpu_arm_reset(void);

// Decodifica e executa uma instrução ARM de 32 bits
void cpu_arm_executar_instrucao(uint32_t instrucao);

#endif // CPU_ARM_H
/* =========================================================================
   GBA_OS - Módulo: cpu_arm.h (Arquivo de Cabeçalho / Header)
   ========================================================================= */

#ifndef CPU_ARM_H
#define CPU_ARM_H

#include <stdint.h>

// Definição da estrutura compartilhada da CPU
typedef struct {
    uint32_t registradores[16]; 
    uint32_t cpsr;               
    uint32_t spsr;               
} GBA_CPU_t;

// Torna a variável global 'cpu' visível para o main.c e cpu_memoria.c
extern GBA_CPU_t cpu;

// Declaração das funções para os outros arquivos saberem que elas existem
void cpu_arm_reset(void);
void cpu_arm_executar_instrucao(uint32_t instrucao);

#endif

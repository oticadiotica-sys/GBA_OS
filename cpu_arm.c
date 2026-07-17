/* =========================================================================
   GBA_OS - Módulo: cpu_arm.c (Implementação do Interpretador)
   ========================================================================= */

#include "cpu_arm.h"
#include <stdint.h>

GBA_CPU_t cpu;

uint32_t gba_memoria_ler32(uint32_t endereco);

void cpu_arm_reset(void) {
    for (int i = 0; i < 16; i++) {
        cpu.registradores[i] = 0;
    }
    cpu.cpsr = 0x1F;
    cpu.registradores[15] = 0x08000000;
}

void cpu_arm_executar_instrucao(uint32_t instrucao) {
    // Processa a instrução ARM de 32-bit
    (void)instrucao; // Usar a instrução no futuro
    
    // Avança o ponteiro de execução em 4 bytes (32 bits)
    cpu.registradores[15] += 4;
}

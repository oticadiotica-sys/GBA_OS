/* =========================================================================
   GBA_OS - Módulo: cpu_arm.c (Implementação do Interpretador)
   ========================================================================= */

#include "cpu_arm.h" // Liga diretamente com o arquivo Header (.h)
#include <stdint.h>

// Alocação real da variável global na memória
GBA_CPU_t cpu;

// Função externa para leitura de memória
uint32_t gba_memoria_ler32(uint32_t endereco);

// Inicializa os registradores da CPU
void cpu_arm_reset(void) {
    for (int i = 0; i < 16; i++) {
        cpu.registradores[i] = 0;
    }
    cpu.cpsr = 0x1F;                     // Modo de operação padrão (System Mode)
    cpu.registradores[15] = 0x08000000;  // Aponta o PC para o início da ROM do jogo
}

// Executa o ciclo básico de instrução
void cpu_arm_executar_instrucao(void) {
    uint32_t pc_atual = cpu.registradores[15];
    uint32_t instrucao = gba_memoria_ler32(pc_atual);
    (void)instrucao;
    
    // Avança o ponteiro de execução em 4 bytes (32 bits)
    cpu.registradores[15] += 4;
}

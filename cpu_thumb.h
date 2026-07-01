#ifndef CPU_THUMB_H
#define CPU_THUMB_H

#include <stdint.h>

// Decodifica e executa uma instrução Thumb de 16 bits
void cpu_thumb_executar_instrucao(uint16_t instrucao);

#endif // CPU_THUMB_H
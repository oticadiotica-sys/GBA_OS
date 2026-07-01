#include "cpu_thumb.h"
#include "cpu_arm.h"     // Para acessar a estrutura global 'cpu'
#include "cpu_memoria.h" // Para leitura e escrita de dados/saves

void cpu_thumb_executar_instrucao(uint16_t instrucao) {
    
    // Format 3: Move/Compare/Add/Subtract Immediate
    // Muito utilizado para carregar valores pequenos em registradores
    if ((instrucao & 0xE000) == 0x2000) {
        uint8_t opcode = (instrucao >> 11) & 0x3;
        uint8_t rd = (instrucao >> 8) & 0x7;
        uint8_t imed = instrucao & 0xFF;

        switch (opcode) {
            case 0: // MOV Rd, #Offset8
                cpu.registradores[rd] = imed;
                break;
            case 1: // CMP Rd, #Offset8
                {
                    uint32_t resultado = cpu.registradores[rd] - imed;
                    cpu.cpsr &= 0x3FFFFFFF; // Limpa as flags N e Z
                    if (resultado == 0) cpu.cpsr |= (1 << 30); // Ativa flag Z
                    if (resultado & 0x80000000) cpu.cpsr |= (1 << 31); // Ativa flag N
                }
                break;
            case 2: // ADD Rd, #Offset8
                cpu.registradores[rd] += imed;
                break;
            case 3: // SUB Rd, #Offset8
                cpu.registradores[rd] -= imed;
                break;
        }
        
        cpu.registradores[15] += 2; // No modo Thumb, avançamos de 2 en 2 bytes
        return;
    }

    // Format 9: Load/Store with Immediate Offset
    // Intercepta transferências de dados cruciais (Memória/SRAM/Save)
    if ((instrucao & 0xE000) == 0x6000) {
        uint8_t bit_b = (instrucao >> 12) & 1; // 1 = Byte, 0 = Word
        uint8_t bit_l = (instrucao >> 11) & 1; // 1 = Load, 0 = Store
        uint8_t offset = (instrucao >> 6) & 0x1F;
        uint8_t rn = (instrucao >> 3) & 0x7;   // Registrador base
        uint8_t rd = instrucao & 0x7;          // Registrador origem/destino

        uint32_t endereco_alvo = cpu.registradores[rn];

        if (bit_b) {
            endereco_alvo += offset; // Byte offset direto
            if (bit_l) {
                cpu.registradores[rd] = gba_memoria_ler8(endereco_alvo);
            } else {
                gba_memoria_escrever8(endereco_alvo, (uint8_t)cpu.registradores[rd]);
            }
        } else {
            endereco_alvo += (offset << 2); // Word offset escalado por 4
            if (bit_l) {
                cpu.registradores[rd] = gba_memoria_ler8(endereco_alvo) |
                                       (gba_memoria_ler8(endereco_alvo + 1) << 8) |
                                       (gba_memoria_ler8(endereco_alvo + 2) << 16) |
                                       (gba_memoria_ler8(endereco_alvo + 3) << 24);
            } else {
                uint32_t valor = cpu.registradores[rd];
                gba_memoria_escrever8(endereco_alvo,     (uint8_t)(valor & 0xFF));
                gba_memoria_escrever8(endereco_alvo + 1, (uint8_t)((valor >> 8) & 0xFF));
                gba_memoria_escrever8(endereco_alvo + 2, (uint8_t)((valor >> 16) & 0xFF));
                gba_memoria_escrever8(endereco_alvo + 3, (uint8_t)((valor >> 24) & 0xFF));
            }
        }

        cpu.registradores[15] += 2;
        return;
    }

    // Format 18: Unconditional Branch (B)
    // Saltos e loops dentro do fluxo de execução do jogo
    if ((instrucao & 0xF800) == 0xE000) {
        int32_t offset = instrucao & 0x7FF;
        // Extensão de sinal para imediato de 11 bits
        if (offset & 0x400) {
            offset |= 0xFFFFF800;
        }
        offset <<= 1; // Alinhamento de 2 bytes para Thumb

        // Aplica o pulo considerando o offset do pipeline (+4 no modo Thumb)
        cpu.registradores[15] = (cpu.registradores[15] + 4) + offset;
        return;
    }

    // Fallback de segurança para avançar o PC caso encontre um opcode não implementado
    cpu.registradores[15] += 2;
}
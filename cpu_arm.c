/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: cpu_arm.c (Interpretador e Pipeline do Processador em Modo ARM)
   ========================================================================= */

#include <stdint.h>

// Definição da Estrutura de Registadores do ARM7TDMI
typedef struct {
    uint32_t registradores[16]; // R0 a R15 (R13=SP, R14=LR, R15=PC)
    uint32_t cpsr;               // Current Program Status Register
    uint32_t spsr;               // Saved Program Status Register
} GBA_CPU_t;

// Instanciação da variável global da CPU exigida pelo Linker
GBA_CPU_t cpu;

// Declaração das funções de barramento de memória (externas)
uint32_t gba_memoria_ler32(uint32_t endereco);
uint16_t gba_memoria_ler16(uint32_t endereco);
uint8_t gba_memoria_ler8(uint32_t endereco);
void gba_memoria_escrever32(uint32_t endereco, uint32_t valor);

// Máscaras das Flags de Condição do CPSR (Bits 31, 30, 29, 28)
#define FLAG_N (1U << 31) // Negativo
#define FLAG_Z (1U << 30) // Zero
#define FLAG_C (1U << 29) // Carry (Vai um)
#define FLAG_V (1U << 28) // Overflow (Estouro de sinal)

/* =========================================================================
   Função: cpu_arm_verificar_condicao
   Avalia os 4 bits superiores da instrução ARM para checar se a condição 
   de execução foi atendida com base nas flags atuais do CPSR.
   ========================================================================= */
static uint8_t cpu_arm_verificar_condicao(uint8_t condicao) {
    uint32_t cpsr = cpu.cpsr;
    
    switch (condicao) {
        case 0x0: return (cpsr & FLAG_Z) != 0;          // EQ (Equal / Igual)
        case 0x1: return (cpsr & FLAG_Z) == 0;          // NE (Not Equal / Diferente)
        case 0x2: return (cpsr & FLAG_C) != 0;          // CS/HS (Carry Set / Maior ou Igual)
        case 0x3: return (cpsr & FLAG_C) == 0;          // CC/LO (Carry Clear / Menor)
        case 0x4: return (cpsr & FLAG_N) != 0;          // MI (Minus / Negativo)
        case 0x5: return (cpsr & FLAG_N) == 0;          // PL (Plus / Positivo)
        case 0x6: return (cpsr & FLAG_V) != 0;          // VS (Overflow Set)
        case 0x7: return (cpsr & FLAG_V) == 0;          // VC (Overflow Clear)
        case 0x8: return (cpsr & FLAG_C) != 0 && (cpsr & FLAG_Z) == 0; // HI (Higher)
        case 0x9: return (cpsr & FLAG_C) == 0 || (cpsr & FLAG_Z) != 0; // LS (Lower or Same)
        case 0xA: return ((cpsr & FLAG_N) != 0) == ((cpsr & FLAG_V) != 0); // GE (Greater or Equal)
        case 0xB: return ((cpsr & FLAG_N) != 0) != ((cpsr & FLAG_V) != 0); // LT (Less Than)
        case 0xC: return (cpsr & FLAG_Z) == 0 && (((cpsr & FLAG_N) != 0) == ((cpsr & FLAG_V) != 0)); // GT
        case 0xD: return (cpsr & FLAG_Z) != 0 || (((cpsr & FLAG_N) != 0) != ((cpsr & FLAG_V) != 0)); // LE
        case 0xE: return 1;                             // AL (Always / Sempre Executa)
        default: return 0;
    }
}

/* =========================================================================
   Função: cpu_arm_executar_instrucao
   Busca, decodifica e processa uma instrução padrão de 32-bits ARM
   ========================================================================= */
void cpu_arm_executar_instrucao(void) {
    // 1. Fetch: Busca a instrução de 32 bits apontada pelo R15 (PC)
    uint32_t pc_atual = cpu.registradores[15];
    uint32_t instrucao = gba_memoria_ler32(pc_atual);
    
    // Extrai o código de condição (4 bits superiores)
    uint8_t condicao = (uint8_t)(instrucao >> 28);
    
    // Se a condição não for válida no estado atual do CPSR, pula a instrução
    if (!cpu_arm_verificar_condicao(condicao)) {
        cpu.registradores[15] += 4; // Avança o pipeline para a próxima instrução
        return;
    }
    
    // 2. Decode & Execute: Analisa o formato da instrução por máscaras de bits
    
    // Formato de Desvio: B (Branch) e BL (Branch with Link)
    if ((instrucao & 0x0E000000) == 0x0A000000) {
        uint8_t link_bit = (instrucao >> 24) & 1;
        int32_t offset = (instrucao & 0x00FFFFFF);
        
        // Estende o sinal do offset de 24 bits para 32 bits
        if (offset & 0x00800000) {
            offset |= 0xFF000000;
        }
        offset <<= 2; // Deslocamento de endereço alinhado a 4 bytes
        
        if (link_bit) {
            // Guarda o endereço de retorno no R14 (LR)
            cpu.registradores[14] = pc_atual + 4;
        }
        
        // Atualiza o PC aplicando o salto (+8 compensa o prefetch do pipeline físico do GBA)
        cpu.registradores[15] = pc_atual + 8 + offset;
        return;
    }
    
    // Formato de Transferência de Dados Únicos: LDR (Load) e STR (Store)
    if ((instrucao & 0x0C000000) == 0x04000000) {
        uint8_t rn = (instrucao >> 16) & 0x0F; // Registrador base
        uint8_t rd = (instrucao >> 12) & 0x0F; // Registrador destino/origem
        uint8_t load_bit = (instrucao >> 20) & 1;
        uint32_t offset_imediato = instrucao & 0xFFF;
        
        uint32_t endereco_alvo = cpu.registradores[rn] + offset_imediato;
        
        if (load_bit) {
            // Executa o LDR (Lê da memória para o registrador)
            cpu.registradores[rd] = gba_memoria_ler32(endereco_alvo);
        } else {
            // Executa o STR (Escreve do registrador para a memória)
            gba_memoria_escrever32(endereco_alvo, cpu.registradores[rd]);
        }
        
        cpu.registradores[15] += 4;
        return;
    }
    
    // Formato de Processamento de Dados (ADD, SUB, MOV, CMP, etc.)
    if ((instrucao & 0x0C000000) == 0x00000000) {
        uint8_t opcode = (instrucao >> 21) & 0x0F;
        uint8_t rn = (instrucao >> 16) & 0x0F;
        uint8_t rd = (instrucao >> 12) & 0x0F;
        uint32_t operando2 = instrucao & 0xFFF; // Simplificado (sem barrel shifter avançado)
        
        switch (opcode) {
            case 0xD: // MOV
                cpu.registradores[rd] = operando2;
                break;
            case 0x4: // ADD
                cpu.registradores[rd] = cpu.registradores[rn] + operando2;
                break;
            case 0x2: // SUB
                cpu.registradores[rd] = cpu.registradores[rn] - operando2;
                break;
            case 0xA: // CMP (Apenas atualiza flags)
                {
                    uint32_t resultado = cpu.registradores[rn] - operando2;
                    cpu.cpsr &= ~(FLAG_N | FLAG_Z); // Limpa flags antigas
                    if (resultado == 0) cpu.cpsr |= FLAG_Z;
                    if (resultado & (1U << 31)) cpu.cpsr |= FLAG_N;
                }
                break;
            default:
                break;
        }
        
        cpu.registradores[15] += 4;
        return;
    }

    // Fallback de segurança para instruções não implementadas
    cpu.registradores[15] += 4;
}

/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: main.c (Loop Principal e Sincronização do Sistema)
   ========================================================================= */

#include "cpu_arm.h"
#include "cpu_thumb.h"
#include "cpu_memoria.h"
#include "ppu.h"
#include "gba_timers.h"
#include "gba_sintetizador.h"
#include <stdint.h>

// Definição global das memórias do sistema usadas por todos os seus módulos
unsigned char rom_do_jogo[0x2000000];  // Espaço de 32MB para a ROM do jogo
unsigned char vram_do_gba[0x18000];    // 96KB de VRAM para os gráficos
unsigned char sram_do_gba[0x8000];     // 32KB de SRAM para os Saves

// Buffer de vídeo temporário para compor a imagem de saída para o ecrã do telemóvel (240x160 RGB)
static uint8_t buffer_tela_tcl[240 * 160 * 3];

// Função fictícia que representa a inicialização dos drivers de baixo nível do TCL L5
extern void tcl_l5_inicializar_hardware(void);
extern void tcl_l5_enviar_buffer_tela(uint8_t* buffer);

void main(void) {
    // 1. Inicializa o hardware físico do telemóvel (Drivers de ecrã, som e botões)
    tcl_l5_inicializar_hardware();

    // 2. Inicializa todos os subsistemas emulados que você criou
    cpu_arm_reset();          // Zera registradores e aponta para 0x08000000
    ppu_init();               // Zera os registradores de vídeo
    gba_timers_init();        // Configura os 4 temporizadores virtuais
    gba_sintetizador_init();  // Limpa o buffer FIFO de som

    // Loop principal de execução Bare-Metal
    while (1) {
        
        // O GBA renderiza 228 linhas por frame (160 ativas + 68 de V-Blank)
        for (int linha = 0; linha < 228; linha++) {
            
            // 1. Atualiza a linha atual e renderiza os gráficos se estiver no período ativo
            ppu_renderizar_linha(linha, buffer_tela_tcl);

            // 2. Executa um bloco de instruções da CPU equivalente ao tempo de uma linha de varrimento
            // Uma linha do GBA demora aproximadamente 1232 ciclos de clock da CPU
            uint32_t ciclos_da_linha = 0;
            while (ciclos_da_linha < 1232) {
                
                // Verifica o estado atual de execução (Modo 16-bit THUMB ou 32-bit ARM)
                // Usando a lógica do seu cpu_thumb.c/cpu_arm.c
                if (cpu.cpsr & (1 << 5)) { // Bit 5 do CPSR define o modo THUMB
                    // Lê a instrução de 16 bits combinando duas leituras de 8 bits do seu arquivo
                    uint16_t instr_thumb = gba_memoria_ler8(cpu.registradores[15]) |
                                          (gba_memoria_ler8(cpu.registradores[15] + 1) << 8);
                    
                    cpu_thumb_executar_instrucao(instr_thumb);
                    ciclos_da_linha += 2; // Estimativa média de ciclos Thumb
                } else {
                    // Lê a instrução de 32 bits usando as 4 leituras de 8 bits do seu arquivo
                    uint32_t instr_arm = gba_memoria_ler8(cpu.registradores[15]) |
                                        (gba_memoria_ler8(cpu.registradores[15] + 1) << 8) |
                                        (gba_memoria_ler8(cpu.registradores[15] + 2) << 16) |
                                        (gba_memoria_ler8(cpu.registradores[15] + 3) << 24);
                    
                    cpu_arm_executar_instrucao(instr_arm);
                    ciclos_da_linha += 4; // Estimativa média de ciclos ARM
                }

                // 3. Atualiza os timers com os ciclos passados para manter o som e os tempos síncronos
                gba_timers_atualizar(ciclos_da_linha);
                
                // 4. Processa o fluxo de som empurrado pelos estouros dos timers
                gba_sintetizador_processar();
            }
        }

        // Fim do frame: Envia a imagem final renderizada para o painel LCD físico do TCL L5
        tcl_l5_enviar_buffer_tela(buffer_tela_tcl);
    }
}
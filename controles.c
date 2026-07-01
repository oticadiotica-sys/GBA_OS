#include "controles.h"

// Endereço base hipotético do Keypad Controller no MT6739 (Mapeado em I/O)
// Nota: Verifique o mapa de memória exato do MT6739 no código do LK (Little Kernel) ou Preloader
#define MT6739_KP_BASE        0x10010000 
#define KP_MEM_DATA          (MT6739_KP_BASE + 0x0004) // Registrador de dados do teclado

static uint32_t estado_atual = 0;
static uint32_t estado_anterior = 0;

void controles_init(void) {
    // 1. Configurar o hardware do Keypad se necessário
    // Em muitos SoCs MediaTek, o Preloader já deixa isso ativo,
    // mas você pode precisar habilitar o clock do periférico no subsistema correspondente.
    
    estado_atual = 0;
    estado_anterior = 0;
}

void controles_atualizar(void) {
    estado_anterior = estado_atual;

    // Leitura direta do registrador de hardware
    // Volatile garante que o compilador não otimize a leitura da memória de I/O
    uint32_t leitura_hardware = *(volatile uint32_t*)KP_MEM_DATA;

    // Inverte a lógica se o hardware for "Active Low" (0 quando pressionado)
    // Geralmente, chaves físicas fecham curto com o GND, resultando em 0.
    uint32_t chaves_ativas = ~leitura_hardware;

    uint32_t novo_estado = 0;

    // Mapeia os bits do hardware para a nossa enumeração do GBA_OS
    // (Ajuste os bits 0, 1, 2 conforme o mapeamento real do hardware do TCL L5)
    if (chaves_ativas & (1 << 0)) novo_estado |= BOTAO_POWER;
    if (chaves_ativas & (1 << 1)) novo_estado |= BOTAO_VOL_UP;
    if (chaves_ativas & (1 << 2)) novo_estado |= BOTAO_VOL_DOWN;

    estado_atual = novo_estado;
}

uint8_t botao_pressionado(Botao_t botao) {
    return (estado_atual & botao) ? 1 : 0;
}

uint8_t botao_pressionado_clique(Botao_t botao) {
    // Retorna verdadeiro apenas se o botão está pressionado AGORA, mas NÃO estava antes
    return ((estado_atual & botao) && !(estado_anterior & botao)) ? 1 : 0;
}
#include "touch.h"
#include "ppu.h"          // Para saber as dimensões da tela
#include "gba_controles.h" // Para enviar os comandos de botões simulados
#include <stdint.h>

// Definição das regiões dos botões virtuais na tela (coordenadas X, Y)
typedef struct {
    int x_min, y_min;
    int x_max, y_max;
    uint16_t gba_button_mask; // Máscara do botão correspondente (A, B, START, etc)
    const char* label;
} BotaoVirtual_t;

// Mapeamento dos botões (configurado para tela de 240x160)
static BotaoVirtual_t botoes_virtuais[] = {
    {10,  100, 40,  130, 0x0004, "LEFT"},   // D-Pad Esquerda
    {50,  100, 80,  130, 0x0001, "RIGHT"},  // D-Pad Direita
    {180, 110, 205, 135, 0x0002, "B"},      // Botão B
    {210, 110, 235, 135, 0x0001, "A"},      // Botão A
    {90,  145, 120, 155, 0x0040, "SELECT"}, // SELECT
    {130, 145, 160, 155, 0x0008, "START"}   // START
};

#define TOTAL_BOTOES (sizeof(botoes_virtuais) / sizeof(BotaoVirtual_t))

// Variáveis globais para armazenar o estado do toque
static int toque_ativo = 0;
static int toque_x = 0;
static int toque_y = 0;

// ============================================================================
// INICIALIZAR TOUCHSCREEN
// ============================================================================
void touch_inicializar(void) {
    // Configurar o barramento I2C para comunicação com o chip FT6206 do MT6739
    // Em um sistema bare-metal real, isso incluiria:
    // - Habilitar clock do I2C
    // - Configurar pinos de SDA e SCL
    // - Definir velocidade da comunicação (400kHz típico)
    
    // Para este emulador, apenas resetamos o estado
    toque_ativo = 0;
    toque_x = 0;
    toque_y = 0;
}

// ============================================================================
// LER DADOS DO TOUCHSCREEN
// ============================================================================
void touch_ler_dados(int *out_x, int *out_y, int *detectado) {
    // Em bare-metal real, leríamos os registradores do barramento I2C:
    // uint8_t dados[4];
    // i2c_ler_registradores(TOUCH_I2C_ADDR, REG_TOUCH_STATUS, dados, 4);
    // 
    // Extrairíamos:
    // - dados[0]: número de pontos tocados
    // - dados[1-2]: coordenada X (12 bits)
    // - dados[3-4]: coordenada Y (12 bits)
    
    // Simulação: retorna os dados armazenados
    *detectado = toque_ativo;
    *out_x = toque_x;
    *out_y = toque_y;
}

// ============================================================================
// ATUALIZAR CONTROLES BASEADO NO TOQUE
// ============================================================================
uint16_t touch_atualizar_controles(void) {
    int x = 0, y = 0, tocado = 0;
    uint16_t mascara_retorno = 0xFFFF; // No GBA, 1 = solto, 0 = pressionado

    touch_ler_dados(&x, &y, &tocado);

    if (tocado) {
        for (int i = 0; i < TOTAL_BOTOES; i++) {
            // Verifica se o ponto tocado está dentro do quadrado do botão
            if (x >= botoes_virtuais[i].x_min && x <= botoes_virtuais[i].x_max &&
                y >= botoes_virtuais[i].y_min && y <= botoes_virtuais[i].y_max) {
                
                // Limpa o bit correspondente para simular o botão pressionado
                mascara_retorno &= ~(botoes_virtuais[i].gba_button_mask);
            }
        }
    }

    return mascara_retorno;
}

// ============================================================================
// DESENHAR INTERFACE DOS BOTÕES VIRTUAIS NA TELA
// ============================================================================
void touch_desenhar_interface(void) {
    // Desenha um layout de botões virtuais de controle na tela
    // Cores: BGR 5-5-5 (formato do GBA em Modo 3)
    
    // Cores pré-definidas em formato XBGR1555
    #define COR_BRANCO  0x7FFF
    #define COR_PRETO   0x0000
    #define COR_VERMELHO 0x001F
    #define COR_VERDE   0x03E0
    #define COR_AZUL    0x7C00
    #define COR_CINZA   0x42A4
    
    // Desenhar cada botão
    for (int i = 0; i < TOTAL_BOTOES; i++) {
        // Selecionar cor baseada no tipo de botão
        uint16_t cor = COR_CINZA;
        if (botoes_virtuais[i].label[0] == 'A') cor = COR_VERMELHO;
        else if (botoes_virtuais[i].label[0] == 'B') cor = COR_VERDE;
        else if (botoes_virtuais[i].label[0] == 'S' && botoes_virtuais[i].label[1] == 'T') cor = COR_AZUL;
        
        // Desenhar retângulo do botão (versão simplificada)
        // Iteração pelos pixels do botão (seria otimizado em produção)
        for (int y = botoes_virtuais[i].y_min; y < botoes_virtuais[i].y_max; y++) {
            for (int x = botoes_virtuais[i].x_min; x < botoes_virtuais[i].x_max; x++) {
                // Escrever pixel (seria implementado com a função de PPU)
                // ppu_escrever_pixel(x, y, cor);
            }
        }
    }
}

// ============================================================================
// SIMULAR TOQUE NA TELA (Para testes)
// ============================================================================
void touch_simular_toque(int x, int y) {
    toque_x = x;
    toque_y = y;
    toque_ativo = 1;
}

// ============================================================================
// LIBERAR TOQUE NA TELA
// ============================================================================
void touch_liberar_toque(void) {
    toque_ativo = 0;
    toque_x = 0;
    toque_y = 0;
}

// ============================================================================
// VERIFICAR SE TOQUE ESTÁ ATIVO
// ============================================================================
int touch_esta_ativo(void) {
    return toque_ativo;
}

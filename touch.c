#include "touch.h"
#include "ppu.h"          // Para saber as dimensões da tela
#include "gba_controles.h" // Para enviar os comandos de botões simulados

// Definição das regiões dos botões virtuais na tela (coordenadas X, Y)
typedef struct {
    int x_min, y_min;
    int x_max, y_max;
    int gba_button_mask; // Máscara do botão correspondente (A, B, START, etc)
} BotaoVirtual_t;

// Mapeamento dos botões (exemplo configurado para tela de 240x160)
static BotaoVirtual_t botoes_virtuais[] = {
    {10,  100, 40,  130, KEY_LEFT},   // D-Pad Esquerda
    {50,  100, 80,  130, KEY_RIGHT},  // D-Pad Direita
    {180, 110, 205, 135, KEY_B},      // Botão B
    {210, 110, 235, 135, KEY_A},      // Botão A
    {90,  145, 120, 155, KEY_SELECT}, // SELECT
    {130, 145, 160, 155, KEY_START}   // START
};

#define TOTAL_BOTOES (sizeof(botoes_virtuais) / sizeof(BotaoVirtual_t))

// Mock para leitura do registrador I2C do driver de toque (ex: FT6206)
void touch_ler_dados(int *out_x, int *out_y, int *detectado) {
    // Em bare-metal real, leríamos os registradores do barramento I2C:
    // uint8_t dados[4];
    // i2c_ler_registradores(TOUCH_I2C_ADDR, REG_TOUCH_DATA, dados, 4);
    
    // Se o registrador indicar toque ativo, popula as variáveis
    *detectado = 0; 
    *out_x = 0;
    *out_y = 0;
}

// Processa o toque e traduz para as teclas do Game Boy Advance
uint16_t touch_atualizar_controles() {
    int x = 0, y = 0, tocado = 0;
    uint16_t mascara_retorno = 0xFFFF; // No GBA, 1 significa solto, 0 significa pressionado

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

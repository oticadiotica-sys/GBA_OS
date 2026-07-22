#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>

// Endereço base fictício do barramento I2C do chip touchscreen no MT6739
#define TOUCH_I2C_ADDR       0x38
#define REG_TOUCH_STATUS     0x02
#define REG_TOUCH_X_MSB      0x03
#define REG_TOUCH_Y_MSB      0x05

// Estrutura de dados para os botões da interface virtual
typedef struct {
    int x_min, y_min;
    int x_max, y_max;
    uint16_t gba_button_mask;
    const char* label; // Para rotinas de renderização gráfica
} BotaoVirtual_t;

// Funções de Inicialização e Controle
void touch_inicializar(void);
void touch_ler_dados(int *out_x, int *out_y, int *detectado);
uint16_t touch_atualizar_controles(void);
void touch_desenhar_interface(void);

#endif // TOUCH_H

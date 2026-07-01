#ifndef GBA_CONTROLES_H
#define GBA_CONTROLES_H

#include <stdint.h>

// Endereço do registrador de entrada de botões do GBA
#define REG_KEYINPUT_ADDR   0x04000130

// Máscaras de bits para os botões do GBA (Padrão: 0 = Ativo / Pressionado)
#define GBA_BUTTON_A        (1 << 0)
#define GBA_BUTTON_B        (1 << 1)
#define GBA_BUTTON_SELECT   (1 << 2)
#define GBA_BUTTON_START    (1 << 3)
#define GBA_BUTTON_RIGHT    (1 << 4)
#define GBA_BUTTON_LEFT     (1 << 5)
#define GBA_BUTTON_UP       (1 << 6)
#define GBA_BUTTON_DOWN     (1 << 7)
#define GBA_BUTTON_R        (1 << 8)
#define GBA_BUTTON_L        (1 << 9)

// Retorna o valor de 16 bits traduzido para o formato do GBA
uint16_t gba_controles_obter_estado(void);

#endif // GBA_CONTROLES_H
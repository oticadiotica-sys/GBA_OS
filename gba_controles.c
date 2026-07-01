#include "gba_controles.h"
#include "controles.h" // Seu driver físico do celular

uint16_t gba_controles_obter_estado(void) {
    // No GBA, se nenhum botão estiver pressionado, todos os bits ficam em 1 (0xFFFF)
    uint16_t estado_gba = 0xFFFF; 

    // Atualiza a leitura física do hardware do TCL L5
    controles_atualizar();

    // Mapeamento lógico de teste:
    // Se o usuário apertar Volume Mais, simulamos o botão "A" do GBA
    if (botao_pressionado(BOTAO_VOL_UP)) {
        estado_gba &= ~GBA_BUTTON_A; // Zera o bit (sinaliza pressionado)
    }

    // Se o usuário apertar Volume Menos, simulamos o botão "B" do GBA
    if (botao_pressionado(BOTAO_VOL_DOWN)) {
        estado_gba &= ~GBA_BUTTON_B;
    }

    // Se o usuário apertar o Botão Power, simulamos o "START" do GBA
    if (botao_pressionado(BOTAO_POWER)) {
        estado_gba &= ~GBA_BUTTON_START;
    }

    return estado_gba;
}
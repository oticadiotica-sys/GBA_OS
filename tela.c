/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5
   Arquivo de Gráficos: tela.c (Gerenciador da Tela)
   ========================================================================= */

#include "tela.h" // Arquivo de configuração que conecta a tela ao sistema

// Endereço físico real do chip controlador de vídeo do TCL L5 (MediaTek)
// (Usado pelo sistema para enviar os pixels coloridos para a tela acender)
#define REG_DISPLAY_CONTROL_TCL   ((volatile unsigned int*)0x40001000)
#define REG_DISPLAY_BUFFER_TCL    ((volatile unsigned int*)0x40002000)

// O GBA original tem uma tela pequena de 240x160 pixels.
// O seu TCL L5 tem uma tela muito maior de 480x960 pixels.
#define GBA_LARGURA  240
#define GBA_ALTURA   160
#define TCL_LARGURA  480
#define TCL_ALTURA   960

// 1. INICIALIZAR A TELA DO CELULAR
void inicializar_tela_tcl() {
    // Envia um comando em código binário para ligar o painel LCD do celular
    // e configurar para o modo de cor de 32 bits (True Color)
    *REG_DISPLAY_CONTROL_TCL = 0x00000001; 
}

// 2. LIMPAR A TELA (Deixa tudo preto antes do jogo começar)
void limpar_tela_tcl() {
    // Percorre cada pontinho (pixel) da tela do celular colocando a cor preta
    for (int i = 0; i < TCL_LARGURA * TCL_ALTURA; i++) {
        REG_DISPLAY_BUFFER_TCL[i] = 0x00000000; // Código para a cor preta
    }
}

// 3. O "CORAÇÃO" DA TELA: DESENHAR O JOGO (Esticar a imagem)
void desenhar_graficos_gba(unsigned char* vram_do_gba) {
    // Como a tela do celular é maior que a do Game Boy, se desenharmos normal,
    // o jogo vai ficar minúsculo no canto da tela.
    // Este código faz um efeito chamado "Upscaling" (Esticar a imagem) para preencher a tela!
    
    for (int y = 0; y < GBA_ALTURA; y++) {
        for (int x = 0; x < GBA_LARGURA; x++) {
            
            // Pega a cor do pixel atual dentro da memória de vídeo (VRAM) do jogo de GBA
            unsigned int pixel_gba = ((unsigned int*)vram_do_gba)[y * GBA_LARGURA + x];
            
            // Multiplica o tamanho por 2 para o jogo caber direitinho na tela do TCL
            int tcl_x1 = x * 2;
            int tcl_y1 = y * 2;
            int tcl_x2 = (x * 2) + 1;
            int tcl_y2 = (y * 2) + 1;
            
            // Desenha o pixel esticado em um bloco de 4 pixels na tela do celular
            REG_DISPLAY_BUFFER_TCL[tcl_y1 * TCL_LARGURA + tcl_x1] = pixel_gba;
            REG_DISPLAY_BUFFER_TCL[tcl_y1 * TCL_LARGURA + tcl_x2] = pixel_gba;
            REG_DISPLAY_BUFFER_TCL[tcl_y2 * TCL_LARGURA + tcl_x1] = pixel_gba;
            REG_DISPLAY_BUFFER_TCL[tcl_y2 * TCL_LARGURA + tcl_x2] = pixel_gba;
        }
    }
}
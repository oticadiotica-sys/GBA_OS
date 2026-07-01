/* =========================================================================
   GBA_OS - Sistema Operacional em Metal Puro para TCL L5 (5033TP)
   Módulo: linker.ld (Script de Linkagem e Mapeamento de Memória Física)
   ========================================================================= */

ENTRY(main)

MEMORY
/* Definição das regiões de memória física do hardware do TCL L5 */
{
    ROM_FREGIAO (rx)  : ORIGIN = 0x00000000, LENGTH = 16M   /* Espaço reservado para o Bootloader/OS */
    RAM_FREGIAO (rwx) : ORIGIN = 0x10000000, LENGTH = 32M   /* Onde alocamos as memórias do GBA e o código ativo */
}

SECTIONS
{
    /* 1. Seção de código do sistema operacional (Vai para a memória de leitura) */
    .text :
    {
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    } > ROM_FREGIAO

    /* 2. Seção de dados inicializados (Variáveis globais pré-configuradas) */
    .data :
    {
        _data_inicio = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _data_fim = .;
    } > RAM_FREGIAO AT > ROM_FREGIAO

    /* 3. Seção de dados não-inicializados (Onde suas matrizes globais rom_do_jogo, vram_do_gba e sram_do_gba vão morar) */
    .bss :
    {
        _bss_inicio = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        _bss_fim = .;
    } > RAM_FREGIAO

    /* Definindo a base da pilha (Stack) apontando para o topo da RAM física do celular */
    _stack_top = ORIGIN(RAM_FREGIAO) + LENGTH(RAM_FREGIAO);
}
# Modified by zhiyb (Yubo Zhi)

TRG	= UTRX
SRC	= rfm12.c adc.c timer.c uart0.c testaudioTRX_FINAL.c
INCDIRS	+= . ../inc

PRGER	= usbasp
MCU_TARGET	= atmega644p
MCU_FREQ	= 12000000

EFUSE	= 0xFF
HFUSE	= 0x9C
LFUSE	= 0xE7

include ../Makefile_AVR.defs

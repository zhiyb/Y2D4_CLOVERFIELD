# Modified by zhiyb (Yubo Zhi)

PRG	= UTRX
SRC	= rfm12.c adc.c uart0.c testaudioTRX_FINAL.c
LIB	= 
VERSION	= 0.0.1

PRGER	= usbasp
MCU_TARGET	= atmega644p
OPTIMIZE	= -Os

DEFS	+= -DF_CPU=12000000
LIBS	+=
INCS	+= -I. -I../inc

EFUSE	= 0xFF
HFUSE	= 0x9C
LFUSE	= 0xE7

LOG	= ~/.make_avr.log

include ../Makefile.in

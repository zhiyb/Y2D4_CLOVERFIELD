# Modified by zhiyb (Yubo Zhi)

PRG	= IlMatto1
SRC	= main.cpp uart0.c menu.cpp menufunc.cpp sketch.cpp
LIB	= list rTouch common tft-cpp
LIBDIR	= ../lib
VERSION	= 0.0.1

PRGER	= usbasp
MCU_TARGET	= atmega644p
OPTIMIZE	= -Os

DEFS	+= -DF_CPU=12000000
DEFS	+= -DTFT_VERTICALSCROLLING
DEFS	+= -DRTOUCH_SWAPXY
LIBS	+= -lm
INCS	+= -I../inc

EFUSE	= 0xFF
HFUSE	= 0x9C
LFUSE	= 0xE7

LOG	= ~/.make_avr.log

include ../Makefile.in

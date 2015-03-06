# Modified by zhiyb (Yubo Zhi)

PRG	= IlMatto1
SRC	= main.cpp pool.cpp
SRC	+= hw/uart0.c hw/tick.c
SRC	+= menu/menu.cpp menu/menufunc.cpp
SRC	+= element/sketch.cpp element/indicator.cpp
LIB	= list rTouch common tft-cpp
LIBDIR	= ../lib
VERSION	= 0.0.1

PRGER	= usbasp
MCU_TARGET	= atmega644p
OPTIMIZE	= -Os

DEFS	+= -DF_CPU=12000000
DEFS	+= -DTFT_VERTICALSCROLLING
DEFS	+= -DRTOUCH_SWAPXY
DEFS	+= -DLIST_TOP_RESERVED=8
LIBS	+= -lm
INCS	+= -I../inc -I. -I./hw -I./menu -I./element

EFUSE	= 0xFF
HFUSE	= 0x9C
LFUSE	= 0xE7

LOG	= ~/.make_avr.log

include ../Makefile.in

# Modified by zhiyb (Yubo Zhi)

TRG	= IlMatto1
SRC	= main.cpp pool.cpp status.cpp operator.cpp
SUBDIRS	= hw menu element capture
INCDIRS	= . ../inc
LIB	= list rTouch common tft-cpp
LIBDIR	= ../lib

PRGER	= usbasp
MCU_TARGET	= atmega644p
MCU_FREQ	= 12000000
OPTIMIZE	= -Os

#DEFS	+= -DPIN_DISPLAY
DEFS	+= -DDEBUG

DEFS	+= -DTFT_VERTICALSCROLLING
DEFS	+= -DRTOUCH_SWAPXY
DEFS	+= -DRTOUCH_AVERAGER=32
DEFS	+= -DRTOUCH_MOVETH=8
DEFS	+= -DLIST_TOP_RESERVED=8
LIBS	+= -lm

EFUSE	= 0xFF
HFUSE	= 0x9C
LFUSE	= 0xE7

include ../Makefile_AVR.defs

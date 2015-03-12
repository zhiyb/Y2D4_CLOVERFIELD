# Modified by zhiyb (Yubo Zhi)

PRG	:= IlMatto1
SRC	:= main.cpp pool.cpp status.cpp operator.cpp
SUBDIRS	:= hw menu element capture
LIB	:= list rTouch common tft-cpp
LIBDIR	:= ../lib
VERSION	:= 0.0.1

PRGER	= usbasp
MCU_TARGET	= atmega644p
OPTIMIZE	= -Os

#DEFS	+= -DPIN_DISPLAY
DEFS	+= -DDEBUG

DEFS	+= -DF_CPU=12000000
DEFS	+= -DTFT_VERTICALSCROLLING
DEFS	+= -DRTOUCH_SWAPXY
DEFS	+= -DRTOUCH_AVERAGER=32
DEFS	+= -DRTOUCH_MOVETH=8
DEFS	+= -DLIST_TOP_RESERVED=8
LIBS	+= -lm
INCS	+= -I../inc -I.

EFUSE	= 0xFF
HFUSE	= 0x9C
LFUSE	= 0xE7

LOG	= ~/.make_avr.log

include $(SUBDIRS:%=%/Makefile)
include ../Makefile.in

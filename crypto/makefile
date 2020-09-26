##===================================================================##
## File Name: makefile
## Create Author: Tom Hui
## Create Date: Tue Sep 12 1359 2015
## Description:
##		
##===================================================================##

include ../inc.mk


TARGET = ${LIB_PATH}/libhuicrypto.a

SRC = $(wildcard *.cpp)

OBJS = ${patsubst %.cpp, %.o, ${SRC}}

CXX_DEFINES += -I${HUI_DEPS_PATH}/openssl/include



ifeq "$(OS_TYPE)" "Darwin"
LINK_CC_LIBS += -liconv
endif

all: def

def: ${TARGET}


include ../route.mk

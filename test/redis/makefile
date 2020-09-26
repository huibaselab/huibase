##===================================================================##
## File Name: makefile
## Create Author: Tom Hui
## Create Date: Tue Sep 12 1359 2015
## Description:
##		
##===================================================================##

include ../../inc.mk


TARGET = test.out

SRC = $(wildcard *.cpp)

OBJS = ${patsubst %.cpp, %.o, ${SRC}}

CXX_INCLUDE += -I../../src -I../../nosql -I../../deps/libmemcached -I../../deps/redisclient
LINK_CXX_LIBS += -L../../lib ../../lib/libnosql.a ../../lib/libhuibase.a ../../deps/libmemcached/libmemcached/.libs/libmemcached.a -L../../deps/redisclient -lredisclient -lboost_system

ifeq "$(OS_TYPE)" "Darwin"
LINK_CC_LIBS += -liconv
endif

TTYPE = EFILE

all: def

def: ${TARGET}


include ../../route.mk

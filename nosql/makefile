##===================================================================##
## File Name: makefile
## Create Author: Tom Hui
## Create Date: Sun Mar 11 1622 2017
## Description:
## apt-get install msqlpp;
## apt-get install libmysqlclient-dev;
##===================================================================##


include ../inc.mk

TARGET = ${LIB_PATH}/libnosql.a

SRC = $(wildcard *.cpp)

OBJS = ${patsubst %.cpp, %.o, ${SRC}}

CXX_INCLUDE += -I${HUI_DEPS_PATH}/hiredis

CXX_CFLAGS = -fPIC -O2 -g -Wall -std=c++11

all: def

def: ${TARGET}


include ../route.mk

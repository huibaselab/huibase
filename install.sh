###
 # @Author: Tom Hui
 # @Date: 2019-12-16 20:41:56
 # @Description: 
 ###
#!/bin/bash

apt-get install perl -y
apt-get install -y libmysqlclient-dev

NOW_DIR=`pwd`
PRONAME=huibase
ROOT_DIR=~/huilib
DEP_DIR=${ROOT_DIR}/deps/
DEP_OPENSSL=${DEP_DIR}openssl
DEP_CURL=${DEP_DIR}curl
DEP_REDIS=${DEP_DIR}hiredis

function mk_if_not()
{
	dd=$1
	if [ ! -d ${dd} ]
	then 
		mkdir -p ${dd}
	fi
}

mk_if_not ${ROOT_DIR}

mk_if_not ${DEP_DIR}

mk_if_not ${ROOT_DIR}/${PRONAME}


if [ ! -d ${DEP_OPENSSL} ]
then 
    cp ${NOW_DIR}/deps/openssl.tar.gz ${DEP_DIR}/
	cd ${DEP_DIR}
	tar xvfz openssl.tar.gz
	rm -f openssl.tar.gz
	cd openssl
	export CFLAGS=-fPIC
	./config shared
	make -j8
	cd ${NOW_DIR}
fi

if [ ! -d ${DEP_CURL} ]
then 
    cp ${NOW_DIR}/deps/curl.tar.gz ${DEP_DIR}/
	cd ${DEP_DIR}
	tar xvfz curl.tar.gz
	rm -f curl.tar.gz
	cd curl
	export LD_LIBRARY_PATH=/usr/local/ssl/lib
	./configure --with-ssl=/usr/local/ssl --without-libssh2
	make -j8
	cd ${NOW_DIR}
fi


if [ ! -d ${DEP_REDIS} ]
then 
    cp ${NOW_DIR}/deps/hiredis.tar.gz ${DEP_DIR}/
	cd ${DEP_DIR}
	tar xvfz hiredis.tar.gz
	rm -f hiredis.tar.gz
	cd hiredis
	make -j8
	cd ${NOW_DIR}
fi


cd ${NOW_DIR}
make -j8
make install
cd ${NOW_DIR}
rm -fr *


exit 0
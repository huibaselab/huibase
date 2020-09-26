
ROOTPATH=/hui
PRONAME=huibase
LIB_PATH=${ROOTPATH}/${PRONAME}/lib
INCLUDE_PATH=${ROOTPATH}/${PRONAME}/include
HUI_ITEMS = crypto db http nosql
DIRS = src ${HUI_ITEMS}

all:
	@for dir in ${DIRS}; do make -C $$dir -j8; echo ; done


clean:
	@for dir in ${DIRS}; do make -C $$dir clean; echo ; done


install:
	mkdir -p ${LIB_PATH}
	cd lib; cp *.a ${LIB_PATH}/;
	mkdir -p ${INCLUDE_PATH}/;
	@for dir in ${HUI_ITEMS}; do mkdir -p ${INCLUDE_PATH}/$$dir; done
	cp src/*.h ${INCLUDE_PATH}/;
	cp src/*.hpp ${INCLUDE_PATH}/;
	@for dir in ${HUI_ITEMS}; do cp $$dir/*.h ${INCLUDE_PATH}/$$dir/; done


type_test:
	@echo ${OS_NAME};



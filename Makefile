BUILD_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib
EXAMPLE_DIR = examples

DEMO_NAME = algorithm_demo

all: ${BUILD_DIR}/algorithm_demo

${BUILD_DIR}/%: ${EXAMPLE_DIR}/%.c ${LIB_DIR}/*.c
	$(CC) $^ -o $@ -I ${INCLUDE_DIR} 

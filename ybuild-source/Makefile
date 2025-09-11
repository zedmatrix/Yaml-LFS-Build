CC := gcc
CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2

LDFLAGS_STATIC := -static -lcurl -lidn2 -lpsl -lunistring -lzstd -lz -lssl -lcrypto -lyaml-cpp
LDFLAGS_SHARED := -lcurl -lyaml-cpp -lssl -lcrypto

SOURCE := main.cpp
OBJ := main.o

STATIC_EXEC := Ybuild
SHARED_EXEC := yaml-build
STATIC := -static -lcurl -lidn2 -lpsl -lunistring -lzstd -lz -lssl -lcrypto -lyaml-cpp

${OBJ}: ${SOURCE}
	${CXX} ${CXXFLAGS} -c ${SOURCE} -o ${OBJ}

${STATIC_EXEC}: ${OBJ}
	${CXX} ${CXXFLAGS} ${OBJ} ${LDFLAGS_STATIC} -o ${STATIC_EXEC}

${SHARED_EXEC}: ${OBJ}
	${CXX} ${CXXFLAGS} ${OBJ} ${LDFLAGS_SHARED} -o ${SHARED_EXEC}

yaml: ${SHARED_EXEC}

ybuild: ${STATIC_EXEC}

clean:
	rm -f ${OBJ} ${STATIC_EXEC} ${SHARED_EXEC}

.PHONY: clean yaml ybuild

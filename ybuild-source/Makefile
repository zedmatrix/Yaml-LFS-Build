CC := gcc
CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2
LDFLAGS := -lcurl -lyaml-cpp #-lssl -lcrypto

SOURCE := main.cpp

STATIC_EXEC := Ybuild
SHARED_EXEC := yaml-build
STATIC := -static

${SOURCE}.o: ${SOURCE}
	${CXX} ${CXXFLAGS} -c ${SOURCE} -o ${SOURCE}.o

${STATIC_EXEC}: ${SOURCE}.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${SOURCE}.o ${STATIC} -o ${STATIC_EXEC}

${SHARED_EXEC}: ${SOURCE}.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${SOURCE}.o -o ${SHARED_EXEC}

yaml: ${SHARED_EXEC}

ybuild: ${STATIC_EXEC}

clean:
	rm -f ${SOURCE}.o ${STATIC_EXEC} ${SHARED_EXEC}

.PHONY: clean yaml ybuild

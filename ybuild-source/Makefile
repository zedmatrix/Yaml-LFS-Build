CC := gcc
CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2 -march=x86-64
LDFLAGS := -lssl -lcrypto -lcurl -lyaml-cpp

SOURCE := main.cpp
EXEC := parse-yaml

TEST1 := test.cpp
OUT1 := fs-test

${SOURCE}.o: ${SOURCE}
	${CXX} ${CXXFLAGS} -c ${SOURCE} -o ${SOURCE}.o

${EXEC}: ${SOURCE}.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${SOURCE}.o -o ${EXEC}

yaml: ${EXEC}

${TEST1}.o: ${TEST1}
	${CXX} ${CXXFLAGS} -c ${TEST1} -o ${TEST1}.o

${OUT1}: ${TEST1}.o
	${CXX} ${CXXFLAGS} ${TEST1}.o -o ${OUT1}

test: ${OUT1}

clean:
	rm -f ${SOURCE}.o ${EXEC} ${TEST1}.o ${OUT1}

.PHONY: clean yaml test

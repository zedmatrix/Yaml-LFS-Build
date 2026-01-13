CC := gcc
CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2

LDFLAGS_STATIC := -static -lcurl -lidn2 -lpsl -lunistring -lzstd -lz -lssl -lcrypto -lyaml-cpp -lmagic -llzma -lbz2
LDFLAGS_SHARED := -lcurl -lyaml-cpp -lssl -lcrypto -lmagic

SOURCE := main.cpp
OBJ := $(SOURCE:.cpp=.o)

STATIC_EXEC := Ybuild
SHARED_EXEC := yaml-build

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(STATIC_EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS_STATIC) -o $(STATIC_EXEC)

$(SHARED_EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS_SHARED) -o $(SHARED_EXEC)

all: clean yaml ybuild

yaml: clean-shared $(SHARED_EXEC)

ybuild: clean-static $(STATIC_EXEC)

clean: clean-static clean-shared
	rm -f $(OBJ)

clean-static:
	rm -f $(STATIC_EXEC)

clean-shared:
	rm -f $(SHARED_EXEC)

.PHONY: all clean clean-static clean-shared yaml ybuild

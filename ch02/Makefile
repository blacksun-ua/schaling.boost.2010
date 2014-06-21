CC=g++
CXXFLAGS=-std=c++03 -Wall

EX1=1
EX2=2
ADD1=add1

all: $(EX1) $(EX2) $(ADD1)

$(EX1): $(EX1).cpp
	$(CC) $(CXXFLAGS) $@.cpp -o $@

$(EX2): $(EX2).cpp
	$(CC) $(CXXFLAGS) $@.cpp -o $@

$(ADD1): $(ADD1).cpp
	$(CC) $(CXXFLAGS) $@.cpp -o $@


clean:
	rm -rf $(EX1) $(EX2) $(ADD1)

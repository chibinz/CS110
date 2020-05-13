ARCH = x86-64

ifndef DEBUG
	CXXFLAGS = -march=$(ARCH) -O2 -fopenmp -pthread
else
	CXXFLAGS = -march=$(ARCH) -g -fopenmp -pthread
endif

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

calc: calc.o main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) calc *.o

.PHONY: clean


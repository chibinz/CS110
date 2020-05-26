ARCH = skylake

ifndef DEBUG
	CXXFLAGS = -mavx512f -O2 -fopenmp -pthread
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

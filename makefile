CXX = c++-4.2
CXXFLAGS = -Wall -O3 -funroll-loops -fomit-frame-pointer -fexpensive-optimizations

STRING_FILES = \
	StringTest.o

TL_FILES = \
	TL.o

LINCOMB_TEST_FILES = \
	LinCombTest.o

lincomb-test: $(LINCOMB_TEST_FILES)
	$(CXX) $(CXXFLAGS) -o lincomb_test $(LINCOMB_TEST_FILES)

string-test: $(STRING_FILES)
	$(CXX) $(CXXFLAGS) -o string_test $(STRING_FILES)

clean: 
	rm -f *.o

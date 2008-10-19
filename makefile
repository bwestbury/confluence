CXX = c++-4.2
CXXFLAGS = -Wall -O3 -funroll-loops -fomit-frame-pointer -fexpensive-optimizations

BOARD_FILES = \
	drawing/BoardLib/Board.o \
	drawing/BoardLib/Color.o \
	drawing/BoardLib/Rect.cpp \
	drawing/BoardLib/ShapeList.o \
	drawing/BoardLib/Shapes.o \
	drawing/BoardLib/Transforms.o

DRAWING_FILES = \
	drawing/crc.o \
	drawing/triang.o \
	drawing/drawing.o

STRING_FILES = \
	StringTest.o

board: $(BOARD_FILES)

drawing: $(DRAWING_FILES)

drawingtest: board drawing drawing/test.o 
	$(CXX) $(CXXFLAGS) -o drawing/drawing_test drawing/test.o $(DRAWING_FILES) $(BOARD_FILES)

string: $(STRING_FILES)
	$(CXX) $(CXXFLAGS) -o string_test $(STRING_FILES)

clean: 
	rm -f *.o
	rm -f BoardLib/*.o
	rm -f drawimg/*.o

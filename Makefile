# Configure compiler settings
CXX=clang++
CXXFLAGS=-gdwarf-4 -std=c++11 -Werror -D_GLIBCXX_DEBUG
# The header files for the program
HFILES = $(shell ls -1 *.h)
# unittest++ keeps its object files in this directory.
UNITTEST_LIB = -lUnitTest++

all: wordcount tests manualTests written-trees/problem2.3.tex

# This target builds your main program.
wordcount: main.o
	$(CXX) $(CXXFLAGS) -o $@ main.o

# This target builds your tests.
tests: tests.o
	$(CXX) $(CXXFLAGS) -o $@ tests.o $(UNITTEST_LIB)

# This target builds your manual testing program.
manualTests: manualTests.o
	$(CXX) $(CXXFLAGS) -o $@ manualTests.o

# This target describes how to compile a .o file from a .cpp file.
%.o: %.cpp $(HFILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# This target describes how to compile .tex files from .tree files.
%.tex: %.tree
	python textree.py $<

# This target deletes the temporary files we have built.
.PHONY: clean all

clean:
	rm -f *.o
	rm -f wordcount
	rm -f tests
	rm -f manualTests
	rm -f $(shell ls -1 *.tree written-trees/*.tree 2>/dev/null | sed -re 's/\.tree$$/\.tex/g')
	rm -f *.aux *.log *.fdb_latexmk *.fls *.synctex.gz


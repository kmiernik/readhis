CPP = g++
CPPFLAGS = -Wall
#Source dir
SDIR = 

#Rule to make .o from .cpp files
%.o: $(SDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $(SDIR)/$@
		
all: readhis test

readhis: readhis.o HisDrr.o Histogram.o
	$(CPP) $(CPPFLAGS) -o $@ readhis.o HisDrr.o Histogram.o

test: test.o Histogram.o
	$(CPP) $(CPPFLAGS) -o $@ Histogram.o test.o
clean: 
	rm -f *.o *~ readhis test

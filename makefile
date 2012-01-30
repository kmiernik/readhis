CPP = g++
CPPFLAGS = -Wall
#Source dir
SDIR = src
#Header dir
HDIR = include
#Examples dir
EXDIR = examples

#Rule to make .o from .cpp files
%.o: $(SDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -I $(HDIR) -c $< -o $@

all: readhis 

readhis: readhis.o HisDrr.o Histogram.o HisDrrHisto.o Options.o Exceptions.o
	$(CPP) $(CPPFLAGS) -o $@ readhis.o HisDrr.o Histogram.o HisDrrHisto.o Options.o Exceptions.o

test: Histogram.o Exceptions.o test.o
	$(CPP) $(CPPFLAGS) -o $@ test.o Histogram.o Exceptions.o

test.o: $(EXDIR)/test.cpp
	$(CPP) $(CPPFLAGS) -I $(HDIR) -c $< -o $@

clean: 
	rm -f *.o *~ include/*~ src/*~ readhis test

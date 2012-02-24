CPP = g++
CPPFLAGS = -Wall
#Source dir
SDIR = src
#Header dir
HDIR = include

#Rule to make .o from .cpp files
%.o: $(SDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -I $(HDIR) -c $< -o $@

all: readhis 

readhis: readhis.o HisDrr.o Histogram.o HisDrrHisto.o Options.o Debug.o Polygon.o
	$(CPP) $(CPPFLAGS) -o $@ readhis.o HisDrr.o Histogram.o HisDrrHisto.o Options.o Debug.o Polygon.o

install:
	cp readhis /usr/local/bin

clean: 
	rm -f *.o *~ include/*~ src/*~ readhis

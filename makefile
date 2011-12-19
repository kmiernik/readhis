CPP = g++
CPPFLAGS = -Wall
#Source dir
SDIR = 

#Rule to make .o from .cpp files
%.o: $(SDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $(SDIR)/$@
		
all: readhis

readhis: readhis.o hisdrr.o readHisClass.o
	$(CPP) $(CPPFLAGS) -o $@ readhis.o hisdrr.o readHisClass.o

clean: 
	rm -f *.o *~ readhis

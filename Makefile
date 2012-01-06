CPP = g++
CPPFLAGS = -Wall
#Source dir
SDIR = 

#Rule to make .o from .cpp files
%.o: $(SDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $(SDIR)/$@
		
all: readhis zerotest createhis

readhis: readhis.o hisdrr.o readHisClass.o
	$(CPP) $(CPPFLAGS) -o $@ readhis.o hisdrr.o readHisClass.o

zerotest: zerotest.o hisdrr.o
	$(CPP) $(CPPFLAGS) -o $@ zerotest.o hisdrr.o 

createhis: createhis.o hisdrr.o
	$(CPP) $(CPPFLAGS) -o $@ createhis.o hisdrr.o

clean: 
	rm -f *.o *~ 

CPP = g++
CPPFLAGS = -Wall
#Source dir
SDIR = 

#Rule to make .o from .cpp files
%.o: $(SDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $(SDIR)/$@
		
all: readhis zerotest createhis

readhis: readhis.o HisDrr.o readHisClass.o
	$(CPP) $(CPPFLAGS) -o $@ readhis.o HisDrr.o readHisClass.o

zerotest: zerotest.o HisDrr.o
	$(CPP) $(CPPFLAGS) -o $@ zerotest.o HisDrr.o 

createhis: createhis.o HisDrr.o
	$(CPP) $(CPPFLAGS) -o $@ createhis.o HisDrr.o

clean: 
	rm -f *.o *~ 

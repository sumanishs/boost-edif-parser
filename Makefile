CC=g++
IPATHS=-I. -I/home/sumanish/Boost_1.68/boost_1_68_0 
CFLAGS=-g
LPATHS=-L/home/sumanish/Boost_1.68/boost_1_68_0/stage/lib
LFLAGS=
DEPS = EDIFReader.hpp	\
	   EDIFParserFunctors.hpp 
OBJS = EDIFReader.o	\
	   main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(IPATHS)

Parse: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LPATHS) $(LFLAGS)

clean:
	rm -f *.o Parse

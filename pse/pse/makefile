
all: pscx_main
	
pscx_main: pscx_bios.o pscx_cpu.o pscx_instruction.o pscx_interconnect.o pscx_main.o
	g++ pscx_bios.o pscx_cpu.o pscx_instruction.o pscx_interconnect.o pscx_main.o -o pscx_main

pscx_bios.o: pscx_bios.cpp
	g++ -c pscx_bios.cpp	

pscx_cpu.o: pscx_cpu.cpp
	g++ -c pscx_cpu.cpp

pscx_instruction.o: pscx_instruction.cpp
	g++ -c pscx_instruction.cpp

pscx_interconnect.o: pscx_interconnect.cpp
	g++ -c pscx_interconnect.cpp

pscx_main.o: pscx_main.cpp
	g++ -c pscx_main.cpp

clean:
	rm -rf *.o pscx_main


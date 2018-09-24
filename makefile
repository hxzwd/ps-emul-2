
all: pscx_main
	
pscx_main: pscx_bios.o pscx_cpu.o pscx_instruction.o pscx_interconnect.o pscx_main.o
	g++ pscx_bios.o pscx_cpu.o pscx_instruction.o pscx_interconnect.o pscx_main.o -o pscx_main

pscx_bios.o: pscx_bios.cpp
	g++ -c pscx_bios.cpp -std=c++11

pscx_cpu.o: pscx_cpu.cpp
	g++ -c pscx_cpu.cpp -std=c++11

pscx_instruction.o: pscx_instruction.cpp
	g++ -c pscx_instruction.cpp -std=c++11

pscx_interconnect.o: pscx_interconnect.cpp
	g++ -c pscx_interconnect.cpp -std=c++11

pscx_main.o: pscx_main.cpp
	g++ -c pscx_main.cpp -std=c++11

clean:
	rm -rf *.o pscx_main


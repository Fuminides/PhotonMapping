compile:
	g++  -pthread -O3 -std=c++11 -I ./Cabeceras/ ./Sources/*.cpp -oray-tracer
clean:
	rm vector.out

	

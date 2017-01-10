compile:
	g++  -pthread -Ofast -std=c++11 -I ./Cabeceras/ ./Sources/*.cpp -oray-tracer
dirt:
	g++  -pthread -std=c++11 -I ./Cabeceras/ ./Sources/*.cpp -oray-tracer
clean:
	rm ray-tracer

	

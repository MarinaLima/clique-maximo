all: main.cpp
	g++ -o main main.cpp -Wall -std=c++11 -pg -g3

1: 
	./main instancias/1.clq
	
2: 
	./main instancias/2.clq

3: 
	./main instancias/3.clq

4: 
	./main instancias/4.clq
	
5: 
	./main instancias/5.clq
	
6: 
	./main instancias/6.clq
	
7: 
	./main instancias/7.clq
	
8: 
	./main instancias/8.clq
	

run: 2 5 4 3 7 8 6 1
	
INC=-I/home/santaclos/Desktop/tdlib/
OTHER=-lGL -lGLU -lglut -std=c++11

default:	main

main:   gui.cpp
		g++ gui.cpp -o gui $(INC) $(OTHER)
		#gcc -Wall $(LIB) $(INC) -I/opt/informix/incl/public -c test.cpp

clean:
		rm -r gui.o gui.out
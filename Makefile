all:
	g++ --std=c++11 main.cpp -o /home/z4p/dev/sfml/koshi/dist/Debug/GNU-Linux-x86/koshi -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	
clean:
	rm -rf test

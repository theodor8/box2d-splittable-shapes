all: main.out

clean:
	rm -rf *.out

main.out: *.cpp
	g++ *.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lbox2d -std=c++17 -o main.out

run: main.out
	./main.out

.PHONY: all, clean, run
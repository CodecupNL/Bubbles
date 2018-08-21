objects=main.o bord.o woorden.o extio.o grafisch.o enkel.o volledig.o debug.o
dest=./bubbles
flags=-v
test1=./rand
test2=./prog
CXXFLAGS=-Wall -O2 -g
linkflags=-lncurses
$(dest): $(objects)
	g++ $(CXXFLAGS) $(linkflags) -o $(dest) $(objects)
$(objects): $(wildcard *.h) # truc, is niet helemaal netjes 
# als er een header verandert dan moet alles opnieuw gecompileert

$(test1): $(test1).o
.PHONY: debug clean rebuild
debug: $(dest)
	gdb $(dest)
clean:
	rm -f $(dest) $(objects)
rebuild: clean $(dest)
run: $(dest) $(test1)
	$(dest) $(flags) $(test1) $(test2)

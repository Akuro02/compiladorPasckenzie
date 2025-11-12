pasckenzie: pasckenzie.c
	gcc -Wall -Wno-unused-result -g -Og pasckenzie.c -o compilador -lm

test: compilador
	./compilador programaTeste.txt

clean:
	rm -f compilador
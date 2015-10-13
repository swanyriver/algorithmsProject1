all: maxsubarray

maxsubarray:
	gcc -o maxsubarray maxsubarray.c


clean:
	rm maxsubarray
	rm o.txt


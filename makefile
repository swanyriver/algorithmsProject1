all: maxsubarray

maxsubarray: maxsubarray.c
	gcc -o maxsubarray maxsubarray.c


clean:
	rm maxsubarray
	rm o.txt


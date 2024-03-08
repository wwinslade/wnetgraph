CC = g++
CFLAGS = -Wall -Wextra

JGRAPH_MAC = /Users/williamwinslade/Documents/jgraph/jgraph
JGRAPH_HYDRA = /home/jplank/bin/LINUX-X86_64/jgraph JGRAPH
JGRAPH = $(JGRAPH_MAC)

all: netgraph_to_jgr

netgraph_to_jgr:
	$(CC) $(CFLAGS) -o wnet netgraph_to_jgr.cpp 

examples: ex1 ex2 ex3 ex4 ex5
	echo 'Examples compiled in PDF form in ./output/'

ex1:
	./wnet examples/1.txt output/1.jgr
	$(JGRAPH) -P output/1.jgr > output/1.ps
	ps2pdf output/1.ps output/1.pdf

ex2:
	./wnet examples/2.txt output/2.jgr
	$(JGRAPH) -P output/2.jgr > output/2.ps
	ps2pdf output/2.ps output/2.pdf

ex3:
	./wnet examples/3.txt output/3.jgr
	$(JGRAPH) -P output/3.jgr > output/3.ps
	ps2pdf output/3.ps output/3.pdf

ex4:
	./wnet examples/4.txt output/4.jgr
	$(JGRAPH) -P output/4.jgr > output/4.ps
	ps2pdf output/4.ps output/4.pdf

ex5:
	./wnet examples/5.txt output/5.jgr
	$(JGRAPH) -P output/5.jgr > output/5.ps
	ps2pdf output/5.ps output/5.pdf




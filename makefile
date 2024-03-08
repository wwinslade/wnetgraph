CC = g++
CFLAGS = -Wall -Wextra

JGRAPH_MAC = /Users/williamwinslade/Documents/jgraph/jgraph
JGRAPH_HYDRA = /home/jplank/bin/LINUX-X86_64/jgraph JGRAPH
JGRAPH = $(JGRAPH_MAC)

CONV = convert -density 750

all: begin symbols netgraph_to_jgr examples

begin:
	@echo 'Compiling source code and symbol .eps files'
	@echo 'Creating example .pdf files ...'
	@echo ''

netgraph_to_jgr:
	$(CC) $(CFLAGS) -o wnet netgraph_to_jgr.cpp 

symbols: client firewall router server switch
	@echo 'Symbol .eps files created in ./symbols/'

client:
	@$(JGRAPH) symbols/client.jgr > symbols/client.eps

firewall:
	@$(JGRAPH) symbols/firewall.jgr > symbols/firewall.eps

router:
	@$(JGRAPH) symbols/router.jgr > symbols/router.eps

server:
	@$(JGRAPH) symbols/server.jgr > symbols/server.eps

switch:
	@$(JGRAPH) symbols/switch.jgr > symbols/switch.eps

examples: ex1 ex2 ex3 ex4 ex5
	@echo ''
	@echo 'All examples compiled in PDF form in ./pdf/'

ex1:
	@./wnet examples/1.txt output/1.jgr
	@$(JGRAPH) -P output/1.jgr > ps/1.ps
	$(CONV) ps/1.ps pdf/1.pdf
	@echo 'Example 1 successfully created'

ex2:
	@./wnet examples/2.txt output/2.jgr
	@$(JGRAPH) -P output/2.jgr > ps/2.ps
	@$(CONV) ps/2.ps pdf/2.pdf
	@echo 'Example 2 successfully created'
	
ex3:
	@./wnet examples/3.txt output/3.jgr
	@$(JGRAPH) -P output/3.jgr > ps/3.ps
	@$(CONV) ps/3.ps pdf/3.pdf
	@echo 'Example 3 successfully created'

ex4:
	@./wnet examples/4.txt output/4.jgr
	@$(JGRAPH) -P output/4.jgr > ps/4.ps
	@$(CONV) ps/4.ps pdf/4.pdf
	@echo 'Example 4 successfully created'

ex5:
	@./wnet examples/5.txt output/5.jgr
	@$(JGRAPH) -P output/5.jgr > ps/5.ps
	@$(CONV) ps/5.ps pdf/5.pdf
	@echo 'Example 5 successfully created'

clean:
	@rm -f wnet
	@rm -f output/*
	@rm -f symbols/*.eps
	@rm -f pdf/*
	@rm -f output/*
	@rm -f ps/*
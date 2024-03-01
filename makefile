CC = g++
CFLAGS = -Wall -Wextra 

JGRAPH_MAC = /Users/williamwinslade/Documents/jgraph/jgraph
JGRAPH_HYDRA = /home/jplank/bin/LINUX-X86_64/jgraph JGRAPH = $(JGRAPH_MAC)
JGRAPH_WINDOWS = 

symbols:
	$(JGRAPH) symbols/router.jgr > symbols/router.eps
	$(JGRAPH) symbols/switch.jgr > symbols/switch.eps
	$(JGRAPH) symbols/client.jgr > symbols/client.eps

netgraph_to_jgr:




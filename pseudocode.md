# Pesudocode for Main program

class node
  hostname
  int depth
  device type
  vector of peers < node * >
  vector of children < node * >

class graph
  node *sentinel
  vector of *nodes (not including the sentinel)
  int printwidth, printheight

graph::graph(argc, argv)
  Perform input validation on CLI passed arguments, check for good input and output file

  create a new node to act as the sentinel
  set sentinel depth to -1
  set sentinel name to "---SENTINEL---"
  set sentinel device type to NULL
  
  get lines of a file until the end of the file
    create a string stream from the line
    get first word of the line
    if first word == NETWORK
      read in the rest of the line's network attributes (dfgw, label, peers)
      create a new node, set depth to 0
      set new node to be a child of the sentinel
      if applicable, set peer links
    else if first word == SWITCH, ROUTER, or CLIENT
      read in the rest of the line's device attributes (type, children, parents, peers)
      create a new node, set depth to be parent's depth plus 1
      set node's attributes to represent device's attributes
      create links between node and its parents (or children, if applicable)
    else
      throw error, bad file format
  return

int graph::find_print_dims()
  Traverse the graph's vector of all nodes
    Count the nodes that are leafs
    Track the largest depth integer encountered
  Depending on file spec, set the printing width based on the number of leaf nodes
  Depending on file spec, set the printing height based on the number of leaf nodes

  return

void graph::to_jgraph()
  

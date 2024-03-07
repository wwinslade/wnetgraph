#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

// Typedef for spec of what the network device type is
typedef enum 
{ 
  GLOBAL,
  GATEWAY,
  FIREWALL,
  ROUTER,
  SWITCH,
  HOST,
} NODETYPE;

// Node class. These are held in a map keyed on the hostname string.
class Node {
  public:
    Node();
    string hostname;
    int depth;
    double x, y;
    NODETYPE type;
    vector < class Node * > children;
    vector < class Node * > peers;
    vector < class Node * > parents;
};

// Simple constructor just to initialize some values
Node::Node() {
  hostname = "";
  depth = 0;
  type = HOST;
  x = 0;
  y = 0;
  children.clear();
  peers.clear();
  parents.clear();

}

// The class that does all the real work. 
class Graph {
  public:
    Graph();
    void Read_From_Stream();        // Facilitates reading in from a stream (fstream right now). Calls Read_New_Device when the stream contains an unique hostname
    void Read_New_Device(Node *n);  // Facilitates the creation of each device's node, creating links between nodes
    void Print_Device();            // Debugging method, prints out each device, its xy coords, depth, and links
    void Assign_Depths(Node *n);    // Performs a simple pre-order DFS and assigns depths to each node
    void Count_Depths();
    void Calculate_Positions();     // Determines how big the graph is, calculates places where Assign_Positions can place nodes in space, and assigns them to each node
    void Write_Links(Node *n);      // Writes jgraph link lines between nodes to output stream via DFS
    void Write_Jgraph();            // Writes jgraph setup, calls Write_Links, and prints each node's device symbol to the output stream

    int out_mode;                   // Defunct right now

    int num_leafs;                  // The number of leaf nodes in the graph
    int max_depth;                  // The maximum depth of the graph
    int nx, ny;                     // The dimensions of the jgraph graph

    Node *root;                     // Sentinel node for ease of access
    map < string, Node * > all_nodes; // Map of all nodes, keyed on the hostname string
    map < NODETYPE, string > eps_files; // Map of file locations for the .eps files used to print the various device symbols

    vector < int > depth_counts;    // Vector of number of nodes at each depth, where the depth is the index
    vector < double > x_coords;     // Vector of possible x locations to place a node
    vector < double > y_coords;     // Vector of possible y locations to place a node

    ifstream fin;                   // Input file stream (.txt)
    ofstream fout;                  // Output file stream (.jgr)

};

// Basic constructor for the graph class. Initializes some variables for convenience
Graph::Graph() {
  out_mode = 0;
  num_leafs = 0;
  max_depth = 0;
  nx = 0;
  ny = 0;
}

void Graph::Read_New_Device(Node *n) 
{
  string line_in = "";
  string word1, wordn;
  string rest;
  istringstream sin;
  map < string, Node * >::iterator mit;
  Node *nit;

  while(!fin.eof()) {
    getline(fin, line_in);
    sin.clear();
    sin.str(line_in);
    
    sin >> word1;
    if (line_in.size() == 0) continue;

    if (word1 == "hostname") {
      wordn.clear();
      rest.clear();
      while (sin >> wordn) {
        rest += wordn;
      }

      n->hostname = rest;
    } else if (word1 == "uplink") {
      wordn.clear();
      rest.clear();
      while (sin >> wordn) {
        rest += wordn;
      }

      mit = all_nodes.find(wordn);

      if (mit != all_nodes.end()) {
        // Have seen node before, fetch it
        nit = mit->second;
        nit->children.push_back(n);
        n->parents.push_back(nit);
      } else {
        // Node has not been seen yet, create it and push it to map
        nit = new Node;
        nit->hostname = wordn;
        nit->children.push_back(n);
        n->parents.push_back(nit);
        all_nodes.insert(make_pair(wordn, nit));
      }
    } else if (word1 == "peer") {
      wordn.clear();
      rest.clear();
      while (sin >> wordn) {
        rest += wordn;
      }

      mit = all_nodes.find(wordn);
      if (mit != all_nodes.end()) {
        // Have seen node before, fetch it
        nit = mit->second;
        n->peers.push_back(nit);
        nit->peers.push_back(n);
      } else {
        // Node has not been seen yet, create it and push it to map
        nit = new Node;
        nit->hostname = wordn;
        nit->peers.push_back(n);
        n->peers.push_back(nit);
        all_nodes.insert(make_pair(wordn, nit));
      }

    } else if (word1 == "host") {
      wordn.clear();
      rest.clear();
      while (sin >> wordn) {
        rest += wordn;
      }

      mit = all_nodes.find(wordn);
      if (mit != all_nodes.end()) {
        // Have seen node before, fetch it
        nit = mit->second;
        nit->parents.push_back(n);
        n->children.push_back(nit);
      } else {
        // Node has not been seen yet, create it and push it to map
        nit = new Node;
        nit->hostname = wordn;
        nit->parents.push_back(n);
        n->children.push_back(nit);
        all_nodes.insert(make_pair(wordn, nit));
      }
      
    } else if (word1 == "type") {
      sin >> wordn;
      if (wordn == "router") {
        n->type = ROUTER;
      } else if (wordn == "firewall") {
        n->type = FIREWALL;
      } else if (wordn == "switch") {
        n->type = SWITCH;
      } else if (wordn == "gateway") {
        if (root->children.size() > 0) {
          printf("\t\tError: Only one gateway may be defined per network, rejected\n");
        } else {
          n->type = GATEWAY;
          root->children.push_back(n);
        }
      } else if (wordn == "host") {
        n->type = HOST;
      } else {
        printf("\t\tType unrecognized, try again");
      }
    } else if (word1 == "end") {
      return;
    } else {
      printf("\tCommand unrecognized, try again");
    }
  }
  return;
}

void Graph::Print_Device() {
  int i;
  Node *n;
  map <string, Node *>::iterator mit;

  for (mit = all_nodes.begin(); mit != all_nodes.end(); mit++) {
    n = mit->second;
    printf("%s (%d) [%lf, %lf]:\n", n->hostname.c_str(), n->depth, n->x, n->y);
    printf("  UPLINKS:\n");
    for (i = 0; i < n->parents.size(); i++) {
      printf("    %s\n", n->parents[i]->hostname.c_str());
    }
    printf("  PEERS:\n");
    for (i = 0; i < n->peers.size(); i++) {
      printf("    %s\n", n->peers[i]->hostname.c_str());
    }
    printf("  HOSTS:\n");
    for (i = 0; i < n->children.size(); i++) {
      printf("    %s\n", n->children[i]->hostname.c_str());
    }
  }
  printf("Num leaf nodes: %d\n", num_leafs);
}

void Graph::Read_From_Stream() 
{
  string line_in;
  string w;
  string name = "";
  istringstream liness;
  Node *device_in;
  map < string, Node * >::iterator nit;
  NODETYPE rm = GLOBAL;
  
  root = new Node;
  root->depth = -1;

  all_nodes.clear();

  while (!fin.eof()) 
  {
    getline(fin, line_in);

    if (line_in.size() == 0) continue;
    if (line_in == "done") break;

    liness.clear();
    liness.str(line_in);
    name.clear();

    while (liness >> w) {
      name += w;
    }

    nit = all_nodes.find(name);
    if (nit != all_nodes.end()) {
      device_in = nit->second;
    } else {
      device_in = new Node;
      device_in->hostname = name;
      all_nodes.insert(make_pair(name, device_in));
    }

    Read_New_Device(device_in);
  }
}

void Graph::Assign_Depths(Node *n) {
  Node *nit;
  int i, current_depth;

  current_depth = n->depth + 1;
  if (current_depth > max_depth) max_depth = current_depth;
  if (n->children.size() == 0) num_leafs++;

  for (i = 0; i < n->children.size(); i++) {
    nit = n->children[i];
    if (current_depth > nit->depth) nit->depth = current_depth;
    Assign_Depths(nit);
  }
}

void Graph::Count_Depths() {
  // To make printing neater, need a count of the number of nodes at each depth.
  // Can now simply traverse the map and count
  Node *nit;
  map < string, Node * >::iterator mit;

  depth_counts.clear();
  depth_counts.resize(max_depth, 0);
  
  for (mit = all_nodes.begin(); mit != all_nodes.end(); mit++) {
    nit = mit->second;
    depth_counts[nit->depth]++;
  }
}

void Graph::Calculate_Positions() {
  /* Potential area for improvement:
    For each node, determine the number of children. Change the size of the x slice depending on the number of children. 
    Do this for every node, so that children of parents are closer to eachother, reducing line overlap
  */
  
  
  int dims_coeff;
  int i;
  double minx, maxx, miny, maxy;
  double xslice_width;
  double yslice_width;
  double pos;
  Node *nit;
  map < string, Node * >::iterator mit;
  vector < int > pos_counter;

  // Go ahead and call the depth count method
  Count_Depths();

  // A coefficient for how big the graph should be relative to its number of leaf nodes
  dims_coeff = 10;

  x_coords.clear();
  y_coords.clear();

  // If the graph has more leafs nodes than depth, use that to determine the size. Otherwise, vice versa
  if (num_leafs > max_depth) {
    nx = num_leafs * dims_coeff;
    ny = nx;
  } else {
    nx = max_depth * dims_coeff;
    ny = nx;
  }

  // I want (0,0) to be dead center of the graph, so find the min and max x and y
  minx = 0 - ((double) nx / 2);
  maxx = 0 + ((double) nx / 2);
  miny = 0 - ((double) ny / 2);
  maxy = 0 + ((double) ny / 2);

  printf("Global: x:[%lf, %lf] ; y:[%lf, %lf]\n", minx, maxx, miny, maxy);

  // I want to break the graph up into nice slices to neatly place the symbols, so determine the size of each slice;
  xslice_width = (double) nx / num_leafs;
  yslice_width = (double) ny / (max_depth + 1);

  printf("X slice width: %lf\n", xslice_width);
  printf("Y slice width: %lf\n", yslice_width);

  /*
  Here I will put coordinates for potential symbols into a vector.
  Depending on the depth of each node, and how many nodes are at that depth,
  the nodes will be printed, starting from the top center of the graph.
  */

  x_coords.clear();
  y_coords.clear();
  
  // If the number of leafs is odd, we will consider 0 as eligible for a symbol. Otherwise, don't
  // This will help with neatness
  /* if (num_leafs % 2 == 0) {
    
  } else {
    x_coords.push_back(0);
  } */

  x_coords.push_back(0);

  // Debug prints
  printf("Num leafs: %d\n", num_leafs);
  printf("Max depth: %d\n", max_depth);

  for (i = 1; i < (num_leafs / 2) + 1; i++) {
    x_coords.push_back(0 + ((xslice_width) * (double) i));
    x_coords.push_back(0 - ((xslice_width) * (double) i));
  }

  for (i = 1; i < max_depth + 2; i++) {
    y_coords.push_back(maxy - (yslice_width * i));
  }

  // Debugging printing in these two loops
  printf("x_coords:");
  for (i = 0; i < x_coords.size(); i++) {
    printf(" %lf", x_coords[i]);
  }
  printf("\n");

  printf("y_coords:");
  for (i = 0; i < y_coords.size(); i++) {
    printf(" %lf", y_coords[i]);
  }
  printf("\n");

  // Initialize the position counter. Will be useful for keeping track of where each node at each depth should be printed
  pos_counter.clear();
  pos_counter.resize(max_depth, 0);

  // For each depth, check if the number of nodes is even. If it is, exclude x=0, else, include it (help with centering for depths where num nodes is even)
  for (i = 0; i < pos_counter.size(); i++) {
    if (depth_counts[i] % 2 == 0) {
      pos_counter[i] = 1;
    }
  }

  // The gateway node will have a preset point in space, always want it in the top center of the graph
  nit = root->children[0];
  nit->x = 0;
  nit->y = y_coords[nit->depth];

  // Assign coordinate values to the rest of the nodes
  for (mit = all_nodes.begin(); mit != all_nodes.end(); mit++) {
    if (mit->second == root->children[0]) continue; // Always want the gateway to be at x=0, so skip it
    nit = mit->second;
    nit->x = x_coords[pos_counter[nit->depth]];
    nit->y = y_coords[nit->depth];
    pos_counter[nit->depth]++;
  }

}


void Graph::Write_Links(Node *n) {
  int i;

  for (i = 0; i < n->children.size(); i++) {
    fout << "newline pts " << n->x << " " << n->y << " " << n->children[i]->x << " " << n->children[i]->y << endl;
    Write_Links(n->children[i]);
  }

  for (i = 0; i < n->peers.size(); i++) {
    fout << "newline pts " << n->x << " " << n->y << " " << n->peers[i]->x << " " << n->peers[i]->y << endl;
  }
}

void Graph::Write_Jgraph() {
  ostream *os;
  Node *nit;
  map < string, Node * > ::iterator mit;
  int dims_coeff;
  double dims_offset;

  dims_coeff = 10;
  dims_offset = 0.3;

  // Write the graph specs
  fout << "newgraph" << endl;
  fout << "xaxis min " << 0-(nx/ (double) 2) << " max " << (nx/ (double) 2) << " nodraw" << endl;
  fout << "yaxis min " << 0-(nx/ (double) 2) << " max " << (nx/ (double) 2) << " nodraw" << endl;
  fout << endl;

  // Output link lines
  Write_Links(root->children[0]);
  fout << endl;

  // Write the symbols for each device
  for (mit = all_nodes.begin(); mit != all_nodes.end(); mit++) {
    nit = mit->second;
    fout << "newcurve eps " << eps_files[nit->type] << " marksize " << dims_coeff * dims_offset << " " << dims_coeff * dims_offset << " pts " << nit->x << " " << nit->y << endl;
  }

}

int main(int argc, char **argv) {
  // Variables
  Graph g;

  g.eps_files.clear();
  g.eps_files.insert(make_pair(GATEWAY, "symbols/router.eps"));
  g.eps_files.insert(make_pair(ROUTER, "symbols/router.eps"));
  g.eps_files.insert(make_pair(SWITCH, "symbols/switch.eps"));
  g.eps_files.insert(make_pair(HOST, "symbols/client.eps"));

  g.fin.open(argv[1]);
  g.fout.open(argv[2]);

  g.Read_From_Stream();
  g.Assign_Depths(g.root->children[0]);

  g.Calculate_Positions();
  g.Print_Device();
  g.Write_Jgraph();
  
}
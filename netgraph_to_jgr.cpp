#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Node {
  public:
    string hostname;
    int layer;
    vector < class Node * > children;
    vector < class Node * > parents;

};

class Graph {
  public:
    Node *root;
    vector < Node * > all_nodes;
};

int main(int argc, char **argv) {
  // Variables
  ifstream fin;
  ofstream fout;
  
  try {

  } catch (const exception &e) {
    fprintf(stderr, "%s\n", e.what());
    exit(1);
  }
}
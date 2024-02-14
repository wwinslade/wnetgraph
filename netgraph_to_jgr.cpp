#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Network {
  public:
    Network(int argc, char **argv);

  private:
    string filename;
    ifstream fin;
    
    int validate_file(char *fn);
    
};

Network::Network(int argc, char **argv) {
  // Variables
  string line_in;
  istringstream line_ss;
  
  // Input validation
  try {
    validate_file(argv[1]);
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
    throw;
  }

  // File is good, proceed with reading in input
  while (getline(fin, line_in)) {
    if (line_in.size() == 0) continue; // Line is empty, ignore it
    
    string w1;

    line_ss.clear();
    line_ss.str(line_in);

    line_ss >> w1;
    if (w1 == "//") continue; // Line is a comment or is empty, ignore it

    if (w1 == "NETWORK") {

    } else if (w1 == "SWITCH") {
      
    }

  }
};

int Network::validate_file(char *fn) {
  filename = fn;

  fin.open(filename);
  if (!fin.is_open()) throw runtime_error("File not found or could not be opened");

  if (filename.size() < 5 || filename.substr(filename.size() - 5) != ".netg") {
    throw invalid_argument("Invalid file extension. File must be of type .netg");
  }
};

int main(int argc, char **argv) {
  Network NET(argc, argv);
  

  return 0;
}
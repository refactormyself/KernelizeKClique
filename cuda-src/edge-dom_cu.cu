#include "../include/EdgeDomSerial.hpp"
#include "../include/common.hpp"
#include "../include/cpp_util.hpp"
#include "../include/EdgeDomCUDA.cuh"

#include <chrono>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>
#include <thread>

using namespace std;

void ExitWithProperUseMessage() {
  printf("LAUNCH ERROR: \n"
         "HERE IS HOW SOFTWARE CAN BE LAUNCHED:\n\n"
         "Supply the number of vertices, number of edges and optionally the "
         "filename thus:\n"
         "$edgedom 5000 10000 adjacency_matrix.csv \n"
         "$edgedom 5000 10000 \n\n");
  exit(1);
}

bool ParseCmdLineArgs(int argc, char *argv[], std::string &datafile_path,
                      size_t &num_vert, size_t &num_edges) {
  if (argc != 3 && argc != 4) {
    std::cout << '\n' << "Argc = " << argc << '\n';
    ExitWithProperUseMessage();
  }

  if (!(IsPositiveNumber(argv[1]) && IsPositiveNumber(argv[2])))
    ExitWithProperUseMessage();

  num_vert = strtol(argv[1], NULL, 10);
  num_edges = strtol(argv[2], NULL, 10);
  if (argc == 4) {
    datafile_path = std::string(argv[3]);
    return false;
  }

  return true; // go and generate random graph
}

void GenerateRandomGraph(
    short *adj_matrix, std::unordered_set<Edge, Edge::HashFunction> &edge_list,
    std::vector<std::vector<int>> &adj_list, size_t num_vertices,
    size_t num_edges) {
  // assert maximum possible edges n-Choose-2
  assert(num_edges <= (num_vertices * (num_vertices - 1) / 2));

  // Assign all vertices to an edge: Create the Critical Path (MST)
  for (long idx = 0; idx < num_vertices; idx++) {
    srand((unsigned)time(0));
    long rand_idx = (idx == 0) ? 1 : idx;

    // avoid a loop
    while (rand_idx == idx) {
      rand_idx = rand() % (idx + 1); // pick from 0 .. i
    }

    size_t idx_1 = idx + (rand_idx * num_vertices);
    size_t idx_2 = rand_idx + (idx * num_vertices);

    // std::cout << " => "<< node1_idx<< ", "<< node2_idx << '\n';
    // Adjacency Matrix
    *(adj_matrix + idx_1) = 1;
    *(adj_matrix + idx_2) = 1;

    auto is_inserted = edge_list.insert({idx, rand_idx}).second;
    if (is_inserted)
      adj_list[idx].push_back(rand_idx);
  }

  // Now asign the  rest edges randomly
  srand((unsigned)time(0));

  // ASSUMPTIONS:
  // Graph is connected
  // adj_matrix is initialised to 0
  // No loop or parallel edge allowed

  while (edge_list.size() < num_edges) {
    auto min = 0;
    auto max = num_vertices - 1;

    auto node1_idx = min + (rand() % static_cast<int>(max + 1 - min));
    auto node2_idx = min + (rand() % static_cast<int>(max + 1 - min));

    if (node1_idx == node2_idx) // avoid a loop edge
      continue;

    size_t idx_1 = node1_idx + (node2_idx * num_vertices);
    size_t idx_2 = node2_idx + (node1_idx * num_vertices);

    if (idx_1 >= num_vertices * num_vertices ||
        idx_2 >= num_vertices * num_vertices)
      throw out_of_range("Attempt to write out range of *AdjMatrix");

    // Adjacency Matrix
    *(adj_matrix + idx_1) = 1;
    *(adj_matrix + idx_2) = 1;

    auto is_inserted = edge_list.insert({node1_idx, node2_idx}).second;
    if (is_inserted)
      adj_list[node1_idx].push_back(node2_idx);
  }
}

// Populates the Adjacency Matrix and EdgeList using data from file
void LoadAdjMatrixFromFile(
    const std::string file_path, short *adj_matrix,
    std::unordered_set<Edge, Edge::HashFunction> &edge_list,
    std::vector<std::vector<int>> &adj_list, const size_t num_vertices,
    const size_t num_edges) {

  std::string file = ReadStringFromFile_Fast(file_path);

  std::istringstream file_stream;
  file_stream.str(file);

  size_t value{0}; //, edge_count{0};
  long row{0}, col{0};

  while (file_stream >> value) {
    if (value == 1) {
      if (col == row) // avoid a loop edge
        throw range_error("Loop found  in Graph data. Loop and Parallel edges "
                          "are not supported");

      *(adj_matrix + (col + row * num_vertices)) = 1;

      size_t idx_1 = col + (row * num_vertices);
      size_t idx_2 = row + (col * num_vertices);

      if (idx_1 >= num_vertices * num_vertices ||
          idx_2 >= num_vertices * num_vertices)
        throw out_of_range("Attempt to write out range of *AdjMatrix");

      // Adjacency Matrix
      *(adj_matrix + idx_1) = 1;
      *(adj_matrix + idx_2) = 1;

      auto is_inserted = edge_list.insert({row, col}).second;
      if (is_inserted)
        adj_list[row].push_back(col);
  
    } else {
      *(adj_matrix + (col + row * num_vertices)) = 0;
    }

    col++;
    if (col == num_vertices) {
      // reset edge idx and goto next node
      col = 0;
      row++;
    }
  }
}

std::string ToStingAdjMatrix_1D(const std::string file_path,
                                const size_t num_vertices, short *adj_matrix) {

  std::ostringstream str_stream(file_path);
  for (size_t i = 0; i < (num_vertices * num_vertices); i++) {
    str_stream << *(adj_matrix + i) << '\n';
  }

  return str_stream.str();
}

int main(int argc, char *argv[]) {
  
  // size_t num_vertices = 14;
  // size_t num_edges = 70;

  // size_t num_vertices = 6;
  // size_t num_edges = 7;

  // size_t num_vertices = 125;
  // size_t num_edges = 500;

  // no delete
  // size_t num_vertices = 125;
  // size_t num_edges = 5500;

  // no delete
  // size_t num_vertices = 125;
  // size_t num_edges = 6250;

  // size_t num_vertices = 1400;
  // size_t num_edges = 7000;

  // size_t num_vertices = 14000;
  // size_t num_edges = 70000;

  size_t num_vertices = 100;
  size_t num_edges = 4900; // 50 less max

  // size_t num_vertices = 1000;
  // size_t num_edges = 450000;

  // The Entire Graph stored as an Adjacency Matrix
  // short *adj_matrix =
  //     (short *)std::malloc(num_vertices * num_vertices * sizeof(short));

  // size_t size = num_vertices * num_vertices;
  // short *adj_matrix = new short[size];
  // Edge List representation of the Graph
  // Edge *edge_list =
  //     (Edge *)std::malloc(num_edges * sizeof *edge_list);

  // Edge *edge_list = new Edge[size]();
  // std::unordered_set<Edge, Edge::HashFunction> edge_list;

  // if (adj_matrix == NULL || edge_list == NULL)
  //   throw runtime_error("Malloc Error !!!");

  std::cout
      << '\n'
      << "With two parameters a random graph is generated.\n"
      << "With three parameters the graph is read from the file.\n"
      << "The parameter details are below:\n\n"
      << "An ADJACENCY MATRIX data representation of the graph is expected in "
         "the supplied file.\n"
      << "So the file will contain (num_vertices * num_vertices) data points.\n"
      << "ONLY white space seprated formats are supported. CSV is not "
         "supported.\n\n"
      << "Please, key in the graph size, file location and type\n\n"
      << "    e.g. edgedom 50 100 'graph.data' \n"
      << "This indicates a graph of num_vertices=50, num_edges=100,"
      << "stored in the file 'graph.data'\n\n";

  // short *adj_matrix = nullptr;
  if (argc > 1) {
    size_t size = num_vertices * num_vertices;
    short *adj_matrix = new short[size];
    std::unordered_set<Edge, Edge::HashFunction> edge_list;
    std::vector<std::vector<int>> adj_list(num_vertices, std::vector<int>());
    std::string file_path = "";
    
    if (ParseCmdLineArgs(argc, argv, file_path, num_vertices, num_edges)){
      size_t size = num_vertices * num_vertices;
      adj_matrix = new short[size];
      GenerateRandomGraph(adj_matrix, edge_list, adj_list, num_vertices,
                          num_edges);
      std::cout<< "\n  Over Here !!!\n";
      
      // Print generated graph to file
      std::string adj_list_str = ToStingAdjList_2D(adj_list, num_vertices);
      std::string input_file = "INPUT-" + std::to_string(num_vertices) 
                                  + "-" + std::to_string(num_edges) + ".csv";
      std::thread threadObj(WriteStringToFile,input_file, adj_list_str);
      threadObj.detach();

    } else {
      size_t size = num_vertices * num_vertices;
      adj_matrix = new short[size];      
      LoadAdjMatrixFromFile(file_path, adj_matrix, edge_list, adj_list,
                            num_vertices, num_edges);
      
    }
    
    //ToDo: consider when a filename is actually passed in
    size = num_vertices * num_vertices;
    std::string output_file = "CUDA-OUTPUT-" + std::to_string(num_vertices) 
                              + "-" + std::to_string(num_edges) + ".csv";
    short *adj_matrix_serial = new short[size];
    short *adj_matrix_cuda = new short[size];
    copy_array(adj_matrix, adj_matrix_serial, size);
    copy_array(adj_matrix, adj_matrix_cuda, size);
    
    std::cout <<'\n' << " CUDA RESULTS: \n";
    PrintResultHeader();
    CUDA_Process(adj_matrix_cuda, edge_list, num_vertices, num_edges, output_file);

    std::cout <<'\n' << " SERIAL RESULTS: \n";
    PrintResultHeader();
    Serial_Process(adj_matrix_serial, edge_list, num_vertices, num_edges);
    
    free(adj_matrix);
    free(adj_matrix_serial);
    free(adj_matrix_cuda);
  } else {
    
    PrintResultHeader();
    std::cout <<'\n';
    for (size_t i = 0; i < 5; i++) 
    {
      num_vertices = 100 + (100 * i);
      num_edges = (num_vertices * (num_vertices - 1) / 2) - (num_vertices);

      size_t size = num_vertices * num_vertices;
      short *adj_matrix = new short[size];
      std::unordered_set<Edge, Edge::HashFunction> edge_list;
      std::vector<std::vector<int>> adj_list(num_vertices, std::vector<int>());

      GenerateRandomGraph(adj_matrix, edge_list, adj_list, num_vertices,
                          num_edges);
      
      // Print generated graph to file
      std::string adj_list_str = ToStingAdjList_2D(adj_list, num_vertices);
      std::string input_file = "INPUT-" + std::to_string(num_vertices) 
                                  + "-" + std::to_string(num_edges) + ".csv";
      std::thread threadObj(WriteStringToFile,input_file, adj_list_str);
      threadObj.detach();

      short *adj_matrix_serial = new short[size];
      short *adj_matrix_cuda = new short[size];
      copy_array(adj_matrix, adj_matrix_serial, size);
      copy_array(adj_matrix, adj_matrix_cuda, size);

      std::string output_file = "CUDA-OUTPUT-" + std::to_string(num_vertices) 
                                 + "-" + std::to_string(num_edges) + ".csv";
            
      std::cout << "CUDA RESULTS: ";      
      CUDA_Process(adj_matrix_cuda, edge_list, num_vertices, num_edges, output_file);

      std::cout<< "SERIAL RESULTS:";
      // PrintResultHeader();
      Serial_Process(adj_matrix_serial, edge_list, num_vertices, num_edges);
      
      free(adj_matrix);
      free(adj_matrix_serial);
      free(adj_matrix_cuda);
    }
  }
}

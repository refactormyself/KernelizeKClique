#ifndef EDGEDOMCUDA_HPP
#define EDGEDOMCUDA_HPP

#include "common.hpp"

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

// using namespace std;

namespace chrono = std::chrono;
using chrono::system_clock;

/*
inline void RemoveDominatedEdge(Match match, size_t *counter, short *adj_matrix,
                             size_t num_vertices) {
  size_t idx_1 = (match.a + match.v * num_vertices);
  size_t idx_2 = (match.v + match.a * num_vertices);

  if (*(adj_matrix + idx_1) == 0 || *(adj_matrix + idx_2) == 0)
    return; // matched edge already invalidated

  for (size_t i = 0; i < num_vertices; i++) {
    bool has_node = *(adj_matrix + (i + match.a * num_vertices)) == 1;
    bool has_adj_node = *(adj_matrix + (i + match.u * num_vertices)) == 0;
    bool has_match_node = *(adj_matrix + (i + match.v * num_vertices)) == 1;
    if (has_node && has_adj_node && has_match_node)
      return; // not dominated
  }

  // Edge [a,v]  is dominated, so delete it
  *(adj_matrix + idx_1) = 0;
  *(adj_matrix + idx_2) = 0;
  (*counter)++;
}

inline void GetAllEdgesDominatedBy_(
    const Edge edge_p, size_t *counter, size_t num_vertices, size_t num_edges,
    short *adj_matrix,
    const std::unordered_set<Edge, Edge::HashFunction> &edge_list) {
  auto ep = edge_p.node1;
  auto fp = edge_p.node2;

  size_t idx_p = (edge_p.node1 + edge_p.node2 * num_vertices);
  if (*(adj_matrix + idx_p) == 0)
    return; // not an active node

  // for (size_t edge_count = 0; edge_count < num_edges; ++edge_count)
  // {
  //   Edge edge_q = edge_list[edge_count];
  for (auto &&edge_q : edge_list) {
    size_t idx_q = (edge_q.node1 + edge_q.node2 * num_vertices);
    if (*(adj_matrix + idx_q) == 0 || *(adj_matrix + idx_p) == 0)
      continue; // not an active node

    // don't find matches for current edge
    if ((edge_p.node1 == edge_q.node1 || edge_p.node1 == edge_q.node2) &&
        (edge_p.node2 == edge_q.node1 || edge_p.node2 == edge_q.node2)) {
      continue;
    }

    size_t a, u, v;
    auto eq = edge_q.node1;
    auto fq = edge_q.node2;

    if (ep == eq) {
      u = edge_p.node2;
      a = edge_p.node1;
      v = edge_q.node2;
      RemoveDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
      continue;
    }

    if (ep == fq) {
      u = edge_p.node2;
      a = edge_p.node1;
      v = edge_q.node1;
      RemoveDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
      continue;
    }

    if (fp == eq) {
      u = edge_p.node1;
      a = edge_p.node2;
      v = edge_q.node2;
      RemoveDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
      continue;
    }

    if (fp == fq) {
      u = edge_p.node1;
      a = edge_p.node2;
      v = edge_q.node1;
      RemoveDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
      continue;
    }
  }
}

inline size_t FindAndDeleteDominatedEdges(
    short *adj_matrix,
    const std::unordered_set<Edge, Edge::HashFunction> &edge_list,
    size_t num_vertices, size_t num_edges) {
  size_t num_deleted_edges = 0;
  size_t previous_count = 1;
  int pass = 1;

  // while (num_deleted_edges != previous_count) { // untill no more deletion
  previous_count = num_deleted_edges;

  // TODO: turn this loop into a Kernel per edge and
  // Reduce the result into all_dom
  // for (size_t count = 0; count < num_edges; ++count) {
  //   Edge edge = edge_list[count];
  for (auto &&edge : edge_list) {
    size_t count_del_nodes = 0;
    GetAllEdgesDominatedBy_(edge, &count_del_nodes, num_vertices, num_edges,
                            adj_matrix, edge_list);
    num_deleted_edges +=
        count_del_nodes; // std::cout << '\n' << "Pass = " << pass++ << "; count
                         // is "<< count_del_nodes;
  }

  // std::cout << '\n' << "Pass = " << pass++ << "; count is "<<
  // count_del_nodes;
  // }

  return num_deleted_edges;
}
*/

__global__
void Test_Kernel(short *adj_matrix_D, Edge *edge_list_D, 
  const size_t num_vertices, const size_t num_edges) {
    

}

__device__
void RemoveDominatedEdge(Match match, size_t *counter, 
                                    short *adj_matrix,
                                    size_t num_vertices) {
  size_t idx_1 = (match.a + match.v * num_vertices);
  size_t idx_2 = (match.v + match.a * num_vertices);

  // if (*(adj_matrix + idx_1) == 0 || *(adj_matrix + idx_2) == 0)
  // return; // matched edge already invalidated

  // #pragma unroll
  for (size_t i = 0; i < num_vertices; i++) {
    bool has_node = *(adj_matrix + (i + match.a * num_vertices)) == 1;
    bool has_adj_node = *(adj_matrix + (i + match.u * num_vertices)) == 0;
    bool has_match_node = *(adj_matrix + (i + match.v * num_vertices)) == 1;

    if (has_node && has_adj_node && has_match_node)
    return; // not dominated
  }

  // Edge [a,v]  is dominated, so delete it
  *(adj_matrix + idx_1) = 0;
  *(adj_matrix + idx_2) = 0;
  (*counter)++;
}

// dev array of long with size - num_edges (threads store num of deltes there)
__global__
void DeleteDominatedEdges(short *adj_matrix, Edge *edge_list, 
              const size_t num_vertices, const size_t num_edges) {
  
  int my_idx = threadIdx.x + blockIdx.x * blockDim.x;
  Edge edge_p = edge_list[my_idx];
  size_t counter = 0;

  // GetAllEdgesDominatedBy_(edge, &count_del_nodes, num_vertices, num_edges,
  //                           adj_matrix_D, edge_list_D);
  {
    auto ep = edge_p.node1;
    auto fp = edge_p.node2;
  
    size_t idx_p = (edge_p.node1 + edge_p.node2 * num_vertices);
    if (*(adj_matrix + idx_p) == 1) // only active edges
    {
      for (size_t i = 0; i < num_edges; ++i) {
        Edge edge_q = edge_list[my_idx];

        // confirm if it is still active
        size_t idx_q = (edge_q.node1 + edge_q.node2 * num_vertices);
        if (*(adj_matrix + idx_q) == 0 || *(adj_matrix + idx_p) == 0)
          continue; // not an active node
    
        // don't find matches for current edge
        if ((edge_p.node1 == edge_q.node1 || edge_p.node1 == edge_q.node2) &&
            (edge_p.node2 == edge_q.node1 || edge_p.node2 == edge_q.node2)) {
          continue;
        }
    
        size_t a, u, v;
        auto eq = edge_q.node1;
        auto fq = edge_q.node2;
    
        if (ep == eq) {
          u = edge_p.node2;
          a = edge_p.node1;
          v = edge_q.node2;
          RemoveDominatedEdge({a, u, v}, &counter, adj_matrix, num_vertices);
          continue;
        }
    
        if (ep == fq) {
          u = edge_p.node2;
          a = edge_p.node1;
          v = edge_q.node1;
          RemoveDominatedEdge({a, u, v}, &counter, adj_matrix, num_vertices);
          continue;
        }
    
        if (fp == eq) {
          u = edge_p.node1;
          a = edge_p.node2;
          v = edge_q.node2;
          RemoveDominatedEdge({a, u, v}, &counter, adj_matrix, num_vertices);
          continue;
        }
    
        if (fp == fq) {
          u = edge_p.node1;
          a = edge_p.node2;
          v = edge_q.node1;
          RemoveDominatedEdge({a, u, v}, &counter, adj_matrix, num_vertices);
          continue;
        }
      }
    }
  }    
}

inline void CUDA_Process(short *adj_matrix,
               const std::unordered_set<Edge, Edge::HashFunction> &edge_list,
               const size_t num_vertices, const size_t num_edges, 
               std::string output_file) {
  // std::cout << ToStingAdjMatrix_2D(adj_matrix, num_vertices) << '\n';
  // size_t old_edge_count = num_edges;

  size_t size =  num_vertices * num_vertices;
  std::unordered_set<Edge, Edge::HashFunction> edge_list_new;
  std::vector<std::vector<int>> adj_list(num_vertices, std::vector<int>());

  // ////////////////   THE PLAN //////////////////
  // 
  // Device Variables: adj_matrix_D, edge_list_D
  // Kernel Arguments: num_vertices, num_edges
  // 
  // 1. create Device variables
  // 2. copy adj_matrix to adj_matrix_D
  // 3. convert Set container - edge_list to Array of Edges
  // 4. copy Array of Edges to edge_list_D
  // 
  // ////////////////           //////////////////

  // Allocate memory on the device
  dev_array<short> adj_matrix_D(size);
  dev_array<Edge> edge_list_D(num_edges);

  adj_matrix_D.set(adj_matrix, size);
  edge_list_D.set(edge_list);
  
  // Test_Kernel<<<1, 1>>>(adj_matrix_D.getData(), 
  //                       edge_list_D.getData(),
  //                       num_vertices, num_edges);
  const int ThreadPerBlock = 128;
  const int NumOfBlocks = (int)ceil(num_edges/ThreadPerBlock);

  DeleteDominatedEdges<<<NumOfBlocks, ThreadPerBlock>>>(adj_matrix_D.getData(), 
                        edge_list_D.getData(),
                        num_vertices, num_edges);
  
  cudaDeviceSynchronize();

  short *result_adj_matrix = new short[size];
  adj_matrix_D.get(result_adj_matrix, size);
  cudaDeviceSynchronize();

  // auto start_time = system_clock::now();
  // size_t deleted_edge_count = 
  //                     FindAndDeleteDominatedEdges(adj_matrix, edge_list, 
  //                                                 num_vertices, num_edges);
  // auto end_time = system_clock::now();

  
  // TODO: construct a new adj_list
  auto edge_count_new = RegenerateEdgeList(result_adj_matrix, edge_list_new, adj_list, num_vertices, num_edges);
  // Print generated graph to file
  std::string adj_list_str = ToStingAdjList_2D(adj_list, num_vertices);
  std::thread threadObj(WriteStringToFile,output_file, adj_list_str);
  threadObj.detach();

  // WriteStringToFile(input_file, adj_list_str);

  // std::cout <<"\n"	<< "The New Graph: \n"
  //           // << ToStingAdjMatrix_2D(adj_matrix, num_vertices)
  //           << "\n"
  //           << "Previous Num of Edges : " << edge_list.size() << "\n"
  //           << "Num Deleted of Edges : "
  //           << deleted_edge_count << "\n";

  // auto elapsed_seconds = chrono::duration<float>(end_time - start_time).count();

  // std::cout << "DURATION: "<< elapsed_seconds << " seconds.\n";
  size_t deleted_edge_count = num_edges - edge_count_new;
  std::cout << "\n"
            // << "  Num. of Vertices"
            // << "      Num. of Edges"
            // << "      Num. of Deleted Edges"
            // << "      Duration (Serial)"
            << "     " << num_vertices << "                " << num_edges
            << "                  " << deleted_edge_count
            // << "                    " << elapsed_seconds 
            << "\n";

  // TODO:
  // write old adj_list to file (before calling thois function)
  // make sure that object is now discarded
  // create new adj_list
  // purge currrent edge_list
  // Now : RegenerateEdgeList(...)
  // print edge count to validate deletions
  // print edge list

  // std::free(edge_list);

  // Edge *edge_list_1 =
  //     (Edge *)std::malloc(num_edges * sizeof *edge_list_1);
  /*
    // Edge *edge_list_1 = new Edge[size]();
    std::unordered_set<Edge, Edge::HashFunction> edge_list_1;

    // if (adj_matrix == NULL || edge_list == NULL)
    //   throw runtime_error("Malloc Error !!!");

    int confirm_count = RegenerateEdgeList(adj_matrix, edge_list_1,
    num_vertices, num_edges); std::cout << "New Edge List size is "<<
    confirm_count << "\n";

    // std::free(edge_list_1);
  */
}

#endif

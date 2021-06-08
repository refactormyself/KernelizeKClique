#include <numeric>
#include "../include/graph.hpp"

using namespace std;

void graph::UpdateResult(std::stringstream &result_stream, const std::string& process_type,
                         size_t num_vert, size_t num_edges, size_t deleted_edges, size_t deleted_vertices,
                         double elapsed_seconds) {
    result_stream << process_type << std::fixed
                  << "," << num_vert
                  << "," << num_edges
                  << "," << deleted_edges
                  << "," << deleted_vertices
                  << "," << elapsed_seconds << "\n";
}

void graph::PrintResultHeader(std::stringstream &result_stream, bool to_iostream) {

    if (to_iostream) {
        std::cout << "\n"
                   << "Process Type"
                   << "          Num. of Vertices"
                   << "      Num. of Edges"
                   << "      Num. of Deleted Edges"
                   << "      Num. of Deleted Vertices"
                   << "           Duration"
                   << "\n     ";
    }

    result_stream  << "Process-Type,"
               << "VertexCount,"
               << "EdgeCount,"
               << "DeletedEdges,"
               << "DeletedVertices,"
               << "Duration"
               << "\n";
}

void graph::ExitWithProperUseMessage() {
    printf("LAUNCH ERROR: \n"
           "HERE IS HOW TO USE THIS SOFTWARE:\n\n "
           "- read sample graph files and generate some random sized graphs and show the output: \n"
           "   $ dominate\n"
           "- generate 1 random graphs, given num_vertex & num_edges and show the output: \n"
           "   $ dominate <num_vertex> <num_edges>\n"
           "- Read a graph from file, given num_vertex and show the output: \n"
           "   $ dominate <num_vertex> <file_path>\n"
           "\n\n");
    exit(1);
}

// This will generate a connected graph
void graph::GenerateRandomGraph(std::unique_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges) {
  // assert maximum possible edges n-Choose-2
  assert(num_edges <= (num_vertices * (num_vertices - 1) / 2));
  std::unordered_set<Edge, Edge::HashFunction> edge_list{};

  // Assign all vertices to an edge: Create the Critical Path (MST)
  for (size_t vertex1 = 0; vertex1 < num_vertices; vertex1++) {
    srand((unsigned)time(nullptr));
    size_t rand_vrtx = (vertex1 == 0) ? 1 : vertex1; // node 0 cannot connect to itself

    // pick from 0 .. vertex1. But avoid a loop
    while (rand_vrtx == vertex1) {
        rand_vrtx = rand() % (vertex1 + 1);
    }

    // compute their location within the adj_matrix
    size_t idx_1 = vertex1 + (rand_vrtx * num_vertices);
    size_t idx_2 = rand_vrtx + (vertex1 * num_vertices);

    if (idx_1 >= num_vertices * num_vertices ||
        idx_2 >= num_vertices * num_vertices)
      throw out_of_range("... attempt to write out range of AdjMatrix");

    // mark the edge on the Adjacency Matrix
    *(adj_matrix.get() + idx_1) = 1;
    *(adj_matrix.get() + idx_2) = 1;

    IsAdjacent(vertex1, rand_vrtx, adj_matrix.get(), num_vertices, false);
    edge_list.insert({static_cast<long>(vertex1), static_cast<long>(rand_vrtx)});
  }

  // Now assign the  rest edges randomly

  // ASSUMPTIONS:
  // Graph is connected
  // adj_matrix is initialised to 0
  // No loop or parallel edge allowed

  srand((unsigned)time(nullptr));

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
      *(adj_matrix.get() + idx_1) = 1;
      *(adj_matrix.get() + idx_2) = 1;

      IsAdjacent(node1_idx, node2_idx, adj_matrix.get(), num_vertices, false);

      edge_list.insert({node1_idx, node2_idx});
  }
}

// Populates the Adjacency Matrix and EdgeList using data from file
size_t graph::LoadAdjMatrixFromFile(const std::string &file_path, std::unique_ptr<short[]> &adj_matrix,
                                      const size_t num_vertices) {

  std::string file = ReadStringFromFile_Fast(file_path);
  std::unordered_set<graph::Edge, graph::Edge::HashFunction> edge_list{};

  std::istringstream file_stream;
  file_stream.str(file);

  size_t value{0}; //, edge_count{0};
  size_t row{0}, col{0};

  while (file_stream >> value) {
    if (value == 1) {
      if (col == row) // avoid a loop edge
        throw range_error("Loop found  in Graph data. Loop and Parallel edges "
                          "are not supported");

      *(adj_matrix.get() + (col + row * num_vertices)) = 1;

      size_t idx_1 = col + (row * num_vertices);
      size_t idx_2 = row + (col * num_vertices);

      if (idx_1 >= num_vertices * num_vertices ||
          idx_2 >= num_vertices * num_vertices)
        throw out_of_range("Attempt to write out range of *AdjMatrix");

      // Adjacency Matrix
      *(adj_matrix.get() + idx_1) = 1;
      *(adj_matrix.get() + idx_2) = 1;

      edge_list.insert({static_cast<long>(row), static_cast<long>(col)});
    } else {
      *(adj_matrix.get() + (col + row * num_vertices)) = 0;
    }

    col++;
    if (col == num_vertices) {
      // reset edge idx and goto next node
      col = 0;
      row++;
    }
  }

  return edge_list.size();
}

short graph::GetValue(const std::shared_ptr<short[]>& matrix_start_ptr,
                        size_t matrix_size, size_t row_id, size_t col_id){
    return *(matrix_start_ptr.get() + col_id + row_id * matrix_size);
}

std::string graph::ToStingAdjMatrix(const short *adj_matrix, size_t &num_vertices,
                                      size_t &num_edges, const std::string& dim) {
    size_t vertices = 0;
    size_t edges =0;
    bool is1D;
    if (dim == "1D")
        is1D = true;
    else if (dim == "2D")
        is1D = false;
    else
        throw invalid_argument("Wrong dimension! use: '1D' or '2D'");

    std::stringstream str_stream;
    for (size_t i = 0; i < (num_vertices); i++) {
        auto begin = adj_matrix + (num_vertices * i);
        if(0 == std::accumulate(begin, begin + num_vertices, /*init*/0))
            continue; // skip isolated vertex

        for (size_t j = 0; j < num_vertices; j++) {
            //count edges
            if((i!=j) && IsAdjacent(i, j, adj_matrix, num_vertices, true))
                ++edges;

            // ToDo: the check for (i==j) is a hack to avoid loop edge.
            //  Test the Random Graph Generator properly. THIS SHOULD NOT HAPPEN
            if (is1D){
                str_stream << ((i==j) ? 0: *(begin + j)) << '\n';
            } else{
                str_stream << ((i==j) ? 0: *(begin + j))  << ' ';
                if ((j + 1) % num_vertices == 0)
                    str_stream << '\n';
            }
        }
        ++vertices;
    }

    //update with correct values and return string
    num_vertices = vertices;
    num_edges = edges;
    return str_stream.str();
}

inline size_t RegenerateEdgeList(const short *adj_matrix, std::unordered_set<graph::Edge,
                                 graph::Edge::HashFunction> &edge_list,
                                 std::vector<std::vector<int>> &adj_list,
                                 size_t num_vertices)
{
    for(size_t row=0; row < num_vertices; ++row) {
        for(size_t col=0; col < num_vertices; ++col){
            if (1 == *(adj_matrix + (col + (row * num_vertices))))
            {
                auto is_inserted = edge_list.insert({static_cast<long>(col), static_cast<long>(row)}).second;
                if (is_inserted)
                    adj_list[row].push_back(col);
            }
        }
    }

    return edge_list.size();
}
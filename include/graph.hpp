#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "cpp_util.hpp"

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

namespace graph {

    struct Edge{
        long node1;
        long node2;
        Edge():node1(-1), node2(-1){}
        Edge(long x1, long x2):node1(x1), node2(x2){}

        bool operator==(const Edge& otherEdge) const
        {
            return (this->node1 == otherEdge.node1 && this->node2 == otherEdge.node2) ||
                   (this->node1 == otherEdge.node2 && this->node2 == otherEdge.node1);
        }

        bool operator!=(const Edge& otherEdge) const
        {
            return !(*this == otherEdge);
        }

        bool isActive(const std::shared_ptr<short[]>& adj_matrix, size_t num_vertices) const {
            size_t idx_p = (this->node1 + this->node2 * num_vertices);
            return (*(adj_matrix.get() + idx_p) == 1);
        }

        struct HashFunction
        {
            size_t operator()(const Edge& edge) const
            {
                size_t xHash = std::hash<int>()(std::max(edge.node1, edge.node2));
                size_t yHash = std::hash<int>()(std::min(edge.node1, edge.node2)) << 1;
                return xHash ^ yHash;
            }
        };
    };

    struct Match{
        size_t a;
        size_t u;
        size_t v;
    };

    struct SetOf4v {
        short node1;
        short node2;
        short node3;
        short node4;
    };

    void GenerateRandomGraph(std::unique_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges);

    size_t LoadAdjMatrixFromFile(const std::string &file_path, std::unique_ptr<short[]> &adj_matrix,
                                 size_t num_vertices);

    void PrintResultHeader(std::stringstream &result_stream, bool to_iostream= true);

    void ExitWithProperUseMessage();

    short GetValue(const std::shared_ptr<short[]>& matrix_start_ptr, size_t matrix_size, size_t row_id, size_t col_id);

    // Is Node x adjacent to Node y
    inline bool IsAdjacent(size_t x, size_t y, const short *adj_matrix, size_t num_vert, bool no_assert= false) {
        bool xy = (1 == *(adj_matrix + (x + y * num_vert)));
        bool yx = (1 == *(adj_matrix + (y + x * num_vert)));

        //    bool test = (xy == yx);
        //    assert(("Adjacent Matrix is in a Malformed state"),test);

        if (!no_assert && !((xy && yx) || (!xy && !yx)))
            throw std::domain_error("Adjacent Matrix is in a Malformed state");

        return xy;
    }
    // Is N(x) a subset of N(y); where N(a) means Neighbors of node a
    inline bool  IsNxSubsetNy(size_t x, size_t y, const short * adj_matrix, size_t num_vert){
        for (size_t i = 0; i < num_vert; ++i) {
            auto x_val = *(adj_matrix + (i + x * num_vert));
            auto y_val = *(adj_matrix + (i + y * num_vert));
            if ((y_val - x_val) < 0)
                return false;
        }
        return true;
    }

// Returns a mapping row representing the intersection of N(x) and N(y):
// each element maps to each vertex, 0 is absent; 1 is present in the set
   inline std::unique_ptr<short[]> NxIntersectNy(size_t x, size_t y, const short *adj_matrix, size_t num_vert) {

        std::unique_ptr<short[]> intersect = std::unique_ptr<short[]>(new short[num_vert]);
        for (size_t i = 0; i < num_vert; ++i) {
            short x_val = *(adj_matrix + (i + x * num_vert));
            short y_val = *(adj_matrix + (i + y * num_vert));
            auto result = x_val & y_val;
            *(intersect.get() + i) = result;
        }
        return intersect;
    }

    inline bool IsxASubsetOfxB(size_t num_vertices, const std::unique_ptr<short[]> &xA,
                                        const std::unique_ptr<short[]> &xB) {
        bool is_a_subset = true;
        // Is "N(X) intersect N(Y)" a subset of "N(A) intersect N(B)"
        for (size_t i = 0; i < num_vertices; ++i) {
            if (*(xB.get() + i) - *(xA.get() + i) < 0) {
                is_a_subset = false;
                break;
            }
        }
        return is_a_subset;
    }

    std::string ToStingAdjMatrix(const short *adj_matrix, size_t &num_vertices,
                                 size_t &num_edges, const std::string& dim="1D");

    void
    UpdateResult(std::stringstream &result_stream, const std::string& process_type, size_t num_vert, size_t num_edges,
                 size_t deleted_edges, size_t deleted_vertices, double elapsed_seconds);
}
#endif //GRAPH_HPP

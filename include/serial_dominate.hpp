#ifndef SERIAL_DOM_HPP
#define SERIAL_DOM_HPP

#include "graph.hpp"
#include <bits/stdc++.h>

namespace sd {
    void EdgeDom1(std::shared_ptr<short[]> adj_matrix, size_t num_vertices, size_t num_edges,
                  std::stringstream &result_stream);

    void Algorithm1(const std::shared_ptr<short[]>& adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream);

    void Algorithm2(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream);

    void Algorithm3(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream);

    void Algorithm3_v2(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                       std::stringstream &result_stream);

    bool IsxASubsetOfxB(size_t num_vertices, const std::unique_ptr<short[]> &xA,
                        const std::unique_ptr<short[]> &xB);

    int FindDeleteDominatedEdges(const std::shared_ptr<short[]> &adj_matrix,
                                 size_t num_vert, short nodeX, int nodeU);
}

#endif //SERIAL_DOM_HPP

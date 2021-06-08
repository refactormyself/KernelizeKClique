#ifndef OMP_DOMINATE_HPP
#define OMP_DOMINATE_HPP

#include "graph.hpp"

namespace ompd {

    void EdgeDom1(std::shared_ptr<short[]> adj_matrix, size_t num_vertices, size_t num_edges,
                  std::stringstream &result_stream);

    void Algorithm1(const std::shared_ptr<short[]>& adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream);

    void Algorithm2(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream);

//    void Algorithm3(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges);

//    void Algorithm1perm(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges);

}

#endif //OMP_DOMINATE_HPP

#include "omp_dominate.hpp"

namespace chrono = std::chrono;
using chrono::system_clock;

inline int InvalidateDominatedEdge(graph::Match match,
                                   const std::shared_ptr<short[]> &adj_matrix,
                                   size_t num_vertices) {
    size_t idx_1 = (match.a + match.v * num_vertices);
    size_t idx_2 = (match.v + match.a * num_vertices);

    if (*(adj_matrix.get() + idx_1) == 0 || *(adj_matrix.get() + idx_2) == 0)
        return 0; // matched edge already invalidated

    for (size_t i = 0; i < num_vertices; i++) {
        bool has_node = *(adj_matrix.get() + (i + match.a * num_vertices)) == 1;
        bool has_adj_node = *(adj_matrix.get() + (i + match.u * num_vertices)) == 0;
        bool has_match_node = *(adj_matrix.get() + (i + match.v * num_vertices)) == 1;
        if (has_node && has_adj_node && has_match_node)
            return 0; // not dominated
    }

    // Edge [a,v]  is dominated, so delete it
    *(adj_matrix.get() + idx_1) = 0;
    *(adj_matrix.get() + idx_2) = 0;
    return 1;
}

void ompd::EdgeDom1(std::shared_ptr<short[]> adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream) {

    size_t num_deleted_edges = 0;
    std::unordered_set<graph::Edge, graph::Edge::HashFunction> edge_list_new;
    std::shared_ptr<short[]> matrix = std::move(adj_matrix);

    // TIMING STARTS
    auto start_time = system_clock::now();
#pragma omp parallel default(none) shared(matrix, num_deleted_edges) firstprivate(num_vertices)
    {
#pragma omp for simd collapse(2) reduction(+:num_deleted_edges)
        for (size_t row = 0; row < num_vertices; ++row) {
            for (size_t col = 0; col < num_vertices; ++col) {
                short value = graph::GetValue(matrix, num_vertices, row, col);
                if (value == 1) {
                    graph::Edge edge_p(row, col);

                    for (size_t rowi = 0; rowi < num_vertices; ++rowi) {
                        for (size_t coli = 0; coli < num_vertices; ++coli) {
                            short valuei = graph::GetValue(matrix, num_vertices, rowi, coli);
                            if (valuei == 1) {
                                graph::Edge edge_q(rowi, coli);

                                // don't find matches for current edge
                                if (edge_p == edge_q)
                                    continue;

                                size_t a, u, v;
                                if (edge_p.node1 == edge_q.node1) {
                                    u = edge_p.node2;
                                    a = edge_p.node1;
                                    v = edge_q.node2;
                                    num_deleted_edges += InvalidateDominatedEdge({a, u, v}, matrix, num_vertices);
                                    continue;
                                }

                                if (edge_p.node1 == edge_q.node2) {
                                    u = edge_p.node2;
                                    a = edge_p.node1;
                                    v = edge_q.node1;
                                    num_deleted_edges += InvalidateDominatedEdge({a, u, v}, matrix, num_vertices);
                                    continue;
                                }

                                if (edge_p.node2 == edge_q.node1) {
                                    u = edge_p.node1;
                                    a = edge_p.node2;
                                    v = edge_q.node2;
                                    num_deleted_edges += InvalidateDominatedEdge({a, u, v}, matrix, num_vertices);
                                    continue;
                                }

                                if (edge_p.node2 == edge_q.node2) {
                                    u = edge_p.node1;
                                    a = edge_p.node2;
                                    v = edge_q.node1;
                                    num_deleted_edges += InvalidateDominatedEdge({a, u, v}, matrix, num_vertices);
                                    continue;
                                }
                            }

                        }
                    }
                }
            }
        }
    } //END OF OMP PARALLEL

    // TIMING ENDS
    auto end_time = system_clock::now();

    auto elapsed_seconds = chrono::duration<float>(end_time - start_time).count();
    const long deleted_vertices = 0;
    auto process_type = "OpenMP EdgeDom v1";
    std::cout << "\n"
              << process_type
              << "          " << num_vertices
              << "                   " << num_edges
              << "                  " << num_deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    graph::UpdateResult(result_stream,process_type, num_vertices,
                        num_edges, num_deleted_edges, deleted_vertices, elapsed_seconds);
}

void ompd::Algorithm1(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                      std::stringstream &result_stream) {
    int deleted_vertices = 0;
    int deleted_edges = 0;

    // TIMING STARTS
    auto start_time = std::chrono::system_clock::now();

#pragma omp parallel default(none) shared(adj_matrix, deleted_edges, deleted_vertices) firstprivate(num_vertices)
    {
#pragma omp for simd  reduction(+:deleted_edges) reduction(+:deleted_vertices)
        for (size_t vertex = 0; vertex < num_vertices; ++vertex) {
            auto neigh_start = adj_matrix.get() + (vertex * num_vertices);

            for (size_t neighbor = 0; neighbor < num_vertices; ++neighbor) {
                if ((vertex == neighbor) || *(neigh_start + neighbor) == 1)
                    continue;

                if (graph::IsNxSubsetNy(vertex, neighbor, adj_matrix.get(), num_vertices)) {
                    // delete vertex (make it an isolated vertex)
                    for (size_t i = 0; i < num_vertices; ++i) {
                        if (*(neigh_start + i) == 1) {
                            *(neigh_start + i) = 0;
                            *(adj_matrix.get() + (i * num_vertices) + vertex) = 0;
                            ++deleted_edges;
                        }
                    }
                    ++deleted_vertices;
                    break;
                }
            }
        }

    }
    // TIMING ENDS
    auto end_time = std::chrono::system_clock::now();
    auto elapsed_seconds = std::chrono::duration<float>(end_time - start_time).count();

//    graph::PrintResultHeader();
    auto process_type = "OpenMP DOM. ALG-1";
    std::cout << "\n"
              << process_type
              << "          " << num_vertices
              << "                   " << num_edges
              << "                  " << deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    graph::UpdateResult(result_stream,process_type, num_vertices,
                        num_edges, deleted_edges, deleted_vertices, elapsed_seconds);
}

void ompd::Algorithm2(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                      std::stringstream &result_stream) {
    int deleted_vertices = 0;
    int deleted_edges = 0;

    // TIMING STARTS
    auto start_time = std::chrono::system_clock::now();

#pragma omp parallel default(none) shared(adj_matrix, deleted_edges) firstprivate(num_vertices)
    {
#pragma omp for simd collapse(2) reduction(+:deleted_edges)
        for (size_t vertexA = 0; vertexA < num_vertices; ++vertexA) {
            for (size_t vertexU = 0; vertexU < num_vertices; ++vertexU) {
                if (!graph::IsAdjacent(vertexA, vertexU, adj_matrix.get(), num_vertices, true))
                    continue;

                for (size_t vertexB = 0; vertexB < num_vertices; ++vertexB) {

                    if ((vertexB == vertexA) // Condition 1: vertices must be unique and adjacent
                        || !graph::IsAdjacent(vertexU, vertexB, adj_matrix.get(), num_vertices, true)) {
                        continue;
                    }

                    auto nA_int_nU = graph::NxIntersectNy(vertexA, vertexU, adj_matrix.get(), num_vertices);
                    auto nU_int_nB = graph::NxIntersectNy(vertexU, vertexB, adj_matrix.get(), num_vertices);

                    // Condition 2: Is Vertex B is NOT an element of N(A) intersect N(U)
                    if (*(nA_int_nU.get() + vertexB) == 0) {
                        // Condition 3: Is "N(A) intersect N(U)" a subset of "N(U) intersect N(V)"
                        bool is_a_subset = graph::IsxASubsetOfxB(num_vertices, nA_int_nU, nU_int_nB);

                        if (is_a_subset) {
                            // hence, {u,b} dominates {a,u}. so {a,u} can be deleted
                            *(adj_matrix.get() + (vertexA * num_vertices) + vertexU) = 0;
                            *(adj_matrix.get() + (vertexU * num_vertices) + vertexA) = 0;
                            ++deleted_edges;
                            break; // Condition 3 failed: go pick another vertexU
                        }
                    } else break; // Condition 2 failed: go pick another vertexU
                }
            }
        }
    }
    // TIMING ENDS
    auto end_time = std::chrono::system_clock::now();
    auto elapsed_seconds = std::chrono::duration<float>(end_time - start_time).count();

//    graph::PrintResultHeader();
    auto process_type = "OpenMP DOM. ALG-2";
    std::cout << "\n"
              << process_type
              << "          " << num_vertices
              << "                   " << num_edges
              << "                  " << deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    graph::UpdateResult(result_stream,process_type, num_vertices,
                        num_edges, deleted_edges, deleted_vertices, elapsed_seconds);
}

#include <numeric>
#include "../include/serial_dominate.hpp"

graph::SetOf4v CopyFromArray(const short *value){
    return {*(value),*(value + 1),  *(value + 2), *(value + 3)};
}

long factorial(long n){
    return (n==0) || (n==1) ? 1 : n* factorial(n-1);
}

inline int DeleteDominatedEdge(graph::SetOf4v setOf4V, short *adj_matrix, size_t num_vert) {
    bool is_edge_1_3 = graph::IsAdjacent(setOf4V.node1, setOf4V.node3, adj_matrix, num_vert, true);
    bool is_edge_2_3 = graph::IsAdjacent(setOf4V.node2, setOf4V.node3, adj_matrix, num_vert, true);
    bool condition1 = is_edge_1_3 && is_edge_2_3; // both cannot be an edge
    if (condition1)
        return 0;

    bool is_edge_1_4 = graph::IsAdjacent(setOf4V.node1, setOf4V.node4, adj_matrix, num_vert, true);
    bool is_edge_2_4 = graph::IsAdjacent(setOf4V.node2, setOf4V.node4, adj_matrix, num_vert, true);
    bool condition2 = is_edge_1_4 && is_edge_2_4; // both cannot be an edge
    if (condition2)
        return 0;

    auto n1_int_n2 = graph::NxIntersectNy(setOf4V.node1, setOf4V.node2, adj_matrix, num_vert);
    auto n3_int_n4 = graph::NxIntersectNy(setOf4V.node3, setOf4V.node4, adj_matrix, num_vert);

    // Is "N(A) intersect N(U)" a subset of "N(U) intersect N(V)" ?
    bool condition3 = graph::IsxASubsetOfxB(num_vert, n1_int_n2, n3_int_n4);

    if (condition3){
        // Hence, {node3, node4} dominates {node1, node2}
        *(adj_matrix + (setOf4V.node1 * num_vert) + setOf4V.node2) = 0;
        *(adj_matrix + (setOf4V.node2 * num_vert) + setOf4V.node1) = 0;
        return 1;
    }
    return 0;
}

inline void InvalidateDominatedEdge(graph::Match match, size_t &counter,
                                    const std::shared_ptr<short[]>& adj_matrix,
                                    size_t num_vertices) {
    size_t idx_1 = (match.a + match.v * num_vertices);
    size_t idx_2 = (match.v + match.a * num_vertices);

    if (*(adj_matrix.get() + idx_1) == 0 || *(adj_matrix.get() + idx_2) == 0)
        return; // matched edge already invalidated

    for (size_t i = 0; i < num_vertices; i++) {
        bool has_node = *(adj_matrix.get() + (i + match.a * num_vertices)) == 1;
        bool has_adj_node = *(adj_matrix.get() + (i + match.u * num_vertices)) == 0;
        bool has_match_node = *(adj_matrix.get() + (i + match.v * num_vertices)) == 1;
        if (has_node && has_adj_node && has_match_node)
            return; // not dominated
    }

    // Edge [a,v]  is dominated, so delete it
    *(adj_matrix.get() + idx_1) = 0;
    *(adj_matrix.get() + idx_2) = 0;
    counter++;
}

inline size_t DeleteEdgesDominatedBy(const graph::Edge &edge_p, size_t num_vertices,
                                       const std::shared_ptr<short[]> &adj_matrix) {
    size_t counter = 0;
    for (size_t row = 0; row < num_vertices; ++row) {
        for (size_t col = 0; col < num_vertices; ++col) {
            short value = graph::GetValue(adj_matrix, num_vertices, row, col);
            if (value == 1) {
                graph::Edge edge_q(row, col);

                // don't find matches for current edge
                if (edge_p == edge_q) {
                    continue;
                }

                size_t a, u, v;
                if (edge_p.node1 == edge_q.node1) {
                    u = edge_p.node2;
                    a = edge_p.node1;
                    v = edge_q.node2;
                    InvalidateDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
                    continue;
                }

                if (edge_p.node1 == edge_q.node2) {
                    u = edge_p.node2;
                    a = edge_p.node1;
                    v = edge_q.node1;
                    InvalidateDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
                    continue;
                }

                if (edge_p.node2 == edge_q.node1) {
                    u = edge_p.node1;
                    a = edge_p.node2;
                    v = edge_q.node2;
                    InvalidateDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
                    continue;
                }

                if (edge_p.node2 == edge_q.node2) {
                    u = edge_p.node1;
                    a = edge_p.node2;
                    v = edge_q.node1;
                    InvalidateDominatedEdge({a, u, v}, counter, adj_matrix, num_vertices);
                    continue;
                }
            }
        }
    }
    return counter;
}

void sd::EdgeDom1(std::shared_ptr<short[]> adj_matrix, size_t num_vertices, size_t num_edges,
                  std::stringstream &result_stream) {

    size_t num_deleted_edges = 0;
    std::unordered_set<graph::Edge, graph::Edge::HashFunction> edge_list_new;
    std::shared_ptr<short[]> matrix = std::move(adj_matrix);

    // TIMING STARTS
    auto start_time = std::chrono::system_clock::now();

    for (size_t row = 0; row < num_vertices; ++row) {
        for (size_t col = 0; col < num_vertices; ++col) {
            short value = graph::GetValue(matrix, num_vertices, row, col);
            if (value == 1){
                graph::Edge edge(row, col);
                num_deleted_edges += DeleteEdgesDominatedBy(edge, num_vertices, matrix);
            }
        }
    }
    // TIMING ENDS
    auto end_time = std::chrono::system_clock::now();

    auto elapsed_seconds = std::chrono::duration<double>(end_time - start_time).count();
    const long deleted_vertices = 0;
    std::cout << "\n"
              << "SERIAL EdgeDom v1" << std::fixed
              << "          " << num_vertices
              << "                   " << num_edges
              << "                  " << num_deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    result_stream << "SERIAL EdgeDom v1," << std::fixed
                  << num_vertices
                  << "," << num_edges
                  << "," << num_deleted_edges
                  << "," << deleted_vertices
                  << "," << elapsed_seconds << "\n";

}

void sd::Algorithm1(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream) {
    int deleted_vertices = 0;
    int deleted_edges = 0;

    // TIMING STARTS
    auto start_time = std::chrono::system_clock::now();

    for (size_t vertex = 0; vertex < num_vertices; ++vertex) {
        auto neigh_start = adj_matrix.get() + (vertex * num_vertices);

        for (size_t neighbor = 0; neighbor < num_vertices; ++neighbor) {
            if ((vertex == neighbor) || *(neigh_start + neighbor) == 1)
                continue;

            if (graph::IsNxSubsetNy(vertex, neighbor, adj_matrix.get(), num_vertices)){
                // delete vertex (make it an isolated vertex)
                for (size_t i = 0; i < num_vertices; ++i) {
                    if (*(neigh_start + i) == 1){
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
    // TIMING ENDS
    auto end_time = std::chrono::system_clock::now();
    auto elapsed_seconds = std::chrono::duration<double>(end_time - start_time).count();

//    graph::PrintResultHeader();

    std::cout << "\n"
              << "SERIAL DOM. ALG-1"  << std::fixed
              << "          " << num_vertices
              << "                   " << num_edges
              << "                  " << deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    result_stream << "SERIAL DOM. ALG-1," << std::fixed
                  << num_vertices
                  << "," << num_edges
                  << "," << deleted_edges
                  << "," << deleted_vertices
                  << "," << elapsed_seconds << "\n";
}

void sd::Algorithm2(const std::shared_ptr<short[]> &adj_matrix, size_t num_vertices, size_t num_edges,
                    std::stringstream &result_stream) {
    int deleted_vertices = 0;
    int deleted_edges = 0;

    // TIMING STARTS
    auto start_time = std::chrono::system_clock::now();

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
                if (*(nA_int_nU.get() + vertexB) == 0){
                    // Condition 3: Is "N(A) intersect N(U)" a subset of "N(U) intersect N(V)"
                    bool is_a_subset = graph::IsxASubsetOfxB(num_vertices, nA_int_nU, nU_int_nB);

                    if (is_a_subset){
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

    // TIMING ENDS
    auto end_time = std::chrono::system_clock::now();
    auto elapsed_seconds = std::chrono::duration<double>(end_time - start_time).count();

//    graph::PrintResultHeader();

    std::cout << "\n"
              << "SERIAL DOM. ALG-2" << std::fixed
              << "          " << num_vertices
              << "                   " << num_edges
              << "                  " << deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    result_stream << "SERIAL DOM. ALG-2," << std::fixed
                  << num_vertices
                  << "," << num_edges
                  << "," << deleted_edges
                  << "," << deleted_vertices
                  << "," << elapsed_seconds << "\n";
}

void sd::Algorithm3_v2(const std::shared_ptr<short[]> &adj_matrix, size_t num_vert, size_t num_edges,
                       std::stringstream &result_stream) {

    int deleted_edges = 0;
    int size = 2;
    int deleted_vertices = 0;

    // TIMING STARTS
    auto start_time = std::chrono::system_clock::now();

    //fills from 0 ..
    short values[num_vert];
    std::iota(values, values + num_vert, 0);

    size_t selected[size];
    for(int i=0; i < size; ++i){
        selected[i]=0;
    }

    auto nodeX = *values;
    auto nodeU = *values + 1;
    deleted_edges += FindDeleteDominatedEdges(adj_matrix, num_vert, nodeX, nodeU);

    size_t i = 0;
    int loopCount =0;
    while(i < num_vert){
        ++loopCount;
        if(selected[i] < i){
            if(i%2 == 0)
                std::swap(values[0], values[i]);
            else
                std::swap(values[selected[i]], values[i]);

            nodeX = *values;
            nodeU = *values + 1;
            deleted_edges += FindDeleteDominatedEdges(adj_matrix, num_vert, nodeX, nodeU);

            ++selected[i];
            i = 1;

        } else {
            selected[i] = 0;
            ++i;
        }
    }

    // TIMING ENDS
    auto end_time = std::chrono::system_clock::now();
    double elapsed_seconds = std::chrono::duration<double>(end_time - start_time).count();

//    graph::PrintResultHeader();

    std::cout << "\n"
              << "SERIAL DOM. ALG-3 v2" << std::fixed
              << "          " << num_vert
              << "                   " << num_edges
              << "                  " << deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    auto process_type = "SERIAL DOM. ALG-3 v2";
//    result_stream << process_type
//                  << "," << num_vert
//                  << "," << num_edges
//                  << "," << deleted_edges
//                  << "," << deleted_vertices
//                  << "," << elapsed_seconds << "\n";
    graph::UpdateResult(result_stream,process_type, num_vert,
                        num_edges, deleted_edges, deleted_vertices, elapsed_seconds);
}

int sd::FindDeleteDominatedEdges(const std::shared_ptr<short[]> &adj_matrix, size_t num_vert, short nodeX, int nodeU) {
    int deleted_edges = 0;
    bool x_u_exits = graph::IsAdjacent(nodeX, nodeU, adj_matrix.get(), num_vert, true);
    for (size_t nodeY = 0; nodeY < num_vert; ++nodeY) {
        bool not_distinctY = (nodeY - nodeX == 0) || (nodeY - nodeU == 0);
        //make sure {x,y} has not been deleted
        bool x_y_exists = graph::IsAdjacent(nodeX, nodeY, adj_matrix.get(), num_vert, true);
        if (not_distinctY || !x_y_exists) continue;

        bool y_u_exists = graph::IsAdjacent(nodeY, nodeU, adj_matrix.get(), num_vert, true);
        if (x_u_exits && y_u_exists)
            continue; //edges {x,u} AND {y,u} exist! Pick another Node Y

        for (size_t nodeV = 0; nodeV < num_vert; ++nodeV) {
            bool not_distinctV = (nodeV - nodeX == 0) || (nodeV - nodeU == 0) || (nodeV - nodeY == 0);
            //make sure {u,v} has not been deleted
            bool u_v_exists = graph::IsAdjacent(nodeU, nodeV, adj_matrix.get(), num_vert, true);
            if (not_distinctV || !u_v_exists) continue;

            bool y_v_exists = graph::IsAdjacent(nodeY, nodeV, adj_matrix.get(), num_vert, true);
            bool x_v_exists = graph::IsAdjacent(nodeX, nodeV, adj_matrix.get(), num_vert, true);
            if (y_v_exists && x_v_exists)
                continue; //edges {y,v} AND {x,v} exist! Pick another Node V

            auto nX_int_nY = graph::NxIntersectNy(nodeX, nodeY, adj_matrix.get(), num_vert);
            auto nU_int_nV = graph::NxIntersectNy(nodeU, nodeV, adj_matrix.get(), num_vert);

            // Is "N(X) intersect N(Y)" a subset of "N(U) intersect N(V)" ?
            bool condition3 = graph::IsxASubsetOfxB(num_vert, nX_int_nY, nU_int_nV);

            if (condition3){
                // Hence, {u, v} dominates {x, y}
                *(adj_matrix.get() + (nodeX * num_vert) + nodeY) = 0;
                *(adj_matrix.get() + (nodeY * num_vert) + nodeX) = 0;
                ++deleted_edges;
                break;
            }
        }
    }
    return deleted_edges;
}

void sd::Algorithm3(const std::shared_ptr<short[]> &adj_matrix, size_t num_vert, size_t num_edges,
                    std::stringstream &result_stream) {

    int deleted_edges = 0;
    int size = 4;
    int deleted_vertices = 0;

    // TIMING STARTS
    auto start_time = std::chrono::system_clock::now();

    // fills from 0 .. num_vert
    short values[num_vert];
    std::iota(values, values + num_vert, 0); //fills from 0 ..

    size_t selected[size];
    for(int i=0; i < size; ++i){
        selected[i]=0;
    }

    auto vertx = CopyFromArray(values);
    if (graph::IsAdjacent(vertx.node1, vertx.node2, adj_matrix.get(), num_vert, true)
        && graph::IsAdjacent(vertx.node3, vertx.node4, adj_matrix.get(), num_vert, true)) {
        deleted_edges += DeleteDominatedEdge(vertx, adj_matrix.get(), num_vert);
    }

    size_t i = 0;
    int loopCount =0;
    while(i < num_vert){
        ++loopCount;
        if(selected[i] < i){
            if(i%2 == 0)
                std::swap(values[0], values[i]);
            else
                std::swap(values[selected[i]], values[i]);

            auto verts = CopyFromArray(values);
            if (graph::IsAdjacent(verts.node1, verts.node2,adj_matrix.get(), num_vert, true)
                    && graph::IsAdjacent(verts.node3, verts.node4,adj_matrix.get(), num_vert, true)) {
                deleted_edges += DeleteDominatedEdge(verts, adj_matrix.get(), num_vert);
            }

            ++selected[i];
            i = 1;

        } else {
            selected[i] = 0;
            ++i;
        }
    }

    // TIMING ENDS
    auto end_time = std::chrono::system_clock::now();
    auto elapsed_seconds = std::chrono::duration<double>(end_time - start_time).count();

//    graph::PrintResultHeader();

    std::cout << "\n"
            << "SERIAL DOM. ALG-3" << std::fixed
              << "          " << num_vert
              << "                   " << num_edges
              << "                  " << deleted_edges
              << "                        " << deleted_vertices
              << "                        " << elapsed_seconds << "\n";

    result_stream << "SERIAL DOM. ALG-3," << std::fixed
                  << num_vert
                  << "," << num_edges
                  << "," << deleted_edges
                  << "," << deleted_vertices
                  << "," << elapsed_seconds << "\n";
}

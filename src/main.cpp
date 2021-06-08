#include <iostream>
#include <filesystem>
#include <serial_dominate.hpp>
#include <omp_dominate.hpp>

namespace fs = std::filesystem;

void HandleArguments(char *const *argv, std::unique_ptr<short[]> &adj_matrix, size_t &num_edges, size_t &num_vert);

void Kernelize(const std::unique_ptr<short[]> &adj_matrix, size_t edges, size_t verts, std::stringstream &result_stream);

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void SaveDataInputFile(size_t num_vertices, size_t num_edges, const short *adj_matrix) {
    std::string adj_list_str = graph::ToStingAdjMatrix(adj_matrix, num_vertices, num_edges);
    std::string input_file = "graph_" + std::to_string(num_vertices)
                             + "_" + std::to_string(num_edges) + ".dat";
    const fs::path pathToSaveFile = fs::current_path().append("data/saved_input/").append(input_file);
    std::thread threadObj(WriteStringToFile, pathToSaveFile, adj_list_str);
    threadObj.detach();
}

void SaveResultsCSVFile(const std::stringstream &result_stream) {
    std::string input_file = "result_.csv";

    const fs::path pathToSaveFile = fs::current_path().append("data/saved_results/").append(input_file);
    std::thread threadObj(WriteStringToFile, pathToSaveFile, result_stream.str());
    threadObj.detach();
}

std::vector<fs::path> GetListOfFiles(const std::string& path_relative_to_binary);

int main(int argc, char *argv[]) {
    if (argc > 3 || argc == 2)
        graph::ExitWithProperUseMessage();

    std::unique_ptr<short[]> adj_matrix;
    std::unordered_set<graph::Edge, graph::Edge::HashFunction> edge_list{};
    if (argc == 3) {
        size_t num_vert = 0;
        size_t num_edges = 0;
        std::stringstream result_stream;
        HandleArguments(argv, adj_matrix, num_edges, num_vert);

        graph::PrintResultHeader(result_stream);
        Kernelize(adj_matrix, num_edges, num_vert, result_stream);
        SaveResultsCSVFile(result_stream);
    } else {
        std::stringstream result_stream;
        graph::PrintResultHeader(result_stream);
        {   // load sample data from files
            for (const auto &path :GetListOfFiles("data/input")) {
                auto filename = split(path.filename().string(), '.')[0];
                auto verts_edges = split(filename, '_');

                size_t verts = stoull(verts_edges[1]);
                size_t edges = stoull(verts_edges[2]);

                adj_matrix = std::unique_ptr<short[]>(new short[verts * verts]);
                graph::LoadAdjMatrixFromFile(path, adj_matrix, verts);
                Kernelize(adj_matrix, edges, verts, result_stream);
            }
        }

        // generate 5 random graphs
//        for (size_t i = 0; i < 10; ++i) {
//            auto num_vertices = 100 + (200 * i);
//            auto num_edges = (num_vertices * (num_vertices - 1) / 2) - (2 * num_vertices);
//
//            size_t size = num_vertices * num_vertices;
//            adj_matrix = std::unique_ptr<short[]>(new short[size]);
//
//            graph::GenerateRandomGraph(adj_matrix, num_vertices, num_edges);
//            SaveDataInputFile(num_vertices, num_edges, adj_matrix.get());
//            Kernelize(adj_matrix, num_edges, num_vertices, result_stream);
//        }

        SaveResultsCSVFile(result_stream);
    }
}

void Kernelize(const std::unique_ptr<short[]> &adj_matrix, size_t edges,
               size_t verts, std::stringstream &result_stream) {

    auto matrix = std::shared_ptr<short[]>(new short[verts * verts]);

    // Initial EdgeDom Algorithm
    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
    sd::EdgeDom1(matrix, verts, edges, result_stream);
    // reload original graph
    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
    ompd::EdgeDom1(matrix, verts, edges, result_stream);

    // Algorithm 1
    // reload original graph
    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
    sd::Algorithm1(matrix, verts, edges, result_stream);
    // reload original graph
    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
    ompd::Algorithm1(matrix, verts, edges, result_stream);

    // Algorithm 2
    // reload original graph
    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
    sd::Algorithm2(matrix, verts, edges, result_stream);
    // reload original graph
    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
    ompd::Algorithm2(matrix, verts, edges, result_stream);

    // TODO: Algorithm 3 will not converge on >8 vertices !!!
    // reload original graph
//    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
//    sd::Algorithm3(matrix, verts, edges, result_stream);

    // reload original graph
//    std::copy(adj_matrix.get(), adj_matrix.get()+(verts * verts), matrix.get());
//    sd::Algorithm3_v2(matrix, verts, edges, result_stream);

}

void HandleArguments(char *const *argv, std::unique_ptr<short[]> &adj_matrix, size_t &num_edges, size_t &num_vert) {
    num_vert= strtol(argv[1], nullptr, 10);
    if (!(IsPositiveNumber(argv[1]))) // first argument must be the num of vertices
        graph::ExitWithProperUseMessage();

    if (!IsPositiveNumber(argv[2])){ // second argument is either the file_name or num of edges
        auto size = num_vert * num_vert;
        adj_matrix = std::unique_ptr<short[]>(new short[size]);
        num_edges = graph::LoadAdjMatrixFromFile(argv[2], adj_matrix, num_vert);
    } else  {
        // edge count  and vertices count
        auto size = num_vert * num_vert;
        adj_matrix = std::unique_ptr<short[]>(new short[size]);
        num_edges = strtol(argv[1], nullptr, 10);
        graph::GenerateRandomGraph(adj_matrix, num_vert, num_edges);
    }
}

std::vector<fs::path> GetListOfFiles(const std::string& path_relative_to_binary){

    const fs::path pathToShow = fs::current_path().append(path_relative_to_binary);
    std::vector<fs::path> input_files_path = std::vector<fs::path>();

    for (const auto& entry : fs::directory_iterator(pathToShow)) {
        if (entry.is_regular_file())
            input_files_path.push_back(entry.path());
    }

    return input_files_path;
}
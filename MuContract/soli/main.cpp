#include <fstream>
#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <libSolInstru/ASTAnalyser.hpp>
#include <libUtils/Utils.hpp>

#define NODERAWFS
#ifdef NODERAWFS
#define CWD ""
#else
#include <emscripten/emscripten.h>
#define CWD "/working/"
#endif

int main(int argc, char** argv){
    std::string file_name = std::string(argv[1]);
    std::string json_ast_file_name = std::string(argv[2]);
    std::string sol_name = std::string(argv[3]);
    std::string new_name = "";
     std::string mutation_type = std::string(argv[4]);
    if (argc == 6) {
        new_name = std::string(argv[5]);
    }

    #ifndef NODERAWFS
    // mount the current folder as a NODEFS instance
    // inside of emscripten
    EM_ASM(
        FS.mkdir('/working');
        FS.mount(NODEFS, { root: '.' }, '/working');
    );
    #endif

    file_name = CWD + file_name;

    std::ifstream in_file(file_name);
    std::string new_line;
    std::stringstream ast_content;
    while (getline(in_file, new_line)) {
        ast_content << new_line << std::endl;
    }
    in_file.close();

    json_ast_file_name = CWD + json_ast_file_name;
    std::ifstream jsonast_file_stream(json_ast_file_name);
    std::string jsonast_content;
    while (getline(jsonast_file_stream, new_line)) {
        if (new_line.find("======= " + sol_name) != std::string::npos) {
            break;
        }
    }
    while (getline(jsonast_file_stream, new_line)) {
        jsonast_content = jsonast_content + new_line + "\n";
    }
    nlohmann::json jsonast = nlohmann::json::parse(jsonast_content);
    
    jsonast_file_stream.close();


    int num_functions = 0;
    auto root_node = jsonast.at("nodes");
    for (size_t i = 0; i < root_node.size(); ++i) {
        auto contract_node = root_node.at(i);
        if (contract_node.at("nodeType") != "ContractDefinition") continue;
        for (size_t j = 0; j < contract_node.at("nodes").size(); ++j) {
            auto sub_node = contract_node.at("nodes").at(j);
            if (sub_node.at("nodeType") == "FunctionDefinition" && !sub_node.at("body").empty()) {
                num_functions++;
            }
        }
    }
    //std::cout << "num_functions: " << num_functions << std::endl; 
    int target_function_id;
    srand((unsigned)time(0)); 
    target_function_id = (rand()%num_functions)+1; 
    //std::cout << "target_function_id: " << target_function_id << std::endl; 

    /*
    Code debugging the json ast
    std::cout << jsonast.dump();
    std::cout << jsonast.at("nodes").at(2).at("nodes").at(0).at("visibility") << std::endl;
    std::cout << jsonast.at("nodes").at(2).at("nodes").at(0).at("visibility") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(5).at("name") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(5).at("visibility") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(5).at("stateMutability") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(4).at("isConstructor") << std::endl;
    std::cout << jsonast.at("nodes").at(7).at("nodes").at(5).at("visibility") << std::endl;
    */

    
    SolInstru::ASTAnalyser ast_analyser(ast_content, jsonast, mutation_type, target_function_id, true, sol_name);
    std::stringstream new_source = ast_analyser.analyse();
    std::cout<<new_name<<std::endl; 
    if (new_name != "") {
        std::ofstream output_file_stream(new_name);
        output_file_stream << new_source.str();
        output_file_stream.close();
    } else {
        std::cout << new_source.str() << std::endl;
    }

    return 0;
}
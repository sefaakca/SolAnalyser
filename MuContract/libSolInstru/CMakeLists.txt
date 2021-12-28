file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_library(SolInstru ${sources} ${headers})
target_include_directories(SolInstru PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(SolInstru PRIVATE Utils nlohmann_json::nlohmann_json)

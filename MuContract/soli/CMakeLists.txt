set(
	sources
	CommandLineInterface.cpp CommandLineInterface.hpp
	main.cpp
)

add_executable(soli ${sources})
target_include_directories(soli PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(soli PRIVATE SolInstru Utils nlohmann_json::nlohmann_json)

include(GNUInstallDirs)
install(TARGETS soli DESTINATION "${CMAKE_INSTALL_BINDIR}")


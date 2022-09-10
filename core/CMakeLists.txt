project(core)

add_library(core_lib)

target_sources(core_lib
        PUBLIC
        model/include/entity.h
        model/include/hyper_edge.h
        model/include/hyper_graph.h model/src/hyper_graph.cpp
        model/include/node.h
        )

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
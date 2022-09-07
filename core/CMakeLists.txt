target_sources(hyper_grapher_lib
        PUBLIC
        include/entity.h
        include/hyper_edge.h
        include/hyper_graph.h src/hyper_graph.cpp
        include/node.h
        )

target_include_directories(hyper_grapher_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
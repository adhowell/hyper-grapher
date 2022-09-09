add_library(gui_lib)

target_sources(gui_lib
        PUBLIC
        hyper_graph_view/include/hyper_graph_scene.h hyper_graph_view/src/hyper_graph_scene.cpp
        hyper_graph_view/include/hyper_graph_view.h hyper_graph_view/src/hyper_graph_view.cpp
        items/include/edge_item.h items/src/edge_item.cpp
        items/include/node_item.h items/src/node_item.cpp
        main_window/include/main_window.h main_window/src/main_window.cpp
        )

target_include_directories(gui_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(gui_lib PUBLIC Qt5::Widgets core_lib)
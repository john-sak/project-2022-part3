add_executable(evaluate main.cpp)

target_link_libraries(evaluate PRIVATE CGAL::CGAL)
target_link_libraries(evaluate PRIVATE arguments)
target_link_libraries(evaluate PRIVATE polyline)
target_link_libraries(evaluate PRIVATE optimization)

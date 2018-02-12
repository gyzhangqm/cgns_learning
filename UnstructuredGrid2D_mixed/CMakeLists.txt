project (UnstructuredGrid2D_mixed)

include_directories(${CMAKE_SOURCE_DIR}/include)

file (GLOB_RECURSE ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/*.c)

add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} CGNSFile)


cmake_minimum_required( VERSION 4.2.0 )
project( camasim LANGUAGES C CXX )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set( CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic" )
set( CMAKE_CXX_FLAGS_DEBUG "-g" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2" )

find_package( MPI REQUIRED COMPONENTS C CXX )
find_package( OpenMP REQUIRED )
find_package( benchmark REQUIRED )

find_package( ZLIB COMPONENTS REQUIRED )

add_library( cnpy STATIC external/cnpy/cnpy.cpp )
target_include_directories( cnpy PUBLIC external/cnpy )
target_include_directories( cnpy PUBLIC ${ZLIB_INCLUDE_DIRS} )
target_link_libraries(cnpy ${ZLIB_LIBRARIES})


add_library( mpi_monte_options_lib STATIC )
target_link_libraries( mpi_monte_options_lib
  PRIVATE 
    cnpy
    OpenMP::OpenMP_CXX
    MPI::MPI_CXX
)


### EXECUTABLES ###

add_executable( run_mpi_monte_options )
target_link_libraries( run_mpi_monte_options 
  PRIVATE 
    cnpy
    OpenMP::OpenMP_CXX
    MPI::MPI_CXX
)

add_executable( test_mpi_monte_options )
target_link_libraries( test_mpi_monte_options 
  PRIVATE 
    cnpy
    OpenMP::OpenMP_CXX
    MPI::MPI_CXX
)

add_executable( benchmark_mpi_monte_options )
target_link_libraries( benchmark_mpi_monte_options
  PRIVATE 
    cnpy
    OpenMP::OpenMP_CXX
    benchmark::benchmark
    MPI::MPI_CXX
)


add_subdirectory( src )


### COPYING SCRIPTS ###

add_custom_command( 
  TARGET test_mpi_monte_options POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy 
    ${CMAKE_SOURCE_DIR}/scripts/script_mpi_monte_options.py
    $<TARGET_FILE_DIR:test_mpi_monte_options>
)



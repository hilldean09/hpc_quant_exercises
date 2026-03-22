
// GPU Accelerated Algorithm
#include "./gpu_mpi_monte_options.hpp"

#include <cstdlib>
#include <cuda.h>


#include "./mpi_monte_options.hpp"

float Run_GPU_Accelerated_Simulation( unsigned long long total_runs,
                                      unsigned long long total_timesteps,
                                      unsigned long long seed,
                                      bool do_write_to_file,
                                      Heston_Parameters parameters,
                                      float strike_price,
                                      float discounting_rate,
                                      unsigned long long concurrent_runs,
                                      bool do_collect_statistics,
                                      Simulation_Statistics simulation_stats ) {

  float output_call_price = 0.0;

  float* host_random_number_array;
  float* host_call_price_array;

  float* device_random_number_array_1;
  float* device_random_number_array_2;
  float* device_variance_array;
  float* device_call_price_array_1;
  float* device_call_price_array_2;

  // Allocating host memory
  host_random_number_array = ( float* ) std::malloc( 2 * total_timesteps * concurrent_runs * sizeof( float ) );
  host_call_price_array = ( float* ) std::malloc( total_timesteps * concurrent_runs * sizeof( float ) );


  // Allocating device memory
  cudaMalloc( &device_random_number_array_1, 2 * total_timesteps * concurrent_runs * sizeof( float ) );
  cudaMalloc( &device_random_number_array_2, 2 * total_timesteps * concurrent_runs * sizeof( float ) );
  cudaMalloc( &device_variance_array, total_timesteps * concurrent_runs * sizeof( float ) );
  cudaMalloc( &device_call_price_array_1, total_timesteps * concurrent_runs * sizeof( float ) );
  cudaMalloc( &device_call_price_array_2, total_timesteps * concurrent_runs * sizeof( float ) );


  // Freeing Memory
  cudaFree( device_random_number_array_1 );
  cudaFree( device_random_number_array_2 );
  cudaFree( device_variance_array );
  cudaFree( device_call_price_array_1 );
  cudaFree( device_call_price_array_2 );

  std::free( host_random_number_array );
  std::free( host_call_price_array );
}



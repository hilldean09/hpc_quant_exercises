
// GPU Accelerated Algorithm
#include "./gpu_mpi_monte_options.hpp"

#include <cstdlib>


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

  host_random_number_array = ( float* ) std::malloc( total_timesteps * total_runs * sizeof( float ) );
  host_call_price_array = ( float* ) std::malloc( total_timesteps * total_runs * sizeof( float ) );

  

  std::free( host_random_number_array );
}



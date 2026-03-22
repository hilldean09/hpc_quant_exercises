
#ifndef MMCOP_GPU_MPI_MONTE_OPTIONS_HPP
#define MMCOP_GPU_MPI_MONTE_OPTIONS_HPP

#include "./io_mpi_monte_options.hpp"
#include "./mpi_monte_options.hpp"
#include "./pre_controls.hpp"

float Run_GPU_Accelerated_Simulation( unsigned long long total_runs,
                                      unsigned long long total_timesteps,
                                      unsigned long long seed,
                                      bool do_write_to_file,
                                      Heston_Parameters parameters,
                                      float strike_price,
                                      float discounting_rate,
                                      unsigned long long concurrent_runs = MMCOP_GPU_DEFAULT_CONCURRENT_RUNS,
                                      bool do_collect_statistics = false,
                                      Simulation_Statistics simulation_stats = nullptr );

#endif

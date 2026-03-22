
#ifndef MMCOP_GPU_MPI_MONTE_OPTIONS_HPP
#define MMCOP_GPU_MPI_MONTE_OPTIONS_HPP

float Run_GPU_Accelerated_Simulation( unsigned long long total_runs,
                                      unsigned long long total_timesteps,
                                      unsigned long long seed,
                                      bool do_write_to_file,
                                      Heston_Parameters parameters,
                                      float strike_price,
                                      float discounting_rate );

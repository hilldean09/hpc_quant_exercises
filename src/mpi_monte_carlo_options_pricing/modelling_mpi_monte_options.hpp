
#ifndef MODELLING_MPI_MONTE_OPTIONS_HPP
#define MODELLING_MPI_MONTE_OPTIONS_HPP

#include "./io_mpi_monte_options.hpp"
#include "./mpi_monte_options.hpp"
#include "./pre_controls.hpp"

#include <cnpy.h>
#include <omp.h>
#include <mpi.h>

#include <random>
#include <iostream>

namespace MPI_MONTE_OPTIONS {


///// VERSION 1 /////
float Run_Multi_Threaded_Simulation_V1( unsigned long long total_runs,
                                        unsigned long long total_timesteps,
                                        unsigned long long seed,
                                        bool do_write_to_file,
                                        Heston_Parameters parameters,
                                        float strike_price,
                                        float discounting_rate );

float Simulate_Asset_Price_Walk_V1( unsigned long long total_timesteps,
                                    std::mt19937_64* random_engine,
                                    std::normal_distribution<float>* normal_distribution_gen,
                                    Heston_Parameters parameters );

///// VERSION 2 /////
float Run_Multi_Threaded_Simulation_V2( unsigned long long total_runs,
                                        unsigned long long total_timesteps,
                                        unsigned long long seed,
                                        bool do_write_to_file,
                                        Heston_Parameters parameters,
                                        float strike_price,
                                        float discounting_rate );

float Simulate_Asset_Price_Walk_V2( unsigned long long total_timesteps,
                                    std::mt19937_64* random_engine,
                                    std::normal_distribution<float>* normal_distribution_gen,
                                    Heston_Parameters parameters );


///// VERSION 3 /////

float Run_Multi_Threaded_Simulation_V3( unsigned long long total_runs,
                                       unsigned long long total_timesteps,
                                       unsigned long long seed,
                                       bool do_write_to_file,
                                       Heston_Parameters parameters,
                                       float strike_price,
                                       float discounting_rate );

float Simulate_Asset_Price_Walk_V3( const unsigned long long total_timesteps,
                                    std::subtract_with_carry_engine<std::uint_fast64_t, 48, 5, 12>* random_engine,
                                    std::uniform_real_distribution<float>* uniform_distribution_gen,
                                    const Heston_Parameters parameters );


///// VERSION 4 /////
float Run_Multi_Threaded_Simulation_V4( unsigned long long total_runs,
                                       unsigned long long total_timesteps,
                                       unsigned long long seed,
                                       bool do_write_to_file,
                                       Heston_Parameters parameters,
                                       float strike_price,
                                       float discounting_rate );

float Simulate_Asset_Price_Walk_V4( const unsigned long long total_timesteps,
                                    std::subtract_with_carry_engine<std::uint_fast64_t, 48, 5, 12>* random_engine,
                                    std::uniform_real_distribution<float>* uniform_distribution_gen,
                                    const Heston_Parameters parameters );

}

#endif

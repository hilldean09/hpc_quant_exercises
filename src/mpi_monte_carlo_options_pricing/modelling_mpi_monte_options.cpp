
#include "./io_mpi_monte_options.hpp"
#include "./mpi_monte_options.hpp"
#include "./modelling_mpi_monte_options.hpp"
#include "./pre_controls.hpp"

#include <cnpy.h>
#include <mpi.h>
#include <omp.h>

#include <cstdint>
#include <string>
#include <random>
#include <iostream>

namespace MPI_MONTE_OPTIONS {


///// VERSION 1 /////

// Reducing heap memory allocation
float Run_Multi_Threaded_Simulation_V1( unsigned long long total_runs,
                                       unsigned long long total_timesteps,
                                       unsigned long long seed,
                                       bool do_write_to_file,
                                       Heston_Parameters parameters,
                                       float strike_price,
                                       float discounting_rate ) {

  float output_call_price = 0.0;
  float working_call_price = 0.0;

  if( do_write_to_file ) {
    std::cout << "ERROR : Write to file not supported in this implementation" << std::endl;
  }

  #pragma omp parallel default( none ) private( working_call_price ) shared( output_call_price, total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate ) num_threads( MMCOP_NUMBER_OF_THREADS )
  {
    int thread_idx = omp_get_thread_num();

    // Deterministicly random componentry
    // Modifying seed deterministically per thread
    std::mt19937_64 random_engine;
    random_engine.seed( seed + thread_idx * MMCOP_THREAD_SEED_MAGIC_NUMBER );
    std::normal_distribution<float> normal_distribution_gen( 0.0, 1.0 );

    // Parallelised simulation 
    #pragma omp for schedule( static ) 
    for( unsigned long long run = 0; run < total_runs; run++ ) {

      working_call_price = std::max( Simulate_Asset_Price_Walk_V1( total_timesteps, &random_engine, &normal_distribution_gen, parameters )
                                      - strike_price, ( float ) 0.0 );

      // Summing call price
      #pragma omp atomic update
      output_call_price += working_call_price;

    }

  }

  output_call_price = ( std::pow( discounting_rate, total_timesteps * parameters.timestep ) * output_call_price ) / total_runs;

  return output_call_price;
}

float Simulate_Asset_Price_Walk_V1( unsigned long long total_timesteps,
                                    std::mt19937_64* random_engine,
                                    std::normal_distribution<float>* normal_distribution_gen,
                                    Heston_Parameters parameters ) {

  float asset_price = parameters.initial_price;

  float variance = parameters.initial_variance;
  float weiner_step_1;
  float weiner_step_2;

  for( unsigned long long timestep = 0; timestep < total_timesteps; timestep++ ) {

    // Using the Heston stochastic volatility model

    // W_1 = sqrt( dt ) * Z
    weiner_step_1 = std::sqrt( parameters.timestep ) * ( *normal_distribution_gen )( *random_engine );

    // dS_t = drift * S_t * dt + sqrt( var ) * S_t * W_1
    asset_price += parameters.drift * asset_price * parameters.timestep 
                   + std::sqrt( variance ) * asset_price * weiner_step_1;

    // W_2 = p * W_1 + sqrt( 1 - p^2 ) * sqrt( dt ) * Z
    weiner_step_2 = parameters.correlation_factor * weiner_step_1
                    + std::sqrt( std::abs( 1 - std::pow( parameters.correlation_factor, 2 ) ) )
                    * std::sqrt( parameters.timestep ) 
                    * ( *normal_distribution_gen )( *random_engine );

    // dvar = k( Θ - var ) * dt + σ * sqrt( var ) * W_2
    variance += parameters.mean_reversion_speed 
                * ( parameters.mean_reversion_level - variance ) * parameters.timestep
                + parameters.volatility * std::sqrt( variance ) * weiner_step_2;

    // Alternative to abs is max
    variance = std::abs( variance );

  }

  return asset_price;

}


///// VERSION 1 /////

float Run_Multi_Threaded_Simulation_V2( unsigned long long total_runs,
                                       unsigned long long total_timesteps,
                                       unsigned long long seed,
                                       bool do_write_to_file,
                                       Heston_Parameters parameters,
                                       float strike_price,
                                       float discounting_rate ) {

  float output_call_price = 0.0;
  float working_call_price = 0.0;

  if( do_write_to_file ) {
    std::cout << "ERROR : Write to file not supported in this implementation" << std::endl;
  }

  #pragma omp parallel default( none ) private( working_call_price ) shared( output_call_price, total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate ) num_threads( MMCOP_NUMBER_OF_THREADS )
  {
    int thread_idx = omp_get_thread_num();

    // Deterministicly random componentry
    // Modifying seed deterministically per thread
    std::mt19937_64 random_engine;
    random_engine.seed( seed + thread_idx * MMCOP_THREAD_SEED_MAGIC_NUMBER );
    std::normal_distribution<float> normal_distribution_gen( 0.0, 1.0 );

    // Parallelised simulation 
    #pragma omp for schedule( static ) 
    for( unsigned long long run = 0; run < total_runs; run++ ) {

      working_call_price = std::max( Simulate_Asset_Price_Walk_V2( total_timesteps, &random_engine, &normal_distribution_gen, parameters )
                                      - strike_price, ( float ) 0.0 );

      // Summing call price
      #pragma omp atomic update
      output_call_price += working_call_price;

    }

  }

  output_call_price = ( std::pow( discounting_rate, total_timesteps * parameters.timestep ) * output_call_price ) / total_runs;

  return output_call_price;

}

float Simulate_Asset_Price_Walk_V2( const unsigned long long total_timesteps,
                                    std::mt19937_64* random_engine,
                                    std::normal_distribution<float>* normal_distribution_gen,
                                    const Heston_Parameters parameters ) {

  // NOTE: Only calculating root variance once per iteration via another
  // variable actually slowed the function down. Verified

  float asset_price = parameters.initial_price;
  float variance = parameters.initial_variance;
  float weiner_step_1;
  float weiner_step_2;

  float root_timestep = std::sqrt( parameters.timestep ); // Validated beneficial
  float root_one_minus_correlation_squared = std::sqrt( std::abs( 1 - parameters.correlation_factor * parameters.correlation_factor ) ); // Validated beneficial

  for( unsigned long long timestep = 0; timestep < total_timesteps; timestep++ ) {

    // Using the Heston stochastic volatility model

    // W_1 = sqrt( dt ) * Z
    weiner_step_1 = root_timestep * ( *normal_distribution_gen )( *random_engine );

    // dS_t = drift * S_t * dt + sqrt( var ) * S_t * W_1
    asset_price += parameters.drift * asset_price * parameters.timestep 
                   + std::sqrt( variance ) * asset_price * weiner_step_1; 
    // NOTE: Validated benefit over += method timewise however causes the price to get stuck to 0

    // W_2 = p * W_1 + sqrt( 1 - p^2 ) * sqrt( dt ) * Z
    weiner_step_2 = parameters.correlation_factor * weiner_step_1
                    + root_one_minus_correlation_squared
                    * root_timestep
                    * ( *normal_distribution_gen )( *random_engine );

    // dvar = k( Θ - var ) * dt + σ * sqrt( var ) * W_2
    variance += parameters.mean_reversion_speed 
                * ( parameters.mean_reversion_level - variance ) * parameters.timestep
                + parameters.volatility * std::sqrt( variance ) * weiner_step_2;

    // Alternative to abs is max
    variance = std::abs( variance );

  }

  return asset_price;

}


///// VERSION 3 /////

float Run_Multi_Threaded_Simulation_V3( unsigned long long total_runs,
                                       unsigned long long total_timesteps,
                                       unsigned long long seed,
                                       bool do_write_to_file,
                                       Heston_Parameters parameters,
                                       float strike_price,
                                       float discounting_rate ) {

  float output_call_price = 0.0;
  float working_call_price = 0.0;

  if( do_write_to_file ) {
    std::cout << "ERROR : Write to file not supported in this implementation" << std::endl;
  }

  #pragma omp parallel default( none ) private( working_call_price ) shared( output_call_price, total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate ) num_threads( MMCOP_NUMBER_OF_THREADS )
  {
    int thread_idx = omp_get_thread_num();

    // Deterministicly random componentry
    // Modifying seed deterministically per thread
    std::subtract_with_carry_engine<std::uint_fast64_t, 48, 5, 12> random_engine;
    random_engine.seed( seed + thread_idx * MMCOP_THREAD_SEED_MAGIC_NUMBER );
    
    // Changing this alone while less accurate is significantly faster
    std::uniform_real_distribution<float> uniform_distribution_gen( 0.0, 1.0 ); 

    // Parallelised simulation 
    #pragma omp for schedule( static ) 
    for( unsigned long long run = 0; run < total_runs; run++ ) {

      working_call_price = std::max( Simulate_Asset_Price_Walk_V3( total_timesteps, &random_engine, &uniform_distribution_gen, parameters )
                                      - strike_price, ( float ) 0.0 );

      // Summing call price
      #pragma omp atomic update
      output_call_price += working_call_price;

    }

  }

  output_call_price = ( std::pow( discounting_rate, total_timesteps * parameters.timestep ) * output_call_price ) / total_runs;

  return output_call_price;

}

float Simulate_Asset_Price_Walk_V3( const unsigned long long total_timesteps,
                                    std::subtract_with_carry_engine<std::uint_fast64_t, 48, 5, 12>* random_engine,
                                    std::uniform_real_distribution<float>* uniform_distribution_gen,
                                    const Heston_Parameters parameters ) {

  // NOTE: Only calculating root variance once per iteration via another
  // variable actually slowed the function down. Verified

  float asset_price = parameters.initial_price;
  float variance = parameters.initial_variance;
  float weiner_step_1;
  float weiner_step_2;

  // Box Muller
  float box_muller_r;
  float box_muller_theta;
  float box_muller_z1;
  float box_muller_z2;

  float root_timestep = std::sqrt( parameters.timestep ); // Validated beneficial
  float root_one_minus_correlation_squared = std::sqrt( std::abs( 1 - parameters.correlation_factor * parameters.correlation_factor ) ); // Validated beneficial

  for( unsigned long long timestep = 0; timestep < total_timesteps; timestep++ ) {

    // Box Muller
    box_muller_r = std::sqrt( -2 * std::log( ( *uniform_distribution_gen )( *random_engine ) ) );
    box_muller_theta = 2 * 3.141592653 * ( *uniform_distribution_gen )( *random_engine );

    // Seperated to encourage fsincos instruction compilation
    box_muller_z1 = std::cos( box_muller_theta );
    box_muller_z2 = std::sin( box_muller_theta );

    box_muller_z1 *= box_muller_r;
    box_muller_z2 *= box_muller_r;

    // Using the Heston stochastic volatility model

    // W_1 = sqrt( dt ) * Z
    weiner_step_1 = root_timestep * box_muller_z1;

    // dS_t = drift * S_t * dt + sqrt( var ) * S_t * W_1
    asset_price += parameters.drift * asset_price * parameters.timestep 
                   + std::sqrt( variance ) * asset_price * weiner_step_1; 
    // NOTE: Validated benefit over += method timewise however causes the price to get stuck to 0

    // W_2 = p * W_1 + sqrt( 1 - p^2 ) * sqrt( dt ) * Z
    weiner_step_2 = parameters.correlation_factor * weiner_step_1
                    + root_one_minus_correlation_squared
                    * root_timestep
                    * box_muller_z2;
    
    // dvar = k( Θ - var ) * dt + σ * sqrt( var ) * W_2
    variance += parameters.mean_reversion_speed 
                * ( parameters.mean_reversion_level - variance ) * parameters.timestep
                + parameters.volatility * std::sqrt( variance ) * weiner_step_2;

    // Alternative to abs is max
    variance = std::abs( variance );

  }

  return asset_price;

}


///// VERSION 4 /////

float Run_Multi_Threaded_Simulation_V4( unsigned long long total_runs,
                                       unsigned long long total_timesteps,
                                       unsigned long long seed,
                                       bool do_write_to_file,
                                       Heston_Parameters parameters,
                                       float strike_price,
                                       float discounting_rate ) {

  float output_call_price = 0.0;

  if( do_write_to_file ) {
    std::cout << "ERROR : Write to file not supported in this implementation" << std::endl;
  }

  #pragma omp parallel default( none ) shared( output_call_price, total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate ) num_threads( MMCOP_NUMBER_OF_THREADS )
  {
    int thread_idx = omp_get_thread_num();

    // Deterministicly random componentry
    // Modifying seed deterministically per thread
    std::subtract_with_carry_engine<std::uint_fast64_t, 48, 5, 12> random_engine;
    random_engine.seed( seed + thread_idx * MMCOP_THREAD_SEED_MAGIC_NUMBER );
    
    // Changing this alone while less accurate is significantly faster
    std::uniform_real_distribution<float> uniform_distribution_gen( 0.0, 1.0 ); 

    // Parallelised simulation 
    #pragma omp for schedule( static ) reduction( +:output_call_price )
    for( unsigned long long run = 0; run < total_runs; run++ ) {

      output_call_price += std::max( Simulate_Asset_Price_Walk_V4( total_timesteps, &random_engine, &uniform_distribution_gen, parameters )
                                      - strike_price, ( float ) 0.0 );

    }

  }

  output_call_price = ( std::pow( discounting_rate, total_timesteps * parameters.timestep ) * output_call_price ) / total_runs;

  return output_call_price;

}

float Simulate_Asset_Price_Walk_V4( const unsigned long long total_timesteps,
                                    std::subtract_with_carry_engine<std::uint_fast64_t, 48, 5, 12>* random_engine,
                                    std::uniform_real_distribution<float>* uniform_distribution_gen,
                                    const Heston_Parameters parameters ) {

  // NOTE: Only calculating root variance once per iteration via another
  // variable actually slowed the function down. Verified

  float asset_price = parameters.initial_price;
  float variance = parameters.initial_variance;
  float weiner_step_1;
  float weiner_step_2;

  // Box Muller
  float box_muller_r;
  float box_muller_theta;
  float box_muller_z1;
  float box_muller_z2;

  float root_timestep = std::sqrt( parameters.timestep ); // Validated beneficial
  float root_one_minus_correlation_squared = std::sqrt( std::abs( 1 - parameters.correlation_factor * parameters.correlation_factor ) ); // Validated beneficial

  for( unsigned long long timestep = 0; timestep < total_timesteps; timestep++ ) {

    // Box Muller
    box_muller_r = std::sqrt( -2 * std::log( ( *uniform_distribution_gen )( *random_engine ) ) );
    box_muller_theta = 2 * 3.141592653 * ( *uniform_distribution_gen )( *random_engine );

    // Seperated to encourage fsincos instruction compilation
    box_muller_z1 = std::cos( box_muller_theta );
    box_muller_z2 = std::sin( box_muller_theta );

    box_muller_z1 *= box_muller_r;
    box_muller_z2 *= box_muller_r;

    // Using the Heston stochastic volatility model

    // W_1 = sqrt( dt ) * Z
    weiner_step_1 = root_timestep * box_muller_z1;

    // dS_t = drift * S_t * dt + sqrt( var ) * S_t * W_1
    asset_price += parameters.drift * asset_price * parameters.timestep 
                   + std::sqrt( variance ) * asset_price * weiner_step_1; 
    // NOTE: Validated benefit over += method timewise however causes the price to get stuck to 0

    // W_2 = p * W_1 + sqrt( 1 - p^2 ) * sqrt( dt ) * Z
    weiner_step_2 = parameters.correlation_factor * weiner_step_1
                    + root_one_minus_correlation_squared
                    * root_timestep
                    * box_muller_z2;
    
    // dvar = k( Θ - var ) * dt + σ * sqrt( var ) * W_2
    variance += parameters.mean_reversion_speed 
                * ( parameters.mean_reversion_level - variance ) * parameters.timestep
                + parameters.volatility * std::sqrt( variance ) * weiner_step_2;

    // Alternative to abs is max
    variance = std::abs( variance );

  }

  return asset_price;

}




} // Global namespace end




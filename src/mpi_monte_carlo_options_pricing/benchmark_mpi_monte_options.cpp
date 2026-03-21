
#include "./mpi_monte_options.hpp"
#include "./modelling_mpi_monte_options.hpp"
#include "./pre_controls.hpp"

#include <benchmark/benchmark.h>

#include <iostream>
#include <string>


using namespace MPI_MONTE_OPTIONS;


#define MMCOP_BM_TOTAL_RUNS 100000
#define MMCOP_BM_TOTAL_TIMESTEPS 504


// Version 0
static void BM_Run_Single_Threaded_Simulation_V0( benchmark::State& state ) {
  // Parameters
  unsigned long long total_timesteps = MMCOP_BM_TOTAL_TIMESTEPS;
  unsigned long long seed = MMCOP_DEFAULT_SEED;
  bool do_write_to_file = false;
  Heston_Parameters parameters = Construct_Parameters_Object( MMCOP_DEFAULT_INITIAL_PRICE, MMCOP_DEFAULT_INITIAL_VARIANCE, MMCOP_DEFAULT_TIMESTEP, MMCOP_DEFAULT_DRIFT, MMCOP_DEFAULT_MEAN_REVERSION_SPEED, MMCOP_DEFAULT_MEAN_REVERSION_LEVEL, MMCOP_DEFAULT_VOLATILITY, MMCOP_DEFAULT_CORRELATION_FACTOR ); 

  for( auto _ : state ) {
    Run_Single_Threaded_Simulation( state.range( 0 ), total_timesteps, seed, do_write_to_file, parameters );
  }
}

BENCHMARK( BM_Run_Single_Threaded_Simulation_V0 )->RangeMultiplier( 2 )->Range( 64, 1'000'000 );

static void BM_Run_Multi_Threaded_Simulation_V0( benchmark::State& state ) {
  // Parameters
  unsigned long long total_timesteps = MMCOP_BM_TOTAL_TIMESTEPS;
  unsigned long long seed = MMCOP_DEFAULT_SEED;
  bool do_write_to_file = false;
  Heston_Parameters parameters = Construct_Parameters_Object( MMCOP_DEFAULT_INITIAL_PRICE, MMCOP_DEFAULT_INITIAL_VARIANCE, MMCOP_DEFAULT_TIMESTEP, MMCOP_DEFAULT_DRIFT, MMCOP_DEFAULT_MEAN_REVERSION_SPEED, MMCOP_DEFAULT_MEAN_REVERSION_LEVEL, MMCOP_DEFAULT_VOLATILITY, MMCOP_DEFAULT_CORRELATION_FACTOR ); 

  for( auto _ : state ) {
    Run_Multi_Threaded_Simulation( state.range( 0 ), total_timesteps, seed, do_write_to_file, parameters );
  }
}

BENCHMARK( BM_Run_Multi_Threaded_Simulation_V0 )->RangeMultiplier( 2 )->Range( 64, 1'000'000 );


// Version 1
static void BM_Run_Multi_Threaded_Simulation_V1( benchmark::State& state ) {
  // Parameters
  unsigned long long total_timesteps = MMCOP_BM_TOTAL_TIMESTEPS;
  unsigned long long seed = MMCOP_DEFAULT_SEED;
  bool do_write_to_file = false;
  Heston_Parameters parameters = Construct_Parameters_Object( MMCOP_DEFAULT_INITIAL_PRICE, MMCOP_DEFAULT_INITIAL_VARIANCE, MMCOP_DEFAULT_TIMESTEP, MMCOP_DEFAULT_DRIFT, MMCOP_DEFAULT_MEAN_REVERSION_SPEED, MMCOP_DEFAULT_MEAN_REVERSION_LEVEL, MMCOP_DEFAULT_VOLATILITY, MMCOP_DEFAULT_CORRELATION_FACTOR ); 
  float strike_price = MMCOP_DEFAULT_STRIKE_PRICE;
  float discounting_rate = MMCOP_DEFAULT_DISCOUNTING_RATE;

  for( auto _ : state ) {
    Run_Multi_Threaded_Simulation_V1( state.range( 0 ), total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  }
}

BENCHMARK( BM_Run_Multi_Threaded_Simulation_V1 )->RangeMultiplier( 2 )->Range( 64, 1'000'000 );

// Version 2
static void BM_Run_Multi_Threaded_Simulation_V2( benchmark::State& state ) {
  // Parameters
  unsigned long long total_timesteps = MMCOP_BM_TOTAL_TIMESTEPS;
  unsigned long long seed = MMCOP_DEFAULT_SEED;
  bool do_write_to_file = false;
  Heston_Parameters parameters = Construct_Parameters_Object( MMCOP_DEFAULT_INITIAL_PRICE, MMCOP_DEFAULT_INITIAL_VARIANCE, MMCOP_DEFAULT_TIMESTEP, MMCOP_DEFAULT_DRIFT, MMCOP_DEFAULT_MEAN_REVERSION_SPEED, MMCOP_DEFAULT_MEAN_REVERSION_LEVEL, MMCOP_DEFAULT_VOLATILITY, MMCOP_DEFAULT_CORRELATION_FACTOR ); 
  float strike_price = MMCOP_DEFAULT_STRIKE_PRICE;
  float discounting_rate = MMCOP_DEFAULT_DISCOUNTING_RATE;

  for( auto _ : state ) {
    Run_Multi_Threaded_Simulation_V2( state.range( 0 ), total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  }
}

BENCHMARK( BM_Run_Multi_Threaded_Simulation_V2 )->RangeMultiplier( 2 )->Range( 64, 1'000'000 );


// Version 3
static void BM_Run_Multi_Threaded_Simulation_V3( benchmark::State& state ) {
  // Parameters
  unsigned long long total_timesteps = MMCOP_BM_TOTAL_TIMESTEPS;
  unsigned long long seed = MMCOP_DEFAULT_SEED;
  bool do_write_to_file = false;
  Heston_Parameters parameters = Construct_Parameters_Object( MMCOP_DEFAULT_INITIAL_PRICE, MMCOP_DEFAULT_INITIAL_VARIANCE, MMCOP_DEFAULT_TIMESTEP, MMCOP_DEFAULT_DRIFT, MMCOP_DEFAULT_MEAN_REVERSION_SPEED, MMCOP_DEFAULT_MEAN_REVERSION_LEVEL, MMCOP_DEFAULT_VOLATILITY, MMCOP_DEFAULT_CORRELATION_FACTOR ); 
  float strike_price = MMCOP_DEFAULT_STRIKE_PRICE;
  float discounting_rate = MMCOP_DEFAULT_DISCOUNTING_RATE;

  for( auto _ : state ) {
    Run_Multi_Threaded_Simulation_V3( state.range( 0 ), total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  }
}

BENCHMARK( BM_Run_Multi_Threaded_Simulation_V3 )->RangeMultiplier( 2 )->Range( 64, 1'000'000 );


// Version 4
static void BM_Run_Multi_Threaded_Simulation_V4( benchmark::State& state ) {
  // Parameters
  unsigned long long total_timesteps = MMCOP_BM_TOTAL_TIMESTEPS;
  unsigned long long seed = MMCOP_DEFAULT_SEED;
  bool do_write_to_file = false;
  Heston_Parameters parameters = Construct_Parameters_Object( MMCOP_DEFAULT_INITIAL_PRICE, MMCOP_DEFAULT_INITIAL_VARIANCE, MMCOP_DEFAULT_TIMESTEP, MMCOP_DEFAULT_DRIFT, MMCOP_DEFAULT_MEAN_REVERSION_SPEED, MMCOP_DEFAULT_MEAN_REVERSION_LEVEL, MMCOP_DEFAULT_VOLATILITY, MMCOP_DEFAULT_CORRELATION_FACTOR ); 
  float strike_price = MMCOP_DEFAULT_STRIKE_PRICE;
  float discounting_rate = MMCOP_DEFAULT_DISCOUNTING_RATE;

  for( auto _ : state ) {
    Run_Multi_Threaded_Simulation_V4( state.range( 0 ), total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  }
}

BENCHMARK( BM_Run_Multi_Threaded_Simulation_V4 )->RangeMultiplier( 2 )->Range( 64, 1'000'000 );

BENCHMARK_MAIN();


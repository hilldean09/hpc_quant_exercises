
#include "./pre_controls.hpp"
#include "./modelling_mpi_monte_options.hpp"
#include "./mpi_monte_options.hpp"
#include "./io_mpi_monte_options.hpp"

#include <iostream>
#include <string>

using namespace MPI_MONTE_OPTIONS;

void Print_Parameters( unsigned long long total_runs,
                       unsigned long long total_timesteps,
                       unsigned long long seed,
                       bool do_write_to_file,
                       Heston_Parameters parameters,
                       float strike_price,
                       float discounting_rate ) {

  std::cout << "\tTotal runs : " << std::to_string( total_runs ) << "\n";
  std::cout << "\tTotal timesteps : " << std::to_string( total_timesteps ) << "\n";
  std::cout << "\tTimestep : " << std::to_string( parameters.timestep ) << "\n";
  std::cout << "\tSeed : " << std::to_string( seed ) << "\n";
  std::cout << "\tWrite to file? : " << std::to_string( do_write_to_file ) << "\n";

  std::cout << "\tInitial price : " << std::to_string( parameters.initial_price ) << "\n";

  std::cout << "\tInitial variance : " << std::to_string( parameters.initial_variance ) << "\n";
  std::cout << "\tDrift factor : " << std::to_string( parameters.drift ) << "\n";
  std::cout << "\tMean reversion speed : " << std::to_string( parameters.mean_reversion_speed ) << "\n";
  std::cout << "\tMean reversion level : " << std::to_string( parameters.mean_reversion_level ) << "\n";
  std::cout << "\tVolatility : " << std::to_string( parameters.volatility ) << "\n";
  std::cout << "\tCorrelation factor : " << std::to_string( parameters.correlation_factor ) << "\n";

  std::cout << "\tStrike price : " << std::to_string( strike_price ) << "\n";
  std::cout << "\tDiscounting rate : " << std::to_string( discounting_rate ) << std::endl;

}

int main( int argc, char** argv ) {

  // Parameters
  unsigned long long total_runs;
  unsigned long long total_timesteps;
  unsigned long long seed;
  bool do_write_to_file;
  Heston_Parameters parameters = Construct_Parameters_Object( MMCOP_DEFAULT_INITIAL_PRICE,
                                                              MMCOP_DEFAULT_INITIAL_VARIANCE,
                                                              MMCOP_DEFAULT_TIMESTEP,
                                                              MMCOP_DEFAULT_DRIFT,
                                                              MMCOP_DEFAULT_MEAN_REVERSION_SPEED,
                                                              MMCOP_DEFAULT_MEAN_REVERSION_LEVEL,
                                                              MMCOP_DEFAULT_VOLATILITY,
                                                              MMCOP_DEFAULT_CORRELATION_FACTOR );

  float strike_price = MMCOP_DEFAULT_STRIKE_PRICE;
  float discounting_rate = MMCOP_DEFAULT_DISCOUNTING_RATE;

  if( !IO::Parse_Parameters_From_Arguments( argc, argv, &total_runs, &total_timesteps,
                                       &seed, &do_write_to_file, &parameters,
                                       &strike_price, &discounting_rate ) ) {
    seed += 3;
  }

  std::vector<float> price_paths;
  float call_price;

  std::cout << "Running Default Single Threaded Test : " << std::endl;
  Print_Parameters( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  price_paths = Run_Single_Threaded_Simulation( total_runs, total_timesteps, seed, do_write_to_file, parameters );
  call_price = Compute_Call_Price( &price_paths, total_runs, total_timesteps, parameters.timestep, strike_price, discounting_rate ); 
  std::cout << "Test Results : \n";
  std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
  std::cout << std::endl;


  parameters.initial_variance /= 2.0;
  parameters.mean_reversion_speed *= 1.5;
  parameters.volatility /= 11.0;

  std::cout << "Running Calm Single Threaded Test : " << std::endl;
  Print_Parameters( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  price_paths = Run_Single_Threaded_Simulation( total_runs, total_timesteps, seed, do_write_to_file, parameters );
  call_price = Compute_Call_Price( &price_paths, total_runs, total_timesteps, parameters.timestep, strike_price, discounting_rate );
  std::cout << "Test Results : \n";
  std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
  std::cout << std::endl;


  parameters.initial_variance *= 2.0;
  parameters.mean_reversion_speed /= 1.5;
  parameters.volatility *= 10.0;

  std::cout << "Running Default Multi Threaded Test : " << std::endl;
  Print_Parameters( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  price_paths = Run_Multi_Threaded_Simulation( total_runs, total_timesteps, seed, do_write_to_file, parameters );
  call_price = Compute_Call_Price( &price_paths, total_runs, total_timesteps, parameters.timestep, strike_price, discounting_rate );
  std::cout << "Test Results : \n";
  std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
  std::cout << std::endl;


  std::cout << "Running Version 1 Multi Threaded Test : " << std::endl;
  Print_Parameters( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  call_price = Run_Multi_Threaded_Simulation_V1( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  std::cout << "Test Results : \n";
  std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
  std::cout << std::endl;

  std::cout << "Running Version 2 Multi Threaded Test : " << std::endl;
  Print_Parameters( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  call_price = Run_Multi_Threaded_Simulation_V2( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  std::cout << "Test Results : \n";
  std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
  std::cout << std::endl;

  std::cout << "Running Version 3 Multi Threaded Test : " << std::endl;
  Print_Parameters( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  call_price = Run_Multi_Threaded_Simulation_V3( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  std::cout << "Test Results : \n";
  std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
  std::cout << std::endl;

  std::cout << "Running Version 4 Multi Threaded Test : " << std::endl;
  Print_Parameters( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  call_price = Run_Multi_Threaded_Simulation_V4( total_runs, total_timesteps, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  std::cout << "Test Results : \n";
  std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
  std::cout << std::endl;


  return 0;
}


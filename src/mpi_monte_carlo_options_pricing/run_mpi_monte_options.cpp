

#include "./pre_controls.hpp"
#include "./mpi_monte_options.hpp"
#include "./io_mpi_monte_options.hpp"

#include <mpi.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace MPI_MONTE_OPTIONS;

void Print_Parameters( unsigned long long total_runs,
                       unsigned long long total_timesteps,
                       int total_ranks,
                       unsigned long long seed,
                       bool do_write_to_file,
                       Heston_Parameters parameters,
                       float strike_price,
                       float discounting_rate ) {

  std::cout << "\tTotal runs : " << std::to_string( total_runs ) << "\n";
  std::cout << "\tTotal timesteps : " << std::to_string( total_timesteps ) << "\n";
  std::cout << "\tTotal Ranks : " << std::to_string( total_ranks ) << "\n";
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
  std::cout << std::endl;
}

// Adapted from https://stackoverflow.com/a/447307
template <class T_type_to_check>
bool Is_Of_Type( std::string string_to_check ) {

  // Effectively uses a string stream to attempt to construct a value 
  // of the given type and checking if fail or bad bits are set. "noskipws"
  // indicates that initial whitespace is not overlooked.

  std::istringstream input_string_stream( string_to_check );
  T_type_to_check testing_value;

  input_string_stream >> testing_value;

  return input_string_stream.eof() && !input_string_stream.fail();
}


// Overloading a string to type function //
void String_To_Type( std::string input_string, unsigned long long* output ) { 
  *output = std::stoull( input_string );
}

void String_To_Type( std::string input_string, int* output ) { 
  *output = std::stoi( input_string );
}

void String_To_Type( std::string input_string, float* output ) { 
  *output = std::stof( input_string );
}
// End of overloading //

template <class T_return_type>
T_return_type Get_Parameter_From_User( std::string parameter_name, std::string type_name, T_return_type default_value ) {

  T_return_type output;
  std::string input_string;
  bool continue_input_loop = true;

  std::cout << "\t[ " << parameter_name << " ] of type [ " << type_name << " ] with default of [ " << std::to_string( default_value ) << " ] : ";

  while( continue_input_loop ) {
    std::cin >> input_string;
    std::cout << std::endl;

    if( input_string == "default" ) {
      output = default_value;
      continue_input_loop = false;
    }
    else if( Is_Of_Type<T_return_type>( input_string ) ) {
      String_To_Type( input_string, &output );
      continue_input_loop = false;
    }
    else {
      std::cout << "\tPlease try again, expected type [ " << type_name << " ] : ";
    }

  }

  return output;
}

template unsigned long long Get_Parameter_From_User( std::string, std::string, unsigned long long );
template int Get_Parameter_From_User( std::string, std::string, int );
template float Get_Parameter_From_User( std::string, std::string, float );

bool Get_Bool_From_User( std::string question_string ) {

  bool output;
  std::string input_string;
  bool continue_input_loop = true;

  std::cout << "\t" << question_string << " ( y / n ) : ";

  while( continue_input_loop ) {
    std::cin >> input_string;
    std::cout << std::endl;

    if( input_string == "y" || input_string == "Y" ) {
      output = true;
      continue_input_loop = false;
    }
    else if( input_string == "n" || input_string == "N"  ) {
      output = false;
      continue_input_loop = false;
    }
    else {
      std::cout << "\tPlease try again, input not recognised : ";
    }

  }

  return output;
}


void User_Parameter_Initialisation( unsigned long long* total_runs, unsigned long long* total_timesteps, unsigned long long* seed, 
                                    bool* do_write_to_file, Heston_Parameters* parameters, 
                                    float* strike_price, float* discounting_rate ) {

  std::cout << "--- User Parameter Initialisation ---\n" << std::endl;

  std::string input_buffer;

  bool do_manual_initialisation = false;
  bool do_continue_initialisation_mode = true;

  while( do_continue_initialisation_mode ) {
    std::cout << "Do manual initialisation (defaults will be applied otherwise)? ( y / n ) : ";
    std::cin >> input_buffer;
    std::cout << std::endl;

    if( input_buffer == "y" || input_buffer == "Y" ) {
      do_manual_initialisation = true;
      do_continue_initialisation_mode = false;
    }
    else if( input_buffer == "n" || input_buffer == "N" ) {
      do_manual_initialisation = false;
      do_continue_initialisation_mode = false;
    }
    else {
      std::cout << "Answer not recognised, please try again" << std::endl;
    }

  }

  std::cout << std::endl;

  if( do_manual_initialisation ) {
    std::cout << "Manual Initialisation : \"default\" can be entered for number types" << std::endl;

    *do_write_to_file = Get_Bool_From_User( "Write to file?" );
    *total_runs = Get_Parameter_From_User<unsigned long long>( "Total Runs", "Unsigned Long Long", MMCOP_DEFAULT_TOTAL_RUNS );
    *total_timesteps = Get_Parameter_From_User<unsigned long long>( "Total Timesteps", "Unsigned Long Long", MMCOP_DEFAULT_TOTAL_TIMESTEPS );
    parameters->timestep = Get_Parameter_From_User<float>( "Timestep", "Float", MMCOP_DEFAULT_TIMESTEP );
    *seed = Get_Parameter_From_User<unsigned long long>( "Seed", "Unsigned Long Long", MMCOP_DEFAULT_SEED );
    parameters->initial_price = Get_Parameter_From_User<float>( "Initial Price", "Float", MMCOP_DEFAULT_INITIAL_PRICE );
    parameters->initial_variance = Get_Parameter_From_User<float>( "Initial Variance", "Float", MMCOP_DEFAULT_INITIAL_VARIANCE );
    parameters->drift = Get_Parameter_From_User<float>( "Drift Factor", "Float", MMCOP_DEFAULT_DRIFT );
    parameters->mean_reversion_speed = Get_Parameter_From_User<float>( "Mean Reversion Speed", "Float", MMCOP_DEFAULT_MEAN_REVERSION_SPEED );
    parameters->mean_reversion_level = Get_Parameter_From_User<float>( "Mean Reversion Level", "Float", MMCOP_DEFAULT_MEAN_REVERSION_LEVEL );
    parameters->volatility = Get_Parameter_From_User<float>( "Volatility", "Float", MMCOP_DEFAULT_VOLATILITY );
    parameters->correlation_factor = Get_Parameter_From_User<float>( "Correlation Factor", "Float", MMCOP_DEFAULT_CORRELATION_FACTOR );
    *strike_price = Get_Parameter_From_User<float>( "Strike Price", "Float", MMCOP_DEFAULT_STRIKE_PRICE );
    *discounting_rate = Get_Parameter_From_User<float>( "Discounting Rate", "Float", MMCOP_DEFAULT_DISCOUNTING_RATE );

  }
  else {

    // Default initialisation
    *do_write_to_file = false;
    *total_runs = MMCOP_DEFAULT_TOTAL_RUNS;
    *total_timesteps = MMCOP_DEFAULT_TOTAL_TIMESTEPS;
    parameters->timestep = MMCOP_DEFAULT_TIMESTEP;
    *seed = MMCOP_DEFAULT_SEED;
    parameters->initial_price = MMCOP_DEFAULT_INITIAL_PRICE;
    parameters->initial_variance = MMCOP_DEFAULT_INITIAL_VARIANCE;
    parameters->drift = MMCOP_DEFAULT_DRIFT;
    parameters->mean_reversion_speed = MMCOP_DEFAULT_MEAN_REVERSION_SPEED;
    parameters->mean_reversion_level = MMCOP_DEFAULT_MEAN_REVERSION_LEVEL;
    parameters->volatility = MMCOP_DEFAULT_VOLATILITY;
    parameters->correlation_factor = MMCOP_DEFAULT_CORRELATION_FACTOR;
    *strike_price = MMCOP_DEFAULT_STRIKE_PRICE;
    *discounting_rate = MMCOP_DEFAULT_DISCOUNTING_RATE;
    std::cout << std::endl;

  }

}


int main( int argc, char* argv[] ) {

  MPI_Init( &argc, &argv );

  int total_ranks;
  int this_rank;
  MPI_Comm_size( MPI_COMM_WORLD, &total_ranks );
  MPI_Comm_rank( MPI_COMM_WORLD, &this_rank );

  // Parameter Declaration
  unsigned long long total_runs;
  unsigned long long total_timesteps;
  unsigned long long seed;
  bool do_write_to_file;
  Heston_Parameters parameters;
  float strike_price;
  float discounting_rate;

  float call_price;

  bool skip_manual_initialisation = false;

  // Parameter Initialisation //

  // Only occurs for first rank

  // Simple CLI parser
  for( int argv_idx = 1; argv_idx < argc; argv_idx++ ) {

    // Checking for skip manual initialisation flag
    if( std::string( argv[ argv_idx ] ) == "-d" || std::string( argv[ argv_idx ] ) == "--default" ) {
      skip_manual_initialisation = true;
    }

  }

  if( IO::Parse_Parameters_From_Arguments( argc, argv, &total_runs, &total_timesteps,
                                           &seed, &do_write_to_file, &parameters,
                                           &strike_price, &discounting_rate ) ) {

    skip_manual_initialisation = true;

  }

  if( !skip_manual_initialisation ) {

    if( this_rank == 0 ) {
    User_Parameter_Initialisation( &total_runs, &total_timesteps, &seed, 
                                  &do_write_to_file, &parameters, &strike_price, &discounting_rate );
    }

  }
  // Parameter Initialisation End //

  Share_Parameters_Over_MPI( &total_runs,
                             &total_timesteps,
                             &seed, 
                             &do_write_to_file,
                             &parameters,
                             &strike_price,
                             &discounting_rate );


  if( this_rank == 0 ) {
    std::cout << "--- Running ---" << std::endl;
    Print_Parameters( total_runs, total_timesteps, total_ranks, seed, do_write_to_file, parameters, strike_price, discounting_rate );
  }

  // Running simulation
  if( do_write_to_file ) {
    // Using Version 0 for write to file
    call_price = Run_Full_MPI_Simulation( total_runs, total_timesteps,
                                          seed, do_write_to_file,
                                          parameters,
                                          strike_price, discounting_rate,
                                          MMCOP_DEFAULT_DO_MULTI_THREADED,
                                          0 );
  }
  else {
    call_price = Run_Full_MPI_Simulation( total_runs, total_timesteps,
                                          seed, do_write_to_file,
                                          parameters,
                                          strike_price, discounting_rate,
                                          MMCOP_DEFAULT_DO_MULTI_THREADED,
                                          MMCOP_DEFAULT_VERSION );
  }

  if( this_rank == 0 ) {
    std::cout << "Results : \n";
    std::cout << "\tCall price : " << std::to_string( call_price ) << "\n";
    std::cout << std::endl;
  }
  MPI_Finalize();

  return 0;
}





#ifndef MMMCOP_HEADER
#define MMMCOP_HEADER


#include "./pre_controls.hpp"

#include <random>

namespace MPI_MONTE_OPTIONS {

typedef enum mmcop_mpi_tags_struct {

  CALL_PRICE_REDUCTION,
  PARAMETER_SHARING

} MMCOP_MPI_Tags;

// I have learned better naming conventions since this
typedef struct heston_parameters_struct {

  float initial_price;
  float initial_variance;
  float timestep;
  float drift;
  float mean_reversion_speed;
  float mean_reversion_level;
  float volatility;
  float correlation_factor;

} Heston_Parameters;

Heston_Parameters Construct_Parameters_Object( float initial_price,
                                               float initial_variance,
                                               float timestep,
                                               float drift,
                                               float mean_reversion_speed,
                                               float mean_reversion_level,
                                               float volatility,
                                               float correlation_factor );


typedef struct simulation_statistics_struct {

  float mean = 0.0f;
  float standard_deviation = 0.0f;

} Simulation_Statistics;


void Simulate_Asset_Price_Walk( unsigned long long total_timesteps,
                                std::vector<float> price_path_buffer,
                                std::mt19937_64* random_engine,
                                std::normal_distribution<float>* normal_distribution_gen,
                                Heston_Parameters parameters );

std::vector<float> Run_Single_Threaded_Simulation( unsigned long long total_runs,
                                            unsigned long long total_timesteps,
                                            unsigned long long seed,
                                            bool do_write_to_file,
                                            Heston_Parameters parameters );

std::vector<float> Run_Multi_Threaded_Simulation( unsigned long long total_runs,
                                            unsigned long long total_timesteps,
                                            unsigned long long seed,
                                            bool do_write_to_file,
                                            Heston_Parameters parameters );

float Compute_Call_Price( std::vector<float>* price_paths,
                          unsigned long long total_runs,
                          unsigned long long total_timesteps,
                          float timestep,
                          float strike_price,
                          float discounting_rate );

float General_Run_Rank_Simulation( unsigned long long total_runs,
                                   unsigned long long total_timesteps,
                                   unsigned long long seed,
                                   bool do_write_to_file,
                                   Heston_Parameters parameters,
                                   float strike_price,
                                   float discounting_rate,
                                   bool do_multi_threaded = MMCOP_DEFAULT_DO_MULTI_THREADED,
                                   int version_to_use = MMCOP_DEFAULT_VERSION );

float Run_Full_MPI_Simulation( unsigned long long total_runs,
                               unsigned long long total_timesteps,
                               unsigned long long seed,
                               bool do_write_to_file,
                               Heston_Parameters parameters,
                               float strike_price,
                               float discounting_rate,
                               bool do_multi_threaded = MMCOP_DEFAULT_DO_MULTI_THREADED,
                               int version_to_use = MMCOP_DEFAULT_VERSION );

void Share_Parameters_Over_MPI( unsigned long long* total_runs,
                                unsigned long long* total_timesteps,
                                unsigned long long* seed, 
                                bool* do_write_to_file,
                                Heston_Parameters* parameters,
                                float* strike_price,
                                float* discounting_rate );

void Send_Parameters_To_Other_Ranks( unsigned long long* total_runs,
                                     unsigned long long* total_timesteps,
                                     unsigned long long* seed, 
                                     bool* do_write_to_file,
                                     Heston_Parameters* parameters,
                                     float* strike_price,
                                     float* discounting_rate );

void Recieve_Parameters_From_Root_Rank( unsigned long long* total_runs,
                                     unsigned long long* total_timesteps,
                                     unsigned long long* seed, 
                                     bool* do_write_to_file,
                                     Heston_Parameters* parameters,
                                     float* strike_price,
                                     float* discounting_rate );


}

#endif



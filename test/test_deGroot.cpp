#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "models/DeGroot.hpp"
#include "network.hpp"
#include <random>

TEST_CASE( "Test the DeGroot Model Symmetric", "[DeGroot]" )
{
    using namespace Seldon;
    using namespace Catch::Matchers;

    size_t n_agents     = 2;
    auto neighbour_list = std::vector<std::vector<size_t>>{
        { 1, 0 },
        { 0, 1 },
    };

    auto weight_list = std::vector<std::vector<double>>{
        { 0.2, 0.8 },
        { 0.2, 0.8 },
    };

    auto gen     = std::mt19937();
    auto network = Network( std::move( neighbour_list ), std::move( weight_list ) );
    auto model   = DeGrootModel( n_agents, network );

    model.convergence_tol   = 1e-6;
    model.max_iterations    = 100;
    model.agents[0].opinion = 0.0;
    model.agents[1].opinion = 1.0;

    do
    {
        model.iteration();
    } while( !model.finished() );

    fmt::print( "N_iterations = {} (with convergence_tol {})\n", model.n_iterations, model.convergence_tol );
    for( size_t i = 0; i < n_agents; i++ )
    {
        fmt::print( "Opinion {} = {}\n", i, model.agents[i].opinion );
        REQUIRE_THAT( model.agents[i].opinion, WithinAbs( 0.5, model.convergence_tol * 10.0 ) );
    }
}
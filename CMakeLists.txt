cmake_minimum_required(VERSION 3.10.0)
project(subset_sum VERSION 0.1.0 LANGUAGES C CXX)

function(configure_target target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>
    )

    target_compile_features(${target} PRIVATE cxx_std_23)
endfunction()

add_subdirectory(source/helpers)
add_subdirectory(source/subset_sum)
add_subdirectory(source/subset_sum_full_search)
add_subdirectory(source/subset_sum_genetic_algorithm)
add_subdirectory(source/subset_sum_genetic_algorithm_parallel)
add_subdirectory(source/subset_sum_hill_climbing)
add_subdirectory(source/subset_sum_sim_annealing)
add_subdirectory(source/subset_sum_tabu_search)

add_executable(subset_sum_genetic_algorithm_parallel)

configure_target(subset_sum_genetic_algorithm_parallel)

target_sources(subset_sum_genetic_algorithm_parallel PRIVATE
    main.cpp
)

target_link_libraries(subset_sum_genetic_algorithm_parallel PRIVATE
    subset_sum
    helpers
)

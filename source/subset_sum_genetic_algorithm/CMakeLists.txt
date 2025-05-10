add_executable(subset_sum_genetic_algorithm)

configure_target(subset_sum_genetic_algorithm)

target_sources(subset_sum_genetic_algorithm PRIVATE
    main.cpp
)

target_link_libraries(subset_sum_genetic_algorithm PRIVATE
    subset_sum
    helpers
)

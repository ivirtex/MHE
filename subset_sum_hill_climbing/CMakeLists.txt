add_executable(subset_sum_hill_climbing)

configure_target(subset_sum_hill_climbing)

target_sources(subset_sum_hill_climbing PRIVATE
    main.cpp
)

target_link_libraries(subset_sum_hill_climbing PRIVATE
    subset_sum
    helpers
)

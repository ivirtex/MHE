add_executable(subset_sum_tabu_search)

configure_target(subset_sum_tabu_search)

target_sources(subset_sum_tabu_search PRIVATE
    main.cpp
)

target_link_libraries(subset_sum_tabu_search PRIVATE
    subset_sum
    helpers
)

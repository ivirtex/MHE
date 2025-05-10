add_executable(subset_sum_sim_annealing)

configure_target(subset_sum_sim_annealing)

target_sources(subset_sum_sim_annealing PRIVATE
    main.cpp
)

target_link_libraries(subset_sum_sim_annealing PRIVATE
    subset_sum
    helpers
)

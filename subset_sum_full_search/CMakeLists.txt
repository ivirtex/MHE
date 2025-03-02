add_executable(subset_sum_full_search)

configure_target(subset_sum_full_search)

target_sources(subset_sum_full_search PRIVATE
    main.cpp
)

target_link_libraries(subset_sum_full_search PRIVATE
    subset_sum
    helpers
)

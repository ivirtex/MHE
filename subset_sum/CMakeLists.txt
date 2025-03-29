add_library(subset_sum STATIC)

configure_target(subset_sum)

target_sources(subset_sum PRIVATE
    include/subset_sum.h
    source/subset_sum.cpp
)

target_include_directories(subset_sum
    PUBLIC include
)

target_link_libraries(subset_sum PRIVATE
    helpers
)

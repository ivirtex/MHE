add_library(helpers STATIC)

configure_target(helpers)

target_sources(helpers PRIVATE
    include/helpers.h
    source/helpers.cpp
)

target_include_directories(helpers
    PUBLIC include
)

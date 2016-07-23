include_directories(${CMAKE_CURRENT_SOURCE_DIR})

subdirs(Designs)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.
add_executable (AlphaLi.Test ${CMAKE_CURRENT_SOURCE_DIR})

# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
target_link_libraries (AlphaLi.Test LINK_PUBLIC AlphaLi)

set_target_properties(AlphaLi.Test PROPERTIES LINKER_LANGUAGE CXX)

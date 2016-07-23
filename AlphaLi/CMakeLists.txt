include_directories(${CMAKE_CURRENT_SOURCE_DIR})

subdirs(Designs)

# Create a library called "AlphaLi" with the source files from subdirs.
# The extension is already found. Any number of sources could be listed here.
add_library (AlphaLi ${CMAKE_CURRENT_SOURCE_DIR})

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (AlphaLi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(AlphaLi PROPERTIES LINKER_LANGUAGE CXX)

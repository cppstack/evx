macro(_add_example name)
    add_executable(${name} ${name}.cpp)
    target_compile_options(${name} PRIVATE -Wall -Wextra -Wpointer-arith)
    target_link_libraries(${name} cst_evx)
endmacro()

_add_example(demo)

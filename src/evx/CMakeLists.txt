add_library(evx
            core/event_loop.cpp
            core/poller_factory.cpp
            )

target_compile_options(evx PRIVATE -Wall -Wextra -Wpointer-arith)

target_include_directories(evx PUBLIC  ${PROJECT_SOURCE_DIR}/include
                               PRIVATE ${PROJECT_SOURCE_DIR}/src/evx)

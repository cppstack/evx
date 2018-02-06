add_library(evx
            core/event_loop.cpp
            core/poller/poller.cpp
            core/poller/poll_poller.cpp
            )

target_compile_options(evx PRIVATE -Wall -Wextra -Wpointer-arith)

target_include_directories(evx PUBLIC  ${PROJECT_SOURCE_DIR}/include
                               PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(evx PRIVATE logging)

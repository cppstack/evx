add_library(evx
            core/event_loop.cpp
            core/watcher.cpp
            core/watchers/io_watcher.cpp
            core/watchers/timer_watcher.cpp
            core/poller.cpp
            core/pollers/poll_poller.cpp
            core/pollers/epoll_poller.cpp
            )

configure_file(${PROJECT_SOURCE_DIR}/config.h.in
               ${PROJECT_BINARY_DIR}/src/evx/config.h)

target_compile_options(evx PRIVATE -Wall -Wextra -Wpointer-arith)

target_include_directories(evx PUBLIC  ${PROJECT_SOURCE_DIR}/include
                               PRIVATE ${PROJECT_SOURCE_DIR}/src
                               PRIVATE ${PROJECT_BINARY_DIR}/src)

target_link_libraries(evx PRIVATE logging)

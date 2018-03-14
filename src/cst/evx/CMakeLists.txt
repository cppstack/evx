find_package(OpenSSL REQUIRED)

add_library(cst_evx
            core/event_loop.cpp
            core/watcher.cpp
            core/watchers/io_watcher.cpp
            core/watchers/timer_watcher.cpp
            core/poller.cpp
            core/pollers/poll_poller.cpp
            core/pollers/epoll_poller.cpp
            core/buffer.cpp
            net/acceptor.cpp
            net/connector.cpp
            net/tcp_connection.cpp
            net/tcp_client.cpp
            net/tcp_server.cpp
            )

configure_file(${PROJECT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

if(LNX_LOGGING)
    target_compile_definitions(cst_evx PRIVATE LNX_LOGGING)
endif()

target_compile_options(cst_evx PRIVATE -Wall -Wextra -Wpointer-arith)

target_include_directories(cst_evx PUBLIC  ${PROJECT_SOURCE_DIR}/include
                                   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(cst_evx PUBLIC  cst_logging cst_lnx
                              PRIVATE ${OPENSSL_LIBRARIES})

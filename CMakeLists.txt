set(BOARD "bbc_microbit")

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(Comando_AT)

message(WARNING "Main file: ${PROJECT_SOURCE_DIR}/src/main.c")

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    )

list(APPEND SOURCES
    "${PROJECT_SOURCE_DIR}/src/uart_driver.c"
    "${PROJECT_SOURCE_DIR}/src/ICLEDMatrix.c"
    "${PROJECT_SOURCE_DIR}/src/CommandAT.c"
    "${PROJECT_SOURCE_DIR}/src/main.c"
    )

list(APPEND HEADERS
    "${PROJECT_SOURCE_DIR}/src/ICLEDMatrix.h"
    "${PROJECT_SOURCE_DIR}/src/CommandAT.h"
    )

target_sources(app PRIVATE ${SOURCES})
target_sources(app PRIVATE ${HEADERS})

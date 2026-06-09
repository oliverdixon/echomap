# ImPlot CMakeLists.txt for WebCFD
#
# Author: Oliver Dixon
# Date: 2026-06-09

set(IMPLOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../implot")

add_library(implot STATIC
        "${IMPLOT_DIR}/implot.h"
        "${IMPLOT_DIR}/implot_internal.h"
        "${IMPLOT_DIR}/implot.cpp"
        "${IMPLOT_DIR}/implot_items.cpp"
)

target_include_directories(implot PUBLIC "${IMPLOT_DIR}")
target_link_libraries(implot PUBLIC imgui)

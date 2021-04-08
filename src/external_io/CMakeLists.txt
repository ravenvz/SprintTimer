add_library(${EXTERNAL_IO_LIB} SHARED)

target_sources(${EXTERNAL_IO_LIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/CsvSerializationAlgorithm.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/CSVReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/OstreamSink.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/RuntimeSinkRouter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/SerializationAlgorithm.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/SerializationException.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/Serializer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/Sink.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/SinkRoutingException.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/SprintToCsvAlgorithm.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/external_io/TaskToCsvAlgorithm.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/CSVReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/CsvSerializationAlgorithm.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/RuntimeSinkRouter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/SprintToCsvAlgorithm.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TaskToCsvAlgorithm.cpp"
)

target_include_directories(${EXTERNAL_IO_LIB} PUBLIC include PUBLIC src)

target_link_libraries(
    ${EXTERNAL_IO_LIB}
    PRIVATE
        sprint_timer_options
        sprint_timer_warnings
    PUBLIC
        ${CORE_LIB}
)

if (WIN32)
    install(TARGETS ${EXTERNAL_IO_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${EXTERNAL_IO_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

set(PROJECT ${SPRINT_MAIN})
project(${SPRINT_MAIN})

add_executable(${PROJECT})

target_compile_features(${PROJECT} PUBLIC cxx_std_17)
target_sources(${PROJECT} PUBLIC "main.cpp")

target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_link_libraries(${PROJECT}
    PRIVATE ${GUI_LIB}
    PRIVATE ${STORAGE_LIB}
)

install(TARGETS ${PROJECT} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})

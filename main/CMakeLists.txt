project(${SPRINT_MAIN})

add_executable(${SPRINT_MAIN})

target_compile_features(${SPRINT_MAIN} PUBLIC cxx_std_17)
target_sources(${SPRINT_MAIN} PUBLIC "main.cpp")

target_compile_options(${SPRINT_MAIN} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${SPRINT_MAIN} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_link_libraries(${SPRINT_MAIN}
    PRIVATE ${GUI_LIB}
    PRIVATE ${STORAGE_LIB}
)

install(TARGETS ${SPRINT_MAIN} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})

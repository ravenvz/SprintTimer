set(CLANG_COMPILE_OPTIONS -Weverything
                          -Werror
                          -Wno-c++98-compat
                          -Wno-c++98-compat-pedantic
                          -Wno-weak-vtables
                          -Wno-padded
                          -Wno-covered-switch-default
                          -Wno-shadow-field-in-constructor
                          -Wno-missing-prototypes
                          -Wno-unused-parameter
                          -Wno-undefined-reinterpret-cast # Qt's mock
                          -Wno-exit-time-destructors
                          -Wno-global-constructors
                          -Wno-zero-as-null-pointer-constant # gtest implementation
                          )

set(GCC_COMPILE_OPTIONS -Wall
                        -Wextra
                        -pedantic
                        -Werror
                        -Wno-unused-parameter
                        -pedantic-errors)


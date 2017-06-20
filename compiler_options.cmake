set(CLANG_COMPILE_OPTIONS -ftemplate-backtrace-limit=0
                          -Weverything
                          -Werror
                          -Wall
                          -pedantic-errors
                          -Wno-c++98-compat
                          -Wno-c++98-compat-pedantic
                          -Wno-weak-vtables
                          -Wno-padded
                          -Wno-covered-switch-default
                          -Wno-shadow-field-in-constructor
                          -Wno-missing-prototypes
                          -Wno-unused-parameter
                          -Wno-undefined-reinterpret-cast # Qt's mock
                          -Wno-zero-as-null-pointer-constant
                          )

set(GCC_COMPILE_OPTIONS -ftemplate-backtrace-limit=0
                        -Wall
                        -Wextra
                        -pedantic
                        -Werror
                        -Wno-unused-parameter
                        -pedantic-errors)


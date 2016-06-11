set(CLANG_COMPILE_OPTIONS -ftemplate-backtrace-limit=0
                          -Weverything
                          -Werror
                          -pedantic-errors
                          -Wno-c++98-compat
                          -Wno-c++98-compat-pedantic
                          -Wno-weak-vtables
                          -Wno-padded
                          -Wno-covered-switch-default
                          -Wno-shadow
                          -Wno-missing-prototypes
                          -Wno-unused-parameter
                          -Wno-global-constructors
                          -Wno-missing-noreturn
                          -Wno-undefined-reinterpret-cast # Qt's mock
                          -Wno-exit-time-destructors)

set(GCC_COMPILE_OPTIONS -ftemplate-backtrace-limit=0
                        -Wall
                        -Wextra
                        -pedantic
                        -Werror
                        -Wno-unused-parameter
                        -pedantic-errors)


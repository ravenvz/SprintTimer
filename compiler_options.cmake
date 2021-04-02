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
                          -Wno-redundant-parens # Qt'mock
                          -Wno-exit-time-destructors
                          -Wno-global-constructors
                          -Wno-zero-as-null-pointer-constant # gtest implementation
                          -Wno-shadow
                          -Wno-used-but-marked-unused # some Qt's static definitions can trigger this
                          -Wno-extra-semi-stmt # Qt mock
                          -Wno-shift-sign-overflow # TODO source is overflow-detection template in Howard Hinnant's date lib
                          -Wno-conversion # TODO source is old release of Howard Hinnant's date lib
                          -Wno-old-style-cast # TODO source is old release of Howard Hinnant's date lib
                          -Wno-gnu-zero-variadic-macro-arguments # gmock
                          -Wno-return-std-move-in-c++11
                          -Wno-c++20-compat
                          )

set(GCC_COMPILE_OPTIONS -Wall
                        -Wextra
                        -pedantic
                        -Werror
                        -Wno-unused-parameter
                        -pedantic-errors)

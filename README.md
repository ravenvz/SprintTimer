### Intro
SprintTimer is a desktop application inspired by Pomodoro Technique®.

The Pomodoro Technique® is a very simple and effective time management technique that helps you keep focused on tasks
 that require long periods of concentration. Read more about it at the official website.
You can read more about technique at the [official website](http://pomodorotechnique.com).


Pomodoro Technique® and Pomodoro™ are registered and filed trademarks owned by Francesco Cirillo. SprintTimer is not affiliated by, associated with nor endorsed by Francesco Cirillo.

### Features and description
TODO

### Building from source

SprintTimer depends on Qt5 framework, Boost libraries, Sqlite3 and requires compiler with C++14 support to build.
It uses CMake build system.

#### Linux

```shell
cd build && cmake -DBUILD_TYPE=Release .. && make
```

#### Windows and Mac OS X
Windows and Mac OS X systems are not yet supported, as building might require some tweaking of
CMakeLists files and changes in the source code of main.cpp.

Windows support is scheduled for the next release.

### Known issues
* Some text labels might be cropped on smaller resolution screens
* Default configuration is not provided, it must be set on first launch manually

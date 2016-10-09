### Intro
SprintTimer is a desktop application inspired by Pomodoro Technique®.

The Pomodoro Technique® is a very simple and effective time management technique that helps you keep focused on tasks
 that require long periods of concentration. Read more about it at the official website.
You can read more about technique at the [official website](http://pomodorotechnique.com).

Pomodoro Technique® and Pomodoro™ are registered and filed trademarks owned by Francesco Cirillo. SprintTimer is not affiliated by, associated with nor endorsed by Francesco Cirillo.

### Features and description
TODO

### Building from source (quick instructions)

SprintTimer depends on Qt5 framework, Boost libraries, Sqlite3 and requires compiler with C++14 support to build.
It uses CMake build system.
Note that it might be required to add directory with Boost installation to CMAKE_INCLUDE_PATH.

#### Linux

```shell
cd build && cmake -DBUILD_TYPE=Release .. && make
```

#### Windows
Windows support is scheduled for the next release.
Experimental support was added on develop branch.

Build instructions can be found on develop branch.

#### Mac OS X
Mas OS X system is not supported yet, as no testing can be performed atm.

### Known issues
* Some text labels might be cropped and some GUI elements can be too big or too small on smaller resolution screens
* Default configuration is not provided, it must be set on first launch manually
* Long break counter resets when app is closed
* Windows version can have many issues, as it wasn't properly tested yet.

### Building from source

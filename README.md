### Intro
SprintTimer is a desktop application inspired by Pomodoro Technique®.

The Pomodoro Technique® is a very simple and effective time management technique that helps you keep focused on tasks
 that require long periods of concentration. Read more about it at the official website.
You can read more about technique at the [official website](http://pomodorotechnique.com).


Pomodoro Technique® and Pomodoro™ are registered and filed trademarks owned by Francesco Cirillo. SprintTimer is not affiliated by, associated with nor endorsed by Francesco Cirillo.

### Features and description
TODO

### Building from source (quick instructions)

SprintTimer depends on Qt5 framework, Boost libraries, SQLite3 and requires compiler with C++14 support to build.
It uses CMake build system.
Depending on your setup, it might be required to add directory with Boost or Qt installation to CMAKE_INCLUDE_PATH.

#### Linux

```shell
cd build && cmake -DBUILD_TYPE=Release .. && make
```

#### Windows
Windows support is not thoroughly tested yet, but application would probably build with MSVC compiler on any Windows version starting from Vista.
Some backporting might be required for older Windows versions.

MinGw builds were not tested yet, but MinGw compiler with C++11 thread support would be required.

1. Make sure you have all dependencies installed (built from source).
2. Open command prompt and navigate to build directory of cloned repo.
3. Generate Visual Studio project files. In following example (with actual paths replaced with placeholders) project files for Visual Studio 2015 are generated:
```shell
cmake .. -G "Visual Studio 14 Win64" -DCMAKE_PREFIX_PATH=<Path to Qt installation>\msvc2015_64 -DCMAKE_INCLUDE_PATH=<Path to Boost> -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DBUILD_SHARED_LIBS=TRUE
```
Note, that this step assumes that you have installed or build from source Qt for your compiler.

You can now open project files with Visual Studio or build it with command line:
```shell
cmake --build . --config Release
```

Before running executable, make sure that Qt installation is added to system path.
Alternatively, you can copy required dll's manually.
Assuming that you have built Release version: navigate to repo/bin/Release,
copy following dll's from Qt installation for your compiler bin folder to repo/bin/Release folder:

Qt5Core.dll
Qt5Gui.dll
Qt5Multimedia.dll
Qt5Sql.dll
Qt5Svg.dll
Qt5Widgets.dll

Copy to the same folder from Qt installation plugins folder following plugins (creating folders as needed):

audio/qtaudio_windows.dll
sqldrivers/qsqlite.dll

#### Mac OS X
Mas OS X system is not supported yet, as no testing can be performed atm.

### Known issues
* Some text labels might be cropped and some GUI elements might be too small/large on smaller resolution screens.
* Long break counter resets when app is closed.
* Windows version can have many issues, as it wasn't properly tested yet.

### Building from source

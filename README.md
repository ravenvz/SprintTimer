### Intro
SprintTimer is a desktop application inspired by Pomodoro Technique®.

The Pomodoro Technique® is a very simple and effective time management technique that helps you keep focused on tasks
 that require long periods of concentration. Read more about it at the official website.
You can read more about technique at the [official website](http://pomodorotechnique.com).


Pomodoro Technique® and Pomodoro™ are registered and filed trademarks owned by Francesco Cirillo. SprintTimer is not affiliated by, associated with nor endorsed by Francesco Cirillo.


### Features and description
SprintTimer is best suited for people that work with computers most of their time.
It is a tool that helps to deal with time-framed tasks effectively. It encourages you to fully concentrate on task at hand for a limited ammount of time (this time interval is called *sprint* in terms of this application). It then encourages you to take a short break and, after some more sprints, a longer break; then the cycle repeats. It is a very simple and widely known technique, that can be used with any timer, paper and pen, but SprintTimer application provides the following benefits:

* it features task list (that is deliberately made small, as it intended to contain task for 1-2 days)
* it stores every task and sprint and is able to answer some statistical queries for any period of time;
* you can easily see what tasks had been done in, say, August 15 of the last year;
* it allows you to quickly observe progress for last 30 days, last 4 months and last year;
* there are times when you are in the flow and don't want to be distracted - just click *InTheZone* button and sprints will be run without breaks and notifications and then be all stored when you finished.

### SprintTimer in action
![action](screenshots/1.gif)

### Building from source

#### Disclaimer
SprintTimer is uncommercial pet project done in spare time. It is written in C++ and is guaranteed to build with compliler supporting the latest standard version (for now it's C++17). The author does not have resources to maintain backports.

#### Dependencies

SprintTimer uses CMake to control compilation process. Minimum required version is 3.11
SprintTimer depends on Qt5 framework, Boost libraries, SQLite3 and requires compiler with C++17 support to build.

#### Linux

```shell
cd build && cmake -DBUILD_TYPE=Release .. && make
```

#### Windows
Windows build is still in experimental stage as it requires MSVC 2017 compiler supplied with Visual Studio 2017 Preview version to build (It supports CMake version 3.8 that is minimum required version).

##### Shell

1. Make sure you have all dependencies installed (or built from source).
2. Open command prompt and navigate to build directory of cloned repo.
3. Generate Visual Studio project files. In following example (with actual paths replaced with placeholders) project files for Visual Studio 2017 are generated:
```shell
cmake .. -G "Visual Studio 15 Win64" -DCMAKE_PREFIX_PATH=<Path to Qt installation>\msvc2017_64 -DCMAKE_INCLUDE_PATH=<Path to Boost> -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DBUILD_SHARED_LIBS=TRUE
```

4. Build project:
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
Qt5Widgets.dll

It might be required to copy to the same folder from Qt installation plugins folder following plugins (creating folders as needed):

audio/qtaudio_windows.dll
sqldrivers/qsqlite.dll

#### Mac OS X
Mas OS X system is not supported yet, as author is not familiar and has no access to this platform at this moment.
Adding support should be trivial though. If someone wants to contribute, please contact author.

#### Building tests
To build test suite option BUILD_TEST=ON should be passed to cmake.

### Known issues
* Some text labels might be cropped and some GUI elements might be too small/large on smaller resolution screens.
* Long break counter resets when app is closed.

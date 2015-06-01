#ifndef ENTITIES_H
#define ENTITIES_H

#include <chrono>
#include <string>


struct Pomodoro {
    std::string name;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point finishTime;
};

#endif // ENTITIES_H 

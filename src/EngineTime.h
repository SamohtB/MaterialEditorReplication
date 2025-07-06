#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>

class Win32App;

class EngineTime
{
public:
    using Clock = std::chrono::high_resolution_clock;

    static void Initialize(int frameRate);

    static float GetDeltaTime();
    static float GetFixedDeltaTime();
    static float GetTimeSinceStart();

    static void UpdateFPSCounter();
    static int GetFPS();

    EngineTime(double frameRate);
    ~EngineTime() = default;
    EngineTime(EngineTime const&) = delete;
    EngineTime& operator=(EngineTime const&) = delete;
private:
    static std::unique_ptr<EngineTime> sharedInstance;

    bool m_firstFrame = true;
    std::chrono::time_point<Clock> m_nextFrame;
    Clock::time_point m_start;
    Clock::time_point m_end;

    double m_deltaTime;
    double m_fixedDeltaTime;
    double m_targetFrameDuration;
    double m_accumulator;

    static int currentFPS;

    static void LogFrameStart();
    static void LogFrameEnd();

   
    friend class Win32App;
};


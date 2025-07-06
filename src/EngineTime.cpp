#include "EngineTime.h"
#include "Debug.h"

std::unique_ptr<EngineTime> EngineTime::sharedInstance = nullptr;
int EngineTime::currentFPS = 0;

void EngineTime::Initialize(int frameRate)
{
    try
    {
        sharedInstance = std::make_unique<EngineTime>(frameRate);
    }
    catch (...)
    {
        Debug::LogError("Engine Time creation failed!");
    }
}

EngineTime::EngineTime(double frameRate) : m_deltaTime(0), m_accumulator(0)
{
    this->m_fixedDeltaTime = 1.0 / 60.0;
    this->m_targetFrameDuration = 1.0 / frameRate;
    Debug::Log("Target Frame Duration Set At: " + std::to_string(this->m_targetFrameDuration));
}

float EngineTime::GetDeltaTime()
{
    return static_cast<float>(sharedInstance->m_deltaTime);
}

float EngineTime::GetFixedDeltaTime()
{
    return static_cast<float>(sharedInstance->m_targetFrameDuration);
}

float EngineTime::GetTimeSinceStart()
{
    return static_cast<float>(sharedInstance->m_accumulator);
}

void EngineTime::LogFrameStart()
{
    if (sharedInstance->m_firstFrame)
    {
        sharedInstance->m_nextFrame = Clock::now();
        sharedInstance->m_firstFrame = false;
    }

    
    sharedInstance->m_start = Clock::now();
}

void EngineTime::LogFrameEnd()
{
    sharedInstance->m_nextFrame += std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<double>(sharedInstance->m_targetFrameDuration)
    );

    auto now = Clock::now();
    if (sharedInstance->m_nextFrame < now)
    {
        sharedInstance->m_nextFrame = now;
    }

    std::this_thread::sleep_until(sharedInstance->m_nextFrame);

    sharedInstance->m_end = Clock::now();

    std::chrono::duration<double> elapsedSeconds = sharedInstance->m_end - sharedInstance->m_start;
    sharedInstance->m_deltaTime = elapsedSeconds.count();
    sharedInstance->m_accumulator += elapsedSeconds.count();
}

void EngineTime::UpdateFPSCounter()
{
    static int frameCount = 0;
    static auto lastTime = Clock::now();

    frameCount++;
    auto now = Clock::now();
    if (now - lastTime >= std::chrono::seconds(1))
    {
        currentFPS = frameCount;
        frameCount = 0;
        lastTime = now;
    }
}

int EngineTime::GetFPS()
{
    return currentFPS;
}

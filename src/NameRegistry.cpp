#include "NameRegistry.h"
#include "Debug.h"

std::unique_ptr<NameRegistry> NameRegistry::sharedInstance = nullptr;

NameRegistry* NameRegistry::GetInstance()
{
    return sharedInstance.get();
}

void NameRegistry::Initialize()
{
    try
    {
        sharedInstance = std::make_unique<NameRegistry>();
    }
    catch (...)
    {
        Debug::LogError("Name Registry Creation Failed!");
    }
}

void NameRegistry::Destroy()
{
    sharedInstance.reset();
    sharedInstance = nullptr;
}

NameRegistry::String NameRegistry::GenerateUniqueName(const String& baseName)
{
    int& count = m_nameCounters[baseName];
    String candidate;

    do 
    {
        candidate = baseName + "_" + std::to_string(count++);
    } 
    while (m_registeredNames.count(candidate) > 0);

    m_registeredNames.insert(candidate);
    return candidate;
}

void NameRegistry::RegisterName(const String& name)
{
    m_registeredNames.insert(name);
}

void NameRegistry::UnregisterName(const String& name)
{
    m_registeredNames.erase(name);
}

bool NameRegistry::IsNameRegistered(const String& name) const
{
    return m_registeredNames.count(name) > 0;
}
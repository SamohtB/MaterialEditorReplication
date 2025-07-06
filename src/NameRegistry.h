#pragma once
#include "pch.h"

class NameRegistry
{
public:
    using String = std::string;

    static NameRegistry* GetInstance();
    static void Initialize();
    static void Destroy();

    String GenerateUniqueName(const String& baseName);
    void RegisterName(const String& name);
    void UnregisterName(const String& name);
    bool IsNameRegistered(const String& name) const;

    NameRegistry() = default;
    ~NameRegistry() = default;
    NameRegistry(NameRegistry const&) = delete;
    NameRegistry& operator=(NameRegistry const&) = delete;
private:
    static std::unique_ptr<NameRegistry> sharedInstance;

    std::unordered_map<String, int> m_nameCounters;
    std::unordered_set<String> m_registeredNames;
};

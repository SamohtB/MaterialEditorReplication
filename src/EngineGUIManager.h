#pragma once
#include "pch.h"

class UINames
{
public:
    const std::string TOOLBAR = "TOOLBAR";
    const std::string HIERARCHY = "HIERARCHY";
    const std::string INSPECTOR = "INSPECTOR";
    const String CREDITS = "CREDITS";
    const String COLOR_PICKER = "COLOR_PICKER";
};

class AUIScreen;

class EngineGUIManager
{
public:
	using UIPtr = std::shared_ptr<AUIScreen>;
    using UIList = std::vector<UIPtr>;
    using UITable = std::unordered_map<String, UIPtr>;

    static EngineGUIManager* GetInstance();
    static void Initialize(HWND hwnd, UINT width, UINT height);
    static void Destroy();

    void DrawAllUI();
    std::vector<AUIScreen*> GetAllScreens();

    AUIScreen* GetUI(String uiName);

    EngineGUIManager(HWND hwnd);
    ~EngineGUIManager();
    EngineGUIManager(EngineGUIManager const&) = delete;
    EngineGUIManager& operator=(EngineGUIManager const&) = delete;
private:
    static std::unique_ptr<EngineGUIManager> sharedInstance;

    UIList m_uiList;
    UITable m_uiTable;

    void PopulateGUI();
};
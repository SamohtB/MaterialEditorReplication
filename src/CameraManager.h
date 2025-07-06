#pragma once
#include "pch.h"
#include "Math.h"
#include "InputListener.h"

class Camera;

class CameraManager : public InputListener
{
public:
	using CameraPtr = std::shared_ptr<Camera>;
	using CameraList = std::vector<CameraPtr>;
	using CameraMap = std::unordered_map<UINT, CameraPtr>;

	enum PossessionState
	{
		UNPOSSESSED,
		POSSESSED,
		PAUSED
	};

	static CameraManager* GetInstance();
	static void Initialize(UINT width, UINT height);
	static void Destroy();

	int AddCamera(const CameraPtr& reference, bool setMain = false);
	
	Matrix GetActiveCameraViewMatrix();
	Matrix GetActiveCameraProjMatrix();
	Vector3 GetActiveCameraPosition();

	void Update(float deltaTime);
	void UpdateViewportSize(UINT width, UINT height);

	void PossessCamera(int index);
	void PossessCamera(const CameraPtr& reference);
	void UnpossessCamera();

	const CameraPtr& GetActiveCamera() const;
	const CameraPtr& GetSceneCamera() const;

	/* Input Stuff */
	virtual void OnKeyPressed(int key) override;

	/* Singleton Stuff */
	CameraManager(UINT width, UINT height);
	~CameraManager() = default;
	CameraManager(CameraManager const&) = delete;
	CameraManager& operator=(CameraManager const&) = delete;

private:
	void CameraSwitcher();
	void CycleCameras();
	void ResumePossess();
	void CopyPositionToSceneCamera(const CameraPtr& reference);
	
	static std::unique_ptr<CameraManager> sharedInstance;

	CameraPtr m_sceneCamera = nullptr;
	CameraPtr m_activeCamera = nullptr;
	CameraPtr m_previousCamera = nullptr;

	CameraMap m_cameraMap;
	CameraList m_cameraList;

	UINT m_cameraIndex = 0;
	UINT m_cameraCycleTracker = 1;
	PossessionState m_possessionState = UNPOSSESSED;
};


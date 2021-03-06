#pragma once
#include <random>
#include <memory>
#include "BaseScene.h"
#include "../Object/Object.h"
#include "../Graphic/StringAddDraw.h"
#include <functional>

class camera;

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:

	void Run(void);

	static SceneMng &GetInstance(void)
	{
		Create();
		return *sInstance;
	};

	static void Create()
	{
		if (sInstance == nullptr)
		{
			sInstance = new SceneMng();
		}
	};

	static void Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	};

	unsigned int GetFlame(void) { return _flame; };

	const Vector2 GetPlPos(TIME time) const;
	const std::shared_ptr<Object> GetPlObj2(TIME time) const;
	const std::shared_ptr<Object>* GetPlObj(TIME time) const;
	void SetPlObj(std::shared_ptr<Object>& plObj, TIME time);

	const Vector2D GetcPos(void) const;																// iranai
	const std::shared_ptr<Vector2D> GetccPos(void) const;             // iranai
	void SetcPos(std::shared_ptr<Vector2D> cPos);
	const std::shared_ptr<camera>& GetcObj(void) const;
	void SetcObj(std::shared_ptr<camera>& cObj);

	void SetNum(int num);			// _workのset
	int GetNum(void);				// _workのget

	void Quit(void);
	void ChangeScene(BaseScene* scene);

	std::mt19937 _rnd;

	const Vector2 ScreenSize = { 1280,720 };
private:
	static SceneMng *sInstance;
	std::unique_ptr<BaseScene> _activeScene;
	std::map<TIME, std::shared_ptr<Object>> _plObj;
	std::shared_ptr<camera> _cObj;
	std::shared_ptr<Vector2D> _cPos;									// カメラ座標

	unsigned int _flame;

	int _work;				// gameover用スクリーン

	bool SysInit(void);

	std::function<void(void)> _sceneChange = []() {};
	bool _exit = false;
	
	SceneMng();
	SceneMng(const SceneMng&) = delete;
	SceneMng operator=(const SceneMng&) = delete;
	~SceneMng();
	
};

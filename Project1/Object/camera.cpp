#include "camera.h"
#include "../Manage/MapMng.h"

void camera::Update(void)
{
	// 思ってたのと違ったがこれはこれでいい
	_oldPlPos[0] = _oldPlPos[0] + ((_oldPlPos[3] - _oldPlPos[0]) * 0.25);
	_oldPlPos[1] = _oldPlPos[0] + ((_oldPlPos[3] -_oldPlPos[0])* 0.5);
	_oldPlPos[2] = _oldPlPos[0] + ((_oldPlPos[3] - _oldPlPos[0]) * 0.75);
	// 思ってたのと違ったがこれはこれでいい
	_cPos->x = _oldPlPos[0].x + _cOffSet.x;
	_cPos->y = _oldPlPos[0].y + _cOffSet.y;

	//for (int i = 0; i < 3; i++)
	//{
	//	_oldPlPos[i] = _oldPlPos[i + 1];
	//}

	_oldPlPos[3] = _plObj.at(lpTimeMng.getTime())->getPos();
	CheckPos();
}

bool camera::CheckPos(void)
{
	if (_cPos->x < _cEndOffSet.x1)
	{
		_cPos->x = _cEndOffSet.x1;
	}

	if (_cPos->x > _cEndOffSet.x2)
	{
		_cPos->x = _cEndOffSet.x2;
	}

	if (_cPos->y <  490)		// なぜ490だ？
	{
		_cPos->y = 490;
	}

	return false;
}

void camera::Draw(void)
{
}

void camera::SetcEndOffSet(Vector2&& cEndOffSet)
{
	_cEndOffSet = cEndOffSet;
}

camera::camera() : _cPos{ new Vector2D(0,0) }
{
	_plObj.emplace(TIME::NOW, lpSceneMng.GetPlObj2(TIME::NOW));
	_plObj.emplace(TIME::FTR, lpSceneMng.GetPlObj2(TIME::FTR));

	_cOffSet = { 0,-140 };

	_cEndOffSet = { lpSceneMng.ScreenSize.x / 2, lpMapMng.GameMapSize.x - (lpSceneMng.ScreenSize.x / 2) };

	_cPos->x = _plObj.at(lpTimeMng.getTime())->getPos().x;
	_cPos->y = _plObj.at(lpTimeMng.getTime())->getPos().y;
	lpSceneMng.SetcPos(_cPos);
	for (int i = 0; i < 10; i++)
	{
		_oldPlPos[i] = _plObj.at(lpTimeMng.getTime())->getPos();
	}
	_stage = -1;
}

camera::~camera()
{
}
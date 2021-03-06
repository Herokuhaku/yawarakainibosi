#include "s_dragon.h"
#include "../../Scene/SceneMng.h"
#include "../Attack/AttackMng.h"
#include "../../Graphic/ImageMng.h"
#include "../../Manage/MapMng.h"

void s_dragon::Update(void)
{
	Enemy::Update();
}

int s_dragon::Attack(Vector2& pos)
{
	if (_state_dir.first != OBJ_STATE::ATTACK)
	{
		setState({ OBJ_STATE::ATTACK, _plDir });
		lpAtkMng.MakeFireBall({ _pos.x,_pos.y - 20 }, _plDir, { 5, 0 }, _time, _stage, OBJ_TYPE::PLAYER	);
		_waitTime = 140;	// クールタイム
		_waitCnt = 0;
		return static_cast<int>(MOVE_SELECT::WAIT);
	}
	return static_cast<int>(MOVE_SELECT::WAIT);
}

//s_dragon::s_dragon(Vector2Template<int> pos, double rad, TIME time, int stage, OBJ_TYPE type, std::pair<OBJ_STATE, DIR> state_dir)
s_dragon::s_dragon(Vector2 pos, int stage)
	{
	_pos = pos;
	_stage = stage;
	_state_dir = { OBJ_STATE::NORMAL,DIR::RIGHT };
	setHP(100);
	setHitOffset({ 55, 55, 100, 0 });
	
	while (!lpMapMng.getHitMap(_pos, _stage))
	{
		_pos.y += 1;
	}
	Init();
}

s_dragon::s_dragon(Vector2 pos, int stage, int pPos, bool flag)
{
	_pos = pos;
	_stage = stage;
	_etype = ENEMY_TYPE::s_dragon;
	_pPos = pPos;
	_state_dir = { OBJ_STATE::NORMAL,DIR::RIGHT };
	setHP(100);
	setHitOffset({ 55, 55, 100, 0 });
	if (flag)
	{
		while (!lpMapMng.getHitMap(_pos, _stage))
		{
			_pos.y += 1;
		}
	}
	Init();
}

s_dragon::~s_dragon()
{
}

void s_dragon::Init(void)
{
	AnmVec data;
	// 右-------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonR")[4 + i], 20 + i * 20);
	}
	setAnm({ OBJ_STATE::NORMAL,DIR::RIGHT }, data);

	for (int i = 0; i < 4; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonR")[i], 20 + i * 20);
	}
	setAnm({ OBJ_STATE::WALK,DIR::RIGHT }, data);

	for (int i = 0; i < 3; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonR")[16 + i], 8 + i * 3);
	}
	data.emplace_back(-1, 0);
	setAnm({ OBJ_STATE::ATTACK,DIR::RIGHT }, data);

	for (int i = 0; i < 2; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonR")[8 + i], 20 + i * 20);
	}
	data.emplace_back(-1,0);
	setAnm({ OBJ_STATE::DAMAGE,DIR::RIGHT }, data);

	for (int i = 0; i < 3; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonR")[12 + i], 20 + i * 20);
	}
	data.emplace_back(-1,2828);
	setAnm({ OBJ_STATE::DEAD,DIR::RIGHT }, data);

	// 左-------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonL")[4 + i], 20 + i * 20);
	}
	setAnm({ OBJ_STATE::NORMAL,DIR::LEFT }, data);

	for (int i = 0; i < 4; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonL")[i], 20 + i * 20);
	}
	setAnm({ OBJ_STATE::WALK,DIR::LEFT }, data);

	for (int i = 0; i < 3; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonL")[16 + i], 8 + i * 3);
	}
	data.emplace_back(-1, 0);
	setAnm({ OBJ_STATE::ATTACK,DIR::LEFT }, data);

	for (int i = 0; i < 2; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonL")[8 + i], 20 + i * 20);
	}
	data.emplace_back(-1,0);
	setAnm({ OBJ_STATE::DAMAGE,DIR::LEFT }, data);

	for (int i = 0; i < 3; i++)
	{
		data.emplace_back(lpImageMng.getImage("s_dragonL")[12 + i], 20 + i * 20);
	}
	data.emplace_back(-1, 2828);
	setAnm({ OBJ_STATE::DEAD,DIR::LEFT }, data);

	_etype = ENEMY_TYPE::s_dragon;
	_rangeA = 200;
}

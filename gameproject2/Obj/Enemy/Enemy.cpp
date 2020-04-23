#include "Enemy.h"
#include "../../Scene/SceneMng.h"

//void Enemy::Update(std::shared_ptr<Object> plObj);
//{
//}

void Enemy::Update(void)
{
	auto tmp = lpSceneMng.GetPlObj();
	auto plPos = (*tmp)->getPos();
	_work = (this->*autoM[_aState])(plPos);
	_work =	Search(plPos);
	aState(_work);
	Draw();
}

int Enemy::Wait(Vector2 pPos)
{
	_waitCnt++;
	if (_waitCnt >= _waitTime)
	{
		setState({ OBJ_STATE::WALK, _plDir });
		return static_cast<int>(MOVE_SELECT::MOVE);
	}
	return _aState;
}

int Enemy::Move(Vector2 pPos)
{
	if (!_encntF)
	{
		if (_state_dir.second == DIR::RIGHT)
		{
			_pos.x += lpSceneMng.GetFlame() % 2;
			if (_pos.x >= 400)
			{
				setState({ OBJ_STATE::WALK,DIR::LEFT });
			}
		}
		else
		{
			_pos.x -= lpSceneMng.GetFlame() % 2;
			if (_pos.x <= 0)
			{
				setState({ OBJ_STATE::WALK,DIR::RIGHT });
			}
		}
		return _aState;
	}
	else
	{
		return 	AtkMove(pPos);
	}
}

int Enemy::Search(Vector2 pPos)
{
	_plDir = pPos.x > _pos.x ? DIR::RIGHT : DIR::LEFT;
	if (_waitCnt >= _waitTime)
	{
		if ((pPos.x - _pos.x)  * (static_cast<int>(_state_dir.second) - 1) <= _rangeS &&
			(pPos.x - _pos.x)  * (static_cast<int>(_state_dir.second) - 1) >= -80)
		{
			if ((pPos.x - _pos.x) * (static_cast<int>(_state_dir.second) - 1) <= _rangeA)
			{
				aState(static_cast<int>(MOVE_SELECT::ATTACK));
				lpImageMng.AddDraw({ lpImageMng.getImage("excPoint")[0],_pos.x,_pos.y - 40,0.0,LAYER::EX,10 });
				return _aState;
			}
			lpImageMng.AddDraw({ lpImageMng.getImage("queMark")[0],_pos.x,_pos.y - 40,0.0,LAYER::EX,10 });
			_encntF = true;
			return static_cast<int>(MOVE_SELECT::MOVE);
		}
		else
		{
			_encntF = false;
		}
	}
	return _work;
}

int Enemy::Attack(Vector2 pPos)
{
	if (_state_dir.first != OBJ_STATE::ATTACK)
	{
		setState({ OBJ_STATE::ATTACK, _state_dir.second });
		_waitTime = 140;	// クールタイム
		_waitCnt = 0;
		return static_cast<int>(MOVE_SELECT::WAIT);
	}
	return static_cast<int>(MOVE_SELECT::WAIT);
}

int Enemy::AtkMove(Vector2 pPos)
{
	if (_state_dir.second == DIR::RIGHT)
	{
		//_pos.x += lpSceneMng.GetFlame() % 2;
		_pos.x += 2;
	}
	else
	{
		//_pos.x -= lpSceneMng.GetFlame() % 2;
		_pos.x -= 2;
	}
	return _aState;
}

Enemy::Enemy()
{
	_aState = 0;
	Init();
}

void Enemy::Init(void)
{
	_waitTime = std::rand() % 180;
	_waitCnt = 0;
	_waitF = true;
	_rangeS = 200;
	_rangeA = 80;
	_encntF = false;
	_plDir = DIR::RIGHT;
}

void Enemy::aState(int work)
{
	if ( work < e_fn_ptr_num)
	{
		_aState = work;
	}
	else
	{
		// エラー
		_aState = 0;
	}
}

Enemy::~Enemy()
{
}


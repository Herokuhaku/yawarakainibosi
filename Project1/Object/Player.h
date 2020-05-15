#pragma once
#include <DxLib.h>
#include <cmath>
#include "Object.h"
#include "../Manage/KeyMng.h"
#include "../func/CheckHitStage.h"


#define PLAYER_SIZE_X	85
#define PLAYER_SIZE_Y	90

#define DELAY_FIRE		220

#define HP_MAX			100


class Player :
	public Object
{
public:
	Player();
	Player(Vector2Template<int> pos, int stage, TIME time);
	~Player();

	void Update(void)override;

	void Draw(void)override;
private:
	static constexpr int WALK_SPEED = 4;
	static constexpr double G_ACC_NORMAL = 0.4;
	static constexpr double INI_VEL_NORMAL = 9.0;
	static constexpr double VEL_MAX = 9.0;

	void Init(void);

	void ControlNormal(void);
	void ControlAttack(void);
	void (Player::*_control)(void); // 上2つの関数ポインタ

	void StopWalk(void);

	void MagicUpdate(void);
	int _coolTime;

	void StateRotate(void);

	bool _rotateFlag;

	void VelUpdate(void);


	double _vel;
	//
	bool MenuUpdate(void);

	bool MenuFlag;

};


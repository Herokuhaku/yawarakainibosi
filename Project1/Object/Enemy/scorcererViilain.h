#pragma once
#include "Enemy.h"

class sorcerer :
	public Enemy
{
public:
	void Update(void);
	sorcerer(Vector2 pos,int stage);
	sorcerer(Vector2 pos,int stage,int pPos, bool flag);		// lpMapMngの中でループするからEnemyの座標を下に下げるやつを除いたもの
	~sorcerer();
private:
	void Init(void);
};

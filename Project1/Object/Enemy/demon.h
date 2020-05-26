#pragma once
#include "Enemy.h"

class demon :
	public Enemy
{
public:
	void Update(void);
	demon(Vector2 pos,int stage);
	demon(Vector2 pos,int stage,int pPos, bool flag);		// lpMapMngの中でループするからEnemyの座標を下に下げるやつを除いたもの
	~demon();
private:
	void Init(void);
};

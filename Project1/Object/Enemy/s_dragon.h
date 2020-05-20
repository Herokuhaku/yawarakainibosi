#pragma once
#include "Enemy.h"

class s_dragon :
	public Enemy
{
public:
	void Update(void);
	s_dragon(Vector2 pos,int stage);
	s_dragon(Vector2 pos,int stage,int pPos, bool flag);		// lpMapMngの中でループするからEnemyの座標を下に下げるやつを除いたもの
	~s_dragon();
private:
	void Init(void);
};

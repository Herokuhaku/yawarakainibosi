#pragma once
#include "Enemy.h"

class wizard :
	public Enemy
{
public:
	void Update(void);
	wizard(Vector2 pos,int stage);
	wizard(Vector2 pos,int stage,int pPos, bool flag);		// lpMapMng�̒��Ń��[�v���邩��Enemy�̍��W�����ɉ�����������������
	~wizard();
private:
	void Init(void);
};
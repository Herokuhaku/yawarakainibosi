#pragma once
#include "../common/Vector2.h"
#include <array>
#include "../Audio/AudioContainer.h"

struct ItemSave;

class Gimmick
{
public:
	Gimmick() = default;
	Gimmick(const Vector2Template<int>& pos, int stage);
	virtual ~Gimmick() = default;

	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	bool isDeleted(void);

	const Vector2Template<int>& GetPos(void)const;
	const int GetStage(void)const;
	const std::array<int, 4>& GetHitBox(void)const;
	const std::array<int, 4>& GetUsable(void)const;

	virtual void Action(const ItemSave& item);
protected:
	// 座標
	Vector2Template<int> _pos;
	// ステージ
	int _stage;
	// 当たり判定
	std::array<int, 4> _hitBox;
	// アイテム使用可能範囲
	std::array<int, 4> _usableRange;
	// 削除していいか
	bool _deleted = false;

	std::shared_ptr<AudioContainer> _audio;
};


#include "AttackUI.h"
#include "ButtonMng.h"
#include "../Graphic/ImageMng.h"

AttackUI* AttackUI::sInstance = nullptr;

void AttackUI::Update(void)
{
	lpButtonMng.GetThumb(THUMB_RIGHT, _stickX, _stickY);
}

void AttackUI::Draw(void)
{
	// 現在の描画先を退避
	int tmpScreen = GetDrawScreen();

	// 描画
	SetDrawScreen(_uiScreen);
	ClsDrawScreen();

	DrawGraph((UI_SIZE - RING_SIZE) / 2, (UI_SIZE - RING_SIZE) / 2, lpImageMng.getImage("base_ring")[0], true);
	DrawRotaGraph(UI_SIZE / 2 + STICK_TO_POS(_stickX), UI_SIZE / 2 - STICK_TO_POS(_stickY), 1.0, 0.0, lpImageMng.getImage("stick_obj")[0], true);
	
	// 描画先を戻す
	SetDrawScreen(tmpScreen);

	// スクリーンを描画してもらう
	lpImageMng.AddBackDraw({ _uiScreen, DRAW_OFFSET_X, DRAW_OFFSET_Y, 1.0, 0.0, LAYER::EX, 500, DX_BLENDMODE_NOBLEND, 0 });
}

AttackUI::AttackUI()
{
	// スクリーンの作成
	_uiScreen = MakeScreen(UI_SIZE, UI_SIZE, true);

	// 素材読み込み
	lpImageMng.getImage("image/UI/AttackUIRing.png", "base_ring");
	lpImageMng.getImage("image/UI/StickObj.png", "stick_obj", 60, 60, 8, 1);

	// 初期化
	_stickX = 0;
	_stickY = 0;
}

AttackUI::~AttackUI()
{
	DeleteGraph(_uiScreen);
}

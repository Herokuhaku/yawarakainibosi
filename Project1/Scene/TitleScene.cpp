#include "TitleScene.h"
#include "../Graphic/ImageMng.h"
#include "SceneMng.h"
#include "SeleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "../Manage/KeyMng.h"
#include "../Manage/ButtonMng.h"

TitleScene::TitleScene()
{
	lpImageMng.getImage("image/hurosiki.png", "���C�~");
}

TitleScene::~TitleScene()
{
}

std::unique_ptr<BaceScene> TitleScene::Update(std::unique_ptr<BaceScene> own)
{
	lpImageMng.AddBackDraw({ lpImageMng.getImage("���C�~")[0], lpSceneMng.ScreenSize.x / 2, lpSceneMng.ScreenSize.y / 2, 1.0, 0.0, LAYER::BG, 0, DX_BLENDMODE_NOBLEND, 0 });

	if ((lpKeyMng.getBuf()[KEY_INPUT_RETURN] && !lpKeyMng.getOldBuf()[KEY_INPUT_RETURN]) ||
		(lpButtonMng.Buttonf(0, XINPUT_BUTTON_B).first == 1 && lpButtonMng.Buttonf(0, XINPUT_BUTTON_B).second == 0))
	{
		lpImageMng.setGkind(ScrEff::FADEOUT);
		return std::make_unique<GameScene>();
	}

	return std::move(own);
}

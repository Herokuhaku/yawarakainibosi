#include "ItemMenu.h"
#include "MenuExecuter.h"
#include "../Manage/ButtonMng.h"
#include "../Graphic/StringAddDraw.h"
#include "../Scene/SceneMng.h"
#include "../Graphic/ImageMng.h"
#include "../Audio/AudioContainer.h"
#include "SelectMenu.h"

ItemMenu::ItemMenu(std::shared_ptr<MenuExecuter> exe, std::shared_ptr<AudioContainer> audio) : MenuBase(exe, audio)
{
	for (int i = 0;i < _asize;i++)
	{
		_sItem.at(i).first.itemtype = ITEM_TYPE::NON;
		_sItem.at(i).second = i;
	}
	_cpos = lpSceneMng.ScreenSize / 2;
	_offpush.y = 150;
	_offpush.x = 200;
	_count = { 2, 1 };
	lpImageMng.getImage("image/メニュー.png", "Menu");
	lpImageMng.getImage("image/Space.png", "Space");
	lpImageMng.getImage("image/arrow.png", "Arrow");
	lpImageMng.getImage("image/合成.png", "Mix");
	lpImageMng.getImage("image/UI/select_box.png", "s_box");

	mix_.itemtype = ITEM_TYPE::NON;
	exe_ = exe;
}

ItemMenu::~ItemMenu()
{
}

void ItemMenu::Update(void)
{
	if (!MixFlag)
	{
		if (mix_.itemtype == ITEM_TYPE::NON)
		{
			// 矢印操作(スティック)
			SelectCount(_select, XINPUT_THUMBL_X, _count.x);

			// 矢印を右に移動させる
			if (lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_DPAD_RIGHT))
			{
				PlaySoundMem(_audio->GetSound("move"), DX_PLAYTYPE_BACK, true);
				_select++;
				if (_select > _count.x)
				{
					_select = 0;
				}
			}
			//　矢印を左に移動させる
			if (lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_DPAD_LEFT))
			{
				PlaySoundMem(_audio->GetSound("move"), DX_PLAYTYPE_BACK, true);
				_select--;
				if (_select < 0)
				{
					_select = _count.x;
				}
			}

			// 矢印を上にずらす
			if (lpButtonMng.Thumbf(0, XINPUT_THUMBL_Y).first == 2 && lpButtonMng.Thumbf(0, XINPUT_THUMBL_Y).second == 0)
			{
				PlaySoundMem(_audio->GetSound("move"), DX_PLAYTYPE_BACK, true);
				_select2++;
				if (_select2 > _count.y)
				{
					_select2 = 0;
				}
			}
			// 矢印を下にずらす
			if (lpButtonMng.Thumbf(0, XINPUT_THUMBL_Y).first == 1 && lpButtonMng.Thumbf(0, XINPUT_THUMBL_Y).second == 0)
			{
				PlaySoundMem(_audio->GetSound("move"), DX_PLAYTYPE_BACK, true);
				_select2--;
				if (_select2 < 0)
				{
					_select2 = _count.y;
				}
			}

			// 決定描画
			lpStrAdd.AddStringDraw("決定", "SQUARE_BIG", _cpos.x, _cpos.y + 75, 0xffffff, DRAW_TO_CENTER);

			MixDraw();

			// アイテムを選択
			if (lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_B))
			{
				PlaySoundMem(_audio->GetSound("select"), DX_PLAYTYPE_BACK, true);
				if (_select2 == 0)
				{
					lpButtonMng.Buttonf(0, XINPUT_BUTTON_B, 1, 1);
					_selectNo = static_cast<SELECT_ITEM>(_select);
					_select = 0;
					MixFlag = true;
					_count.x = 5;
					_count.y = 2;
				}
				else if (_select2 == 1)
				{
					lpButtonMng.Buttonf(0, XINPUT_BUTTON_B, 1, 1);
					int count = 0;
					for (int i = 0;i < _asize;i++)
					{
						if (_sItem.at(i).first.itemtype != ITEM_TYPE::NON)
						{
							count++;
						}
					}
					if (count >= 2)
					{
						if (Mix(_sItem.at(0).first, _sItem.at(1).first, _sItem.at(2).first))
						{
							mix_ = _sItem.at(0).first;
							// アイテムの二つ目を消すため合成リストからも消す
							for (int i = 0;i < _asize;i++)
							{
								_sItem.at(i).first.itemtype = ITEM_TYPE::NON;
							}
						}
					}
				}
			}
			// 戻る
			if (lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_A))
			{
				PlaySoundMem(_audio->GetSound("cancel"), DX_PLAYTYPE_BACK, true);
				exe_->ChangePage(new SelectMenu(0, exe_, _audio));
			}
		}
		else
		{
			lpStrAdd.AddStringDraw("完成！", "SQUARE_BIG", _cpos.x+2, _cpos.y - 200+2, 0x000000, DRAW_TO_CENTER);
			lpStrAdd.AddStringDraw("完成！", "SQUARE_BIG", _cpos.x, _cpos.y - 200, 0xff0000, DRAW_TO_CENTER);
			mix_.pos = { _cpos.x ,_cpos.y };
			lpImageMng.AddBackDraw({ lpImageMng.getImage(mix_.image[0])[0],mix_.pos.x,mix_.pos.y, 3.0f, 0.0,LAYER::EX, _zorder.item, DX_BLENDMODE_NOBLEND, 0, true });

			if (lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_A) || lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_B))
			{
				PlaySoundMem(_audio->GetSound("select"), DX_PLAYTYPE_BACK, true);
				mix_.itemtype = ITEM_TYPE::NON;
			}
		}
	}
	else
	{
		Item(_selectNo);
	}
}

void ItemMenu::Draw(void)
{
}

bool ItemMenu::Mix(ItemSave& item1, ItemSave& item2, ItemSave& item3)
{
	if ((item1.itemtype == ITEM_TYPE::BOOK || item2.itemtype == ITEM_TYPE::BOOK) &&
		(item1.itemtype == ITEM_TYPE::STONE || item2.itemtype == ITEM_TYPE::STONE))
	{
		if (Dynamite(item1, item2)) { return true; }	// ダイナマイトのレシピ
		else if (Vine(item1, item2)) { return true; }	// 蔓のレシピ
		else if (Bard(item1, item2)) { return true; }	// 鳥のレシピ
		else if (Key(item1, item2)) { return true; }	// 鍵のレシピ
		else if (Powder(item1, item2)) { return true; }	// 粉レシピ
		else if (Hose(item1, item2)) { return true; }	// ホースのレシピ
		else { return false; }
	}
	else if (item1.itemtype != ITEM_TYPE::STONE || item2.itemtype != ITEM_TYPE::STONE)
	{
		return false;
	}

	// 石の色合成
	if (!lpTradeMng.ColorTradeCheck(item1.colortype, item2.colortype))
	{
		return false;
	}

	item1.colortype = lpTradeMng.Trade(item1.colortype, item2.colortype);
	lpTradeMng.AddBag(item1);
	lpTradeMng.DeleteItem(item2);

	return true;
}

bool ItemMenu::Dynamite(ItemSave& item1, ItemSave& item2)
{
	if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::RED) ||
		item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::RED)
	{
		if ((item1.itemtype == ITEM_TYPE::BOOK && item1.colortype == COLOR::RED) ||
			(item2.itemtype == ITEM_TYPE::BOOK && item2.colortype == COLOR::RED))
		{
			item1.colortype = COLOR::RED;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	return false;
}
bool ItemMenu::Vine(ItemSave& item1, ItemSave& item2)
{
	if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::GREEN) ||
		item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::GREEN)
	{
		if ((item1.itemtype == ITEM_TYPE::BOOK && item1.colortype == COLOR::GREEN) ||
			(item2.itemtype == ITEM_TYPE::BOOK && item2.colortype == COLOR::GREEN))
		{
			item1.colortype = COLOR::GREEN;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	return false;
}
bool ItemMenu::Bard(ItemSave& item1, ItemSave& item2)
{
	if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::BLUE) ||
		item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::BLUE)
	{
		if ((item1.itemtype == ITEM_TYPE::BOOK && item1.colortype == COLOR::BLUE) ||
			(item2.itemtype == ITEM_TYPE::BOOK && item2.colortype == COLOR::BLUE))
		{
			item1.colortype = COLOR::BLUE;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	return false;
}
bool ItemMenu::Key(ItemSave& item1, ItemSave& item2)
{
	// 黄色の石と本
	if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::YELLOW) ||
		item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::YELLOW)
	{
		if ((item1.itemtype == ITEM_TYPE::BOOK) ||
			(item2.itemtype == ITEM_TYPE::BOOK))
		{
			item1.colortype = COLOR::YELLOW;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	// 赤の本と緑の石 = 黄
	if ((item1.itemtype == ITEM_TYPE::BOOK && item1.colortype == COLOR::RED) ||
		item2.itemtype == ITEM_TYPE::BOOK && item2.colortype == COLOR::RED)
	{
		if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::GREEN) ||
			item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::GREEN)
		{
			item1.colortype = COLOR::YELLOW;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	// 緑の本と赤の石　= 黄
	if ((item1.itemtype == ITEM_TYPE::BOOK && item1.colortype == COLOR::GREEN) ||
		item2.itemtype == ITEM_TYPE::BOOK && item2.colortype == COLOR::GREEN)
	{
		if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::RED) ||
			item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::RED)
		{
			item1.colortype = COLOR::YELLOW;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	return false;
}
bool ItemMenu::Powder(ItemSave& item1, ItemSave& item2)
{
	if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::MAGENTA) ||
		item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::MAGENTA)
	{
		if ((item1.itemtype == ITEM_TYPE::BOOK && item1.colortype == COLOR::RED) ||
			(item2.itemtype == ITEM_TYPE::BOOK && item2.colortype == COLOR::RED))
		{
			item1.colortype = COLOR::MAGENTA;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	return false;
}
bool ItemMenu::Hose(ItemSave& item1, ItemSave& item2)
{
	if ((item1.itemtype == ITEM_TYPE::STONE && item1.colortype == COLOR::CYAN) ||
		item2.itemtype == ITEM_TYPE::STONE && item2.colortype == COLOR::CYAN)
	{
		if ((item1.itemtype == ITEM_TYPE::BOOK && item1.colortype == COLOR::BLUE) ||
			(item2.itemtype == ITEM_TYPE::BOOK && item2.colortype == COLOR::BLUE))
		{
			item1.colortype = COLOR::CYAN;
			item1.itemtype = ITEM_TYPE::TOOL;
			lpTradeMng.AddBag(item1);
			lpTradeMng.DeleteItem(item2);
			return true;
		}
	}
	return false;
}

void ItemMenu::ItemSelectD(int no)
{

	std::string image;

	// 風呂敷と枠
	lpImageMng.AddBackDraw({ lpImageMng.getImage("Menu")[0], _cpos.x, _cpos.y, 1.0, 0.0, LAYER::EX, _zorder.furoshiki, DX_BLENDMODE_NOBLEND, 0, true });
	lpImageMng.AddBackDraw({ lpImageMng.getImage("Space")[0], _cpos.x, _cpos.y - _offpush.y, 1.0, 0.0, LAYER::EX,_zorder.space, DX_BLENDMODE_NOBLEND, 0, true });

	// 矢印
	lpImageMng.AddBackDraw({ lpImageMng.getImage("s_box")[0], (_cpos.x - 200) + (_select * 75),
		_cpos.y - 50 + (_select2 * 100), 1.0, 0.0, LAYER::EX, _zorder.arrow, DX_BLENDMODE_NOBLEND, 0, true });

	//ItemDraw(510, { _cpos.x - 200 ,_cpos.y }, { 200,150 }, {40,100}, LAYER::EX);
	//lpTradeMng.BagDraw({ static_cast<double>(_cpos.x) - 200.0, static_cast<double>(_cpos.y)-50.0 }, LAYER::EX, { 0,100 }, {0.75,0.75});

	// 本
	Vector2 pos = { _cpos.x - 200,_cpos.y - 50 };
	int tmp = 520;
	for (int i = 0;i < 3;i++)
	{
		image = std::to_string(tmp);
		lpImageMng.AddBackDraw({ lpImageMng.getImage(image)[0], pos.x + (i * 75),pos.y, 0.75, 0.0, LAYER::EX,_zorder.item, DX_BLENDMODE_NOBLEND, 0, true });
		tmp++;
	}
	// 石
	pos.y = _cpos.y + 50;
	tmp = 510;
	for (int i = 0;i < 6;i++)
	{
		image = std::to_string(tmp);
		std::string _no = std::to_string(lpTradeMng.getrock().at(i));
		lpImageMng.AddBackDraw({ lpImageMng.getImage(image)[0], pos.x + (i * 75),pos.y, 0.75, 0.0, LAYER::EX,_zorder.item, DX_BLENDMODE_NOBLEND, 0, true });
		lpStrAdd.AddStringDraw(_no.c_str(), "SQUARE_BIG", pos.x + (i * 75), pos.y, 0x000000, DRAW_TO_CENTER);
		tmp++;
	}

	// アイテム
	pos.y = _cpos.y + 150;
	tmp = 530;
	for (int i = 0;i < 6;i++)
	{
		image = std::to_string(tmp);
		lpImageMng.AddBackDraw({ lpImageMng.getImage(image)[0], pos.x + (i * 75),pos.y, 0.75, 0.0, LAYER::EX,_zorder.item, DX_BLENDMODE_NOBLEND, 0, true });
		tmp++;
	}

	SelectCount(_select, XINPUT_THUMBL_X, _count.x);
	SelectCount(_select2, XINPUT_THUMBL_Y, _count.y);

	if (lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_A))
	{
		PlaySoundMem(_audio->GetSound("cancel"), DX_PLAYTYPE_BACK, true);
		_select = 0;
		_count = { 2,1 };
		MixFlag = false;
	}
}
void ItemMenu::SelectCount(int& select, int thumb, int count)
{
	// 矢印を右 or 上に移動させる
	if (lpButtonMng.Thumbf(0, thumb).first == 1 &&
		lpButtonMng.Thumbf(0, thumb).second == 0)
	{
		PlaySoundMem(_audio->GetSound("move"), DX_PLAYTYPE_BACK, true);
		select++;
		if (select > count)
		{
			select = 0;
		}
	}

	// 矢印を左 or 下に移動させる
	if (lpButtonMng.Thumbf(0, thumb).first == 2 &&
		lpButtonMng.Thumbf(0, thumb).second == 0)
	{
		PlaySoundMem(_audio->GetSound("move"), DX_PLAYTYPE_BACK, true);
		select--;
		if (select < 0)
		{
			select = count;
		}
	}
}
void ItemMenu::MixDraw(void)
{
	// アイテム選択
	for (int x = 0;x < 3;x++)
	{
		lpImageMng.AddBackDraw({ lpImageMng.getImage("Space")[0], _cpos.x - 200 + (_offpush.x * x), _cpos.y - _offpush.y, 1.0, 0.0, LAYER::EX,_zorder.space, DX_BLENDMODE_NOBLEND, 0, true });
	}
	// 矢印
	if (_select2 == 0)
	{
		lpImageMng.AddBackDraw({ lpImageMng.getImage("Arrow")[0], _cpos.x - 300 + (_offpush.x * _select), _cpos.y - _offpush.y, 1.0, 0.0, LAYER::EX,_zorder.arrow, DX_BLENDMODE_NOBLEND, 0, true });
	}
	else
	{
		lpImageMng.AddBackDraw({ lpImageMng.getImage("Arrow")[0], _cpos.x - 75, _cpos.y + 100, 1.0, 0.0, LAYER::EX, _zorder.arrow, DX_BLENDMODE_NOBLEND, 0, true });
	}

	// 選択後
	ItemSelectDraw();
}
void ItemMenu::ItemSelectDraw(void)
{
	for (auto& data : _sItem)
	{
		if (data.first.itemtype != ITEM_TYPE::NON)
		{
			data.first.pos = { _cpos.x - 200 + (_offpush.x * (data.second)),_cpos.y - _offpush.y };
			lpImageMng.AddBackDraw({ lpImageMng.getImage(data.first.image[0])[0],data.first.pos.x,data.first.pos.y, 1.0, 0.0,LAYER::EX, _zorder.item, DX_BLENDMODE_NOBLEND, 0, true });
		}
	}
}

void ItemMenu::Item(SELECT_ITEM item)
{
	int no = static_cast<int>(item);

	// 選択しているアイテムの表示
	if (_sItem.at(no).first.itemtype != ITEM_TYPE::NON) //　その中にアイテムがある場合のみ。
	{
		_sItem.at(no).first.pos = { _cpos.x,_cpos.y - _offpush.y };
		lpImageMng.AddBackDraw({ lpImageMng.getImage(_sItem.at(no).first.image[0])[0],_sItem.at(no).first.pos.x,
			_sItem.at(no).first.pos.y, 1.0, 0.0,LAYER::EX,_zorder.item, DX_BLENDMODE_NOBLEND, 0, true });
	}

	// アイテム選択
	ItemSelectD(no);

	// 決定した場合
	if (lpButtonMng.ButtonTrg(0, XINPUT_BUTTON_B))
	{
		PlaySoundMem(_audio->GetSound("select"), DX_PLAYTYPE_BACK, true);
		if (ColorPtr(_select) != COLOR::BLACK)
		{
			Ins(no, static_cast<ITEM_TYPE>(_select2),ColorPtr(_select));
		}
	}
}
void ItemMenu::Ins(int no, ITEM_TYPE itemtype, COLOR colortype)
{
	int _no = lpTradeMng.ReturnNo(itemtype, colortype);

	if (_no == -1)
	{
		return;
	}
	// アイテムバッグの中身確認
	if (lpTradeMng.NoReturn(_no) == true)
	{
		// 選択した合成欄にアイテムを入れる
		_sItem.at(no) = { lpTradeMng.ReturnBag(_no), no };
		MixFlag = false;
		_select = 0;
		_count = { 2,1 };
		_select2 = 0;
		// arrayのサイズ分回す。
		for (int i = 0;i < _asize;i++)
		{
			// アイテムが入っている場合のみチェック
			if (_sItem.at(i).first.itemtype != ITEM_TYPE::NON &&
				// 同じアイテムが違うところに入っていたら違うほうのアイテムをNONにする
				(_sItem.at(i).first.bagNo == _sItem.at(no).first.bagNo && _sItem.at(i).second != _sItem.at(no).second))
			{
				_sItem.at(i).first.itemtype = ITEM_TYPE::NON;
			}
		}
	}
}

COLOR ItemMenu::ColorPtr(int no)
{
	COLOR type;
	switch (no)
	{
	case 0:
		type = COLOR::RED;
		break;
	case 1:
		type = COLOR::GREEN;
		break;
	case 2:
		type = COLOR::BLUE;
		break;
	case 3:
		type = COLOR::YELLOW;
		break;
	case 4:
		type = COLOR::MAGENTA;
		break;
	case 5:
		type = COLOR::CYAN;
		break;
	default:
		type = COLOR::BLACK;
		break;
	}
	return type;
}

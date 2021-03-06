#include <math.h>
#include <DxLib.h>
#include "../Graphic/ImageMng.h"
#include "ItemTrader.h"
#include "../Scene/SceneMng.h"
#include "AttackUI.h"
#include "MapMng.h"

int ItemTrader::_count = 2;

ItemTrader* ItemTrader::sInstance = nullptr;

Vector2 _offpush = {200, 150 };

void ItemTrader::Draw(void)
{
	for (auto data : _ItemList)
	{
		if ((*data).getStage() == lpMapMng.GetnowStage())
		{
			(*data).Draw();
		}
	}
}

void ItemTrader::BagDraw(Vector2D pos, LAYER lay, Vector2 off, Vector2D rad)
{
	_pos.x = static_cast<int>(pos.x);
	_pos.y = static_cast<int>(pos.y);

	bool red = false, green = false, blue = false;
	int no = 0;
	int redN = 0, greenN = 0, blueN = 0;
	int redNo = 0, greenNo = 0, blueNo = 0;
	for (auto& data : _IBag)
	{
		if (data.first.itemtype == ITEM_TYPE::BOOK)
		{
			auto name = data.first.colortype;
			switch (name)
			{
			case COLOR::RED:
				redN++;
				if (!red)
				{
					data.first.pos = { _pos.x + (100 * no),_pos.y + off.x };
					lpImageMng.AddBackDraw({ lpImageMng.getImage(data.first.image[0])[0],data.first.pos.x,data.first.pos.y, rad.x, 0.0,LAYER::EX, 110, DX_BLENDMODE_NOBLEND, 0, true });
					redNo = no;
					no++;
				}
				red = true;
				break;
			case COLOR::GREEN:
				greenN++;
				if (!green)
				{
					data.first.pos = { _pos.x + (100 * no),_pos.y + off.x };
					lpImageMng.AddBackDraw({ lpImageMng.getImage(data.first.image[0])[0],data.first.pos.x,data.first.pos.y, rad.x, 0.0,LAYER::EX, 110, DX_BLENDMODE_NOBLEND, 0, true });
					greenNo = no;
					no++;
				}
				green = true;
				break;
			case COLOR::BLUE:
				blueN++;
				if (!blue)
				{
					data.first.pos = { _pos.x + (100 * no),_pos.y + off.x };
					lpImageMng.AddBackDraw({ lpImageMng.getImage(data.first.image[0])[0],data.first.pos.x,data.first.pos.y, rad.x, 0.0,LAYER::EX, 110, DX_BLENDMODE_NOBLEND, 0, true });
					blueNo = no;
					no++;
				}
				blue = true;
				break;
			default:
				break;
			}
		}
	}
	if (redN != 0) {
		lpStrAdd.AddStringDraw(std::to_string(redN).c_str(), "SQUARE_BIG", _pos.x + (100 * redNo), _pos.y + off.x, 0xffffff, DRAW_TO_RIGHT);
	}
	if (blueN != 0) {
		lpStrAdd.AddStringDraw(std::to_string(blueN).c_str(), "SQUARE_BIG", _pos.x + (100 * blueNo), _pos.y + off.x, 0xffffff, DRAW_TO_RIGHT);
	}
	if (greenN != 0) {
		lpStrAdd.AddStringDraw(std::to_string(greenN).c_str(), "SQUARE_BIG", _pos.x + (100 * greenNo), _pos.y + off.x, 0xffffff, DRAW_TO_RIGHT);
	}
}

void ItemTrader::ToolDraw(Vector2 pos, LAYER lay, Vector2 off, Vector2D rad)
{
	int count = 0;
	_drawtool[count];
	for (auto& data : _IBag)
	{
		while (_drawtool[count] == ~7)
		{
			count++;
		}
		if (data.first.tool == _drawtool[count])
		{
			lpImageMng.AddBackDraw({ lpImageMng.getImage(data.first.image[0])[0],cos(RAD(count * 45.0) - 45.0) * 75.0,
				sin(RAD(count*45.0) - 45.0) * 75.0,rad.x,0.0,LAYER::EX,120,DX_BLENDMODE_NOBLEND,0,true });
			count++;
		}
	}
}

bool ItemTrader::CheckTool(void)
{
	for (auto& data : _IBag)
	{
		if (data.first.tool == _drawtool[2])
		{
			movetool = data.first;
			return true;
		}
	}
	return false;
}

ItemSave& ItemTrader::getTool(void)
{
	return movetool;
}


COLOR ItemTrader::Trade(COLOR color1, COLOR color2)
{
	color = static_cast<int>(color1) + static_cast<int>(color2);

	return static_cast<COLOR>(color);
}

bool ItemTrader::ColorTradeCheck(COLOR color1, COLOR color2)
{
	if (color1 == color2) { return false;}	// 同じ色同士の足し算はしない

	// RGB以外の足し算もしない
	if (color1 != COLOR::RED && color1 != COLOR::BLUE && color1 != COLOR::GREEN)
	{
		return false;
	}
	else if (color2 != COLOR::RED && color2 != COLOR::BLUE && color2 != COLOR::GREEN)
	{
		return false;
	}
	return true;
}

const void ItemTrader::SetItemList(Vector2 pos, ITEM_TYPE itype, COLOR ctype,int stage)
{
	_ItemList.emplace_back(new Item(pos, itype, ctype, _ItemList.size(),stage));	// サイズ番号を入れる。
	return;
}

ItemSave &ItemTrader::ReturnBag(int no)
{
	return _IBag.at(no).first;
}

void ItemTrader::DeleteItem(ItemSave &item)
{
	for (auto bag = _IBag.begin();bag != _IBag.end();)
	{
		if ((*bag).first.bagNo == item.bagNo)
		{
			bag = _IBag.erase(std::move(bag));
		}
		else
		{
			bag++;
		}
	}
	BagTypeSort();
	BagTypeCount();
	BagNoSort();
}

void ItemTrader::AddBag(void)
{
	for (auto data = _ItemList.begin();data != _ItemList.end();)
	{
		if ((*data)->getPos().x - 24 <= (*lpSceneMng.GetPlObj(lpTimeMng.getTime()))->getPos().x + 24 &&
			(*data)->getPos().x + 24 >= (*lpSceneMng.GetPlObj(lpTimeMng.getTime()))->getPos().x - 24 &&
			(*data)->getPos().y - 24 <= (*lpSceneMng.GetPlObj(lpTimeMng.getTime()))->getPos().y + 24 &&
			(*data)->getPos().y + 24 >= (*lpSceneMng.GetPlObj(lpTimeMng.getTime()))->getPos().y - 24 &&
			
			lpMapMng.GetnowStage() == (*data)->GetStage())
		{
			_IBag.emplace_back((*data)->ReturnSave(),_IBag.size());
			data = _ItemList.erase(data);		// 次を返す
		}
		else
		{
			data++;								// 増やさないと回らない elseじゃなくても次を変えすから++と同じ値
		}
	}
	BagTypeSort();
	BagTypeCount();
	BagNoSort();
}

void ItemTrader::AddBag(ItemSave&save)
{
	for (auto& bag : _IBag)
	{
		if (bag.first.bagNo == save.bagNo)
		{
			save.image[0] = ChangeName(save.itemtype,save.colortype);
			bag.first = save;
		}
	}
}

int ItemTrader::ReturnNo(ITEM_TYPE itemtype, COLOR color)
{
	int no = -1;
	for (auto& bag : _IBag)
	{
		if (bag.first.colortype == color && bag.first.itemtype == itemtype)
		{
			no = bag.first.bagNo;
		}
	}
	return no;
}

bool ItemTrader::ReBook(COLOR color)
{
	for (auto& bag : _IBag)
	{
		if (bag.first.itemtype == ITEM_TYPE::BOOK && bag.first.colortype == color)
		{
			return true;
		}
	}
	return false;
}



bool ItemTrader::NoReturn(unsigned int no)
{
	// アイテムバッグに何も入っていない場合falseを返す
	if (_IBag.empty() == true)
	{
		return false;
	}
	// 選んだ番号がサイズよりも大きい場合falseを返す
	if (_IBag.size() <= no)
	{
		return false;
	}
	return true;
}

void ItemTrader::BagNoSort(void)
{
	int count = 0;

	for (auto& data : _IBag)
	{
		data.second = count;
		data.first.bagNo = count;
		count++;
	}
}

void ItemTrader::BagTypeSort(void)
{
	_stone = 0;
	_book  = 0;
	_tool = 0;
	for (auto &data:_IBag)
	{
		switch(data.first.itemtype)
		{
		case ITEM_TYPE::BOOK:
			data.first.book = _book;
			_book++;
			break;
		case ITEM_TYPE::STONE:
			data.first.stone = _stone;
 			_stone++;
			break;
		case ITEM_TYPE::TOOL:
			data.first.tool = _tool;
			_toolMax = _tool;
			_tool++;
		default:
			break;
		};
	}

}

void ItemTrader::BagTypeCount(void)
{
	for (int i = 0;i < 6;i++)
	{
		rock.at(i) = 0;
	}

	for (auto& item : _IBag)
	{
		if (item.first.itemtype == ITEM_TYPE::STONE)
		{
			switch(item.first.colortype)
			{
			case COLOR::BLACK:
				break;
			case COLOR::RED:
				rock.at(0)++;
				break;
			case COLOR::GREEN:
				rock.at(1)++;
				break;
			case COLOR::BLUE:
				rock.at(2)++;
				break;
			case COLOR::YELLOW:
				rock.at(3)++;
				break;
			case COLOR::MAGENTA:
				rock.at(4)++;
				break;		
			case COLOR::CYAN:
				rock.at(5)++;
				break;
			default:
				break;
			}
		}
	}
}


std::pair<int, int> ItemTrader::getcount(void)
{
	return {_book,_stone};
}

std::array<int, 6> ItemTrader::getrock(void)
{
	return rock;
}

void ItemTrader::ChangeCount(bool num)
{
	if (num == true)
	{
		if (_count < _toolMax+2)
		{
			_count++;
			for (int i = 0;i < 5;i++) {
				if (i == 4)
				{
					_drawtool[i] = _count;
					break;
				}
				_drawtool[i] = _drawtool[i + 1];
			}
		}
	}
	else
	{
		if (_count > 2)
		{
			_count--;
			for (int i = 4;i >= 0;i--)
			{
				if (_drawtool[0] == 0 && i == 0)
				{
					_drawtool[0] = ~7;
					break;
				}
				if (_drawtool[1] == 0 && i == 1)
				{
					_drawtool[1] = ~7;
					_drawtool[0] = ~7;
					break;
				}
				if (i == 0)
				{
					_drawtool[i] = _drawtool[i] - 1;
				}
				_drawtool[i] = _drawtool[i - 1];
			}
		}
	}
}

void ItemTrader::SetUseTool(bool flag,ItemSave save)
{
	for (auto& data : _IBag)
	{
		if (data.first.bagNo == save.bagNo)
		{
			data.first.use = true;
			return;
		}
	}
}

void ItemTrader::UseDeleteTool(void)
{
	auto itr = std::remove_if(_IBag.begin(), _IBag.end(), [&](auto && data) {
		return data.first.use == true;
		});
	_IBag.erase(itr, _IBag.end());
	BagTypeSort();
	BagTypeCount();
	BagNoSort();
}

void ItemTrader::ListUpdate(void)
{
	for (auto& data : _ItemList)
	{
		data->Update();
	}
}

void ItemTrader::DeleteAll(void)
{
	_ItemList.clear();
	_IBag.clear();
	_ItemBag.clear();
	rock.fill(0);
}

ItemTrader::ItemTrader()
{
	for (int i = 0;i < 6;i++)
	{
		rock.at(i) = 0;
	}
	_book = 0;
	_stone = 0;
	_tool = 0;
	color = 0x000000;
	for (int i = 0;i < 2;i++)
	{
		_drawtool[i] = ~7;
	}
	for (int i = 0;i < 3;i++)
	{
		_drawtool[i+2] = i;
	}
}

ItemTrader::~ItemTrader()
{
}

const char* ItemTrader::ChangeName(ITEM_TYPE _itemtype,COLOR _colortype)
{
	AnmVec data;
	data.clear();

	// 通常サイズ
	switch (_itemtype)
	{
	case ITEM_TYPE::BOOK:

		switch (_colortype)
		{
		case COLOR::BLACK:
			break;
		case COLOR::BLUE:
			save.image[0] = "522";
			break;
		case COLOR::GREEN:
			save.image[0] = "521";
			break;
		case COLOR::RED:
			save.image[0] = "520";
			break;
		case COLOR::CYAN:
			save.image[0] = "CyanBook";
			break;
		case COLOR::MAGENTA:
			save.image[0] = "MagentaBook";
			break;
		case COLOR::YELLOW:
			save.image[0] = "YellowBook";
			break;
		case COLOR::WHITE:
			break;
		default:
			break;
		}
		break;
	case ITEM_TYPE::STONE:

		switch (_colortype)
		{
		case COLOR::BLACK:
			break;
		case COLOR::BLUE:
			save.image[0] = "512";
			break;
		case COLOR::CYAN:
			save.image[0] = "515";
			break;
		case COLOR::GREEN:
			save.image[0] = "511";
			break;
		case COLOR::MAGENTA:
			save.image[0] = "514";
			break;
		case COLOR::RED:
			save.image[0] = "510";
			break;
		case COLOR::YELLOW:
			save.image[0] = "513";
			break;
		case COLOR::WHITE:
			break;
		}
		break;
	case ITEM_TYPE::TOOL:
		switch (_colortype)
		{
		case COLOR::BLACK:
			break;
		case COLOR::RED:
			save.image[0] = "530";
			break;
		case COLOR::GREEN:
			save.image[0] = "531";
			break;
		case COLOR::BLUE:
			save.image[0] = "532";
			break;
		case COLOR::YELLOW:
			save.image[0] = "533";
			break;
		case COLOR::MAGENTA:
			save.image[0] = "534";
			break;
		case COLOR::CYAN:
			save.image[0] = "535";
			break;
		case COLOR::WHITE:
			break;
		}
	}
	return save.image[0];
}

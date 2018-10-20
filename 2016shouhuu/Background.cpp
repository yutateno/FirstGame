#include "Dxlib.h"
#include "Scenemgr.h"
#include "Media.h"
#include "Enemy.h"

#define SCROLL_SPEED 2

//更新
int BGx;			//スクロールの量
int BGX = -100;		//ラスボスの背景へのスクロール
bool StopFlag;		//スクロールをやめさせる
int ChengeFlag;		//前半後半背景をスムーズに変える

//雑魚戦の描画
void Background_Draw()
{
	if (EnemyBreak() < 2)
	{
		//一枚目描画
		DrawGraph(BGx, 0, M_BackGround.BackGroundM, TRUE);

		//二枚目描画
		DrawGraph(BGx + 640, 0, M_BackGround.BackGroundM, TRUE);

		//一番右までスクロールしたら初期値に戻す
		if (BGx == -640)
		{
			BGx = 0;
		}
	}
	//雑魚戦の途中から背景を少し変えるギミック
	else
	{
		if (ChengeFlag == 0)
		{
			//一枚目描画
			DrawGraph(BGx, 0, M_BackGround.BackGroundM, TRUE);

			//二枚目描画
			DrawGraph(BGx + 640, 0, M_BackGround.BackGroundM, TRUE);

			//一番右までスクロールしたら初期値に戻す
			if (BGx == -640)
			{
				BGx = 0;
				ChengeFlag = 1;
			}
		}
		else if (ChengeFlag == 1)
		{
			//一枚目描画
			DrawGraph(BGx, 0, M_BackGround.BackGroundM, TRUE);

			//二枚目描画
			DrawGraph(BGx + 640, 0, M_BackGround.SecondBackGroundM, TRUE);

			//一番右までスクロールしたら初期値に戻す
			if (BGx == -640)
			{
				BGx = 0;
				ChengeFlag = 2;
			}
		}
		else
		{
			//一枚目描画
			DrawGraph(BGx, 0, M_BackGround.SecondBackGroundM, TRUE);

			//二枚目描画
			DrawGraph(BGx + 640, 0, M_BackGround.SecondBackGroundM, TRUE);

			//一番右までスクロールしたら初期値に戻す
			if (BGx == -640)
			{
				BGx = 0;
			}
		}
	}
}

//ラスボスの描画
void LastBackground_Draw()
{
	//一枚目描画
	DrawGraph(BGX, 0, M_BackGround.SecondBackGroundM, TRUE);

	//二枚目描画
	DrawGraph(BGX + 640, 0, M_BackGround.LastBackGroundM, TRUE);

	//一番右までスクロールしたら初期値に戻す
	if (BGX == -640)
	{
		StopFlag = 1;
	}
}

//雑魚戦の更新
void Background_Update()
{
	BGx -= SCROLL_SPEED;
}

//ラスボスの更新
void LastBackground_Update()
{
	if (StopFlag != 1)
	{
		BGX -= SCROLL_SPEED;
	}
}

//リセット
void Background_R()
{
	BGx = 0;
	BGX = -100;
	StopFlag = 0;
	ChengeFlag = 0;
}
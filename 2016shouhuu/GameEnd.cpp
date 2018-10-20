#include "DxLib.h"
#include "Background.h"
#include "Player.h"
#include "scenemgr.h"
#include "GameLast.h"
#include "Enemy.h"
#include "GameFirst.h"
#include "GameSecond.h"
#include "SgameFirst.h"
#include "SgameSecond.h"
#include "Sound.h"
#include "Media.h"

//中身
double PEndMoveX, PEndMoveY;	//勇者の動き
double PriMoveX, PriMoveY;		//姫の動き
int enemyfeed;					//ラスボスを消すのに利用
double enemyfeedout = 1;		//ラうボスを消すのに利用part2
int SEcount;					//流れ
int dd = 255;					//フェードアウト用

//コントローラー
int GEpad;


//光度を下げていく
void DOWNn()
{
	if (dd >= 0)
	{
		dd -= 3;
	}
}
void DARKk()
{
	if (dd >= 0)
	{
		SetDrawBright(dd, dd, dd);
	}
}

//リセット
void GameEnd_R()
{
	PEndMoveX = 0;
	PEndMoveY = 0;
	PriMoveX = 0;
	PriMoveY = 0;
	enemyfeed = 0;
	enemyfeedout = 1;
	SEcount = 0;
	dd = 255;
}

//更新
void GameEnd_Update()
{
	GEpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GEpad & PAD_INPUT_7) && (GEpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	if (GEpad & PAD_INPUT_7)   //BACKが押されていたら
	{
		StopSoundMem(S_BGM.MenuS);
		Background_R();
		Enemy_R();
		GameEnd_R();
		GameFirst_R();
		GameLast_R();
		GameSecond_R();
		Player_R();
		SGameFirst_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);
	}
	SEcount++;
	if (SEcount >= 240 && SEcount <= 365)
	{
		// 音量の設定
		ChangeVolumeSoundMem(150, S_BGM.MenuS);

		// 音の再生
		PlaySoundMem(S_BGM.MenuS, DX_PLAYTYPE_LOOP,FALSE);
		//上
		if (GetPlayerYpoint() == -2)
		{
			PEndMoveX += 3.92;
			PEndMoveY += 0.48;
		}

		//真ん中
		else if (GetPlayerYpoint() == 0)
		{
			PEndMoveX += 3.92;
			PEndMoveY -= 0.16;
		}

		//下
		else
		{
			PEndMoveX += 3.92;
			PEndMoveY -= 0.8;
		}

	}
	if (SEcount >= 370 && SEcount <= 395)
	{
		PEndMoveY += 1.3;
	}
	if (SEcount >= 397 && SEcount <= 407)
	{
		PriMoveY += 0.5;
		//PrinMoveY=3
	}
	if (SEcount >= 409)
	{
		PEndMoveX -= 5;
		PriMoveX -= 5;
	}
	if (SEcount >= 430 && SEcount < 515)
	{
		DOWNn();
	}
	if (SEcount >= 515)
	{
		dd = 255;
	}
}

//描画
void GameEnd_Draw()
{
	// 画面全体を描画可能にする
	SetDrawArea(0, 0, 640, 480);
	LastBackground_Draw();	//背景の描画
	//剣の描画
	if (SEcount < 370)
	{
		DrawGraph((int)PEndMoveX + 100, (int)PEndMoveY + (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (SEcount >= 370)
	{
		DrawTurnGraph((int)PEndMoveX + 100, (int)PEndMoveY + (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//姫の描画
	DrawGraph((int)PriMoveX + 590, (int)PriMoveY + 300, M_Character.PrincessM, TRUE);
	//敵の描画
	if (SEcount < 20)
	{
		DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);
	}
	//敵を消していく
	if (SEcount >= 20 && SEcount <= 220)
	{
		// １ライン単位で描画をする処理
		for (enemyfeed = 0; enemyfeed < 150; enemyfeed++)
		{
			if (enemyfeed % (int)enemyfeedout == 0)	//表示する数値(増えるごとに消えていく)
			{
				// 特定ラインだけを描画可能にする
				SetDrawArea(365, 195 + enemyfeed, 515, 195 + enemyfeed + 1);

				// 画像を描画
				DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);	//x座標を0にすればぶれない
				enemyfeedout += 0.005;
			}
		}
	}
	if (SEcount >= 430)
	{
		DARKk();
	}
	if (SEcount >= 515)
	{
		DrawGraph(0, 0, M_BackGround.BlackM, FALSE);
	}
	if (SEcount >= 525)
	{
		DrawFormatString(300, 220, GetColor(255, 255, 0), "Thank You For Playing!");
		DrawFormatString(300, 270, GetColor(255, 255, 0), "BACKでタイトルへ戻ります。");
		DrawFormatString(300, 320, GetColor(255, 255, 0), "メニューで LB を押しと何か");
	}
}
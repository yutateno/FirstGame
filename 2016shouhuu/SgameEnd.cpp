#include "DxLib.h"
#include "Background.h"
#include "PlayerShield.h"
#include "PlayerSword.h"
#include "scenemgr.h"
#include "SgameLast.h"
#include "Enemy.h"
#include "SgameFirst.h"
#include "SgameSecond.h"
#include "Sound.h"
#include "Media.h"

//内容
double PSwEndMoveX, PSwEndMoveY;	//剣の勇者の動き
double PShEndMoveX, PShEndMoveY;	//盾の勇者の動き
double PrinMoveX, PrinMoveY;		//姫の動き
int EnemyFeed;						//敵を消していくのに関する
double EnemyFeedOut = 1;			//敵を消していくのに関する
int SgEcount;						//全体の流れ
int D = 255;						//フェードアウト関連

//コントローラー
int SGEpad;		//コントローラー


//光度を下げていく
void DowN()
{
	if (D >= 0)
	{
		D -= 3;
	}
}
void DarK()
{
	if (D >= 0)
	{
		SetDrawBright(D, D, D);
	}
}

//リセット
void SGameEnd_R()
{
	PSwEndMoveX = 0;
	PSwEndMoveY = 0;
	PShEndMoveX = 0;
	PShEndMoveY = 0;
	PrinMoveX = 0;
	PrinMoveY = 0;
	EnemyFeed = 0;
	EnemyFeedOut = 1;
	SgEcount = 0;
	D = 255;
}

//更新
void SgameEnd_Update()
{
	SGEpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGEpad & PAD_INPUT_7) && (SGEpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	if (SGEpad & PAD_INPUT_7)   //BACK キーが押されていたら
	{
		StopSoundMem(S_BGM.MenuS);
		Background_R();
		Enemy_R();
		PlayerShield_R();
		PlayerSword_R();
		SGameEnd_R();
		SGameFirst_R();
		SGameLast_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);
	}
	SgEcount++;
	if (SgEcount >= 240 && SgEcount <= 365)
	{
		// 音量の設定
		ChangeVolumeSoundMem(150, S_BGM.MenuS);

		// 音の再生
		PlaySoundMem(S_BGM.MenuS, DX_PLAYTYPE_LOOP, FALSE);
		//剣が上で盾が上
		if (GetswordYpoint() == -2 && Getshieldpoint() == -2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY += 0.48;
			PShEndMoveX += 3.12;
			PShEndMoveY += 1.28;
			/*最終的に動く距離　動いた結果//
			PSwEndMoveX = 490	X=590
			PSwEndMoveY = 60	Y=230
			PShEndMoveX = 390	X=590
			PShEndMoveY = 160	Y=330
			*/
		}
		//剣が真ん中で盾が上
		else if (GetswordYpoint() == 0 && Getshieldpoint() == -2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.16;
			PShEndMoveX += 3.12;
			PShEndMoveY += 1.28;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = -20
			PShEndMoveX = 390
			PShEndMoveY = 160
			*/
		}
		//剣が下で盾が上
		else if (GetswordYpoint() == 2 && Getshieldpoint() == -2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.8;
			PShEndMoveX += 3.12;
			PShEndMoveY += 1.28;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = -100
			PShEndMoveX = 390
			PShEndMoveY = 160
			*/
		}
		//剣が上で盾が真ん中
		else if (GetswordYpoint() == -2 && Getshieldpoint() == 0)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY += 0.48;
			PShEndMoveX += 3.12;
			PShEndMoveY += 0.64;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = 60
			PShEndMoveX = 390
			PShEndMoveY = 80
			*/
		}
		//剣が真ん中で盾が真ん中
		else if (GetswordYpoint() == 0 && Getshieldpoint() == 0)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.16;
			PShEndMoveX += 3.12;
			PShEndMoveY += 0.64;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = -20
			PShEndMoveX = 390
			PShEndMoveY = 80
			*/
		}
		//剣が下で盾が真ん中
		else if (GetswordYpoint() == 2 && Getshieldpoint() == 0)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.8;
			PShEndMoveX += 3.12;
			PShEndMoveY += 0.64;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = -100
			PShEndMoveX = 390
			PShEndMoveY = 80
			*/
		}
		//剣が上で盾が下
		else if (GetswordYpoint() == -2 && Getshieldpoint() == 2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY += 0.48;
			PShEndMoveX += 3.12;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = 60
			PShEndMoveX = 390
			PShEndMoveY = 0
			*/
		}
		//剣が真ん中で盾が下
		else if (GetswordYpoint() == 0 && Getshieldpoint() == 2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.16;
			PShEndMoveX += 3.12;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = -20
			PShEndMoveX = 390
			PShEndMoveY = 0
			*/
		}
		//剣が下で盾が下
		else
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.8;
			PShEndMoveX += 3.12;
			/*最終的に//
			PSwEndMoveX = 490
			PSwEndMoveY = -100
			PShEndMoveX = 390
			PShEndMoveY = 0
			*/
		}
	}
	if (SgEcount >= 370 && SgEcount <= 395)
	{
		PSwEndMoveX -= 2;
		PSwEndMoveY += 2;
		PShEndMoveX += 1.68;
		PShEndMoveY -= 2;
		/*最終的に動く距離　動く前 動いた後
		PSwEndMoveX = -50	X=590  X=540
		PSwEndMoveY = 50	Y=230  Y=280
		PShEndMoveX = 42	X=590  X=632
		PShEndMoveY = -50	Y=330  Y=280
		*/
	}
	if (SgEcount >= 397 && SgEcount <= 407)
	{
		PrinMoveY += 0.3;
		//PrinMoveY=3
	}
	if (SgEcount >= 409)
	{
		PSwEndMoveX -= 5;
		PShEndMoveX -= 5;
		PrinMoveX -= 5;
		/*最終的に動く距離　動く前  動いた後
		PSwEndMoveX =682	X=540	X=-142	
		PShEndMoveX =682 	X=632	X=-50
		PrinMoveX	=682	X=590	X=-92
		*/
	}
	if (SgEcount >= 430 && SgEcount < 515)
	{
		DowN();
	}
	if (SgEcount >= 515)
	{
		D = 255;
	}
}

//描画
void SgameEnd_Draw()
{
	// 画面全体を描画可能にする
	SetDrawArea(0, 0, 640, 480);
	LastBackground_Draw();	//背景の描画
	//剣の描画
	if (SgEcount < 370)
	{
		DrawGraph((int)PSwEndMoveX + 100, (int)PSwEndMoveY + (GetswordYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//姫の描画
	DrawGraph((int)PrinMoveX + 590, (int)PrinMoveY + 300, M_Character.PrincessM, TRUE);
	//剣の描画
	if (SgEcount >= 370)
	{
		DrawTurnGraph((int)PSwEndMoveX + 100, (int)PSwEndMoveY + (GetswordYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//盾の描画
	if (SgEcount < 396)
	{
		DrawGraph((int)PShEndMoveX + 200, (int)PShEndMoveY + 250 + (Getshieldpoint() * 40), M_Character.PlayerShieldM, TRUE);
	}
	if (SgEcount >= 396)
	{
		DrawTurnGraph((int)PShEndMoveX + 200, (int)PShEndMoveY + 250 + (Getshieldpoint() * 40), M_Character.PlayerShieldM, TRUE);
	}
	//敵の描画
	if (SgEcount < 20)
	{
		DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);
	}
	//敵を消していく
	if (SgEcount >= 20 && SgEcount <= 220)
	{
		// １ライン単位で描画をする処理
		for (EnemyFeed = 0; EnemyFeed < 150; EnemyFeed++)
		{
			if (EnemyFeed % (int)EnemyFeedOut == 0)	//表示する数値(増えるごとに消えていく)
			{
				// 特定ラインだけを描画可能にする
				SetDrawArea(365, 195 + EnemyFeed, 515, 195 + EnemyFeed + 1);

				// 画像を描画
				DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);	//x座標を0にすればぶれない
				EnemyFeedOut += 0.005;
			}
		}
	}
	if (SgEcount >= 430)
	{
		DarK();
	}
	if (SgEcount >= 515)
	{
		DrawGraph(0, 0, M_BackGround.BlackM, FALSE);
	}
	if (SgEcount >= 525)
	{
		DrawFormatString(300, 220, GetColor(255, 255, 0), "Thank You For Playing!");
		DrawFormatString(300, 270, GetColor(255, 255, 0), "BACKでタイトルへ戻ります。");
		DrawFormatString(300, 320, GetColor(255, 255, 0), "メニューで LB を押すと何か");
	}
}
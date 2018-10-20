#include "SceneMgr.h"
#include "Background.h"
#include "Player.h"
#include "SgameSecond.h"
#include "DxLib.h"
#include "Media.h"
#include "Sound.h"

//内容
int MOvEX;			//最初の移動
int SSCounT;		//このシーンを担うカウント
double EnemyMovEX;	//敵の移動
bool GSFlag;		//このシーンに入ったフラッグ（制限時間がここでもアクティブなのでその調整用

//コントローラー
int GSpad;

//更新
void GameSecond_Update()
{
	GSpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GSpad & PAD_INPUT_7) && (GSpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	SSCounT++;
	GSFlag = 1;
	if (SSCounT >= 60)
	{
		MOvEX += 4;
	}
	if (SSCounT >= 63)
	{
		PlaySoundMem(S_BGM.AsiotoS, DX_PLAYTYPE_LOOP, FALSE);
	}
	if (SSCounT >= 100 && SSCounT <= 200)
	{
		Down();		//二人用と同じなので併用（中身はフェードアウトの処理
	}
	if (SSCounT >= 100)
	{
		StopSoundMem(S_BGM.AsiotoS);
	}
	if (SSCounT >= 220)
	{
		UP();		//二人用の同じなので併用（中身はフェードインの処理
		LastBackground_Update();
	}
}

//描画
void GameSecond_Draw()
{
	if (SSCounT <= 200)
	{
		Background_Draw();
		DrawGraph(MOvEX + 100, (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		DARK();		//二人用と同じなので併用（中身はフェードアウトの処理
	}
	if (SSCounT >= 220)
	{
		BRIGHT();	//二人用と同じなので併用（中身はフェードインの処理
		LastBackground_Draw();
	}
	if (SSCounT >= 220 && SSCounT <= 420)
	{
		DrawGraph(200, 220, M_Character.PlayerSwordM[0], TRUE);
	}
	if (SSCounT >= 360 && SSCounT <= 480)
	{
		EnemyMovEX += 2.25;
		DrawGraph(640 - (int)EnemyMovEX, 195, M_Enemy.LastEnemyM, TRUE);
	}
	if (SSCounT >= 420)
	{
		DrawGraph(100, (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (SSCounT >= 480)
	{
		DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);
	}
	if (SSCounT == 490)
	{
		SceneMgr_ChangeScene(eScene_GameLast);
	}
}

//リセット
void GameSecond_R()
{
	MOvEX = 0;
	SSCounT = 0;
	EnemyMovEX = 0;
	GSFlag = 0;
}

//制限時間の減り調整
bool GSecondFlag()
{
	return GSFlag;
}
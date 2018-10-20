#include "SceneMgr.h"
#include "Background.h"
#include "DxLib.h"
#include "Sound.h"
#include "Media.h"

//内容
int bright = 0;		//フェードイン
int Mcount;			//最初の無言移動から終わりまで
int SGFvol;			//ゆっくり音の大きさ上げていく
int EY = 225;		//敵のY座標
double EXM;			//敵の出現カウント

//コントローラー
int SGFpad;		//コントローラー


//光度を上げていく
void Update()
{
	if (bright <= 255)
	{
		bright += 3;
	}
}
//光度を上げていく
void Bright()
{
	if (bright <= 255)
	{
		SetDrawBright(bright, bright, bright);
	}
}

//更新
void SgameFirst_Update()
{
	SGFpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGFpad & PAD_INPUT_7) && (SGFpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	Update();
	Background_Update();
	Mcount++;
}

//描画
void SgameFirst_Draw()
{
	//音量の設定(だんだん音を出していく
	SGFvol++;
	ChangeVolumeSoundMem(SGFvol * 2 / 3, S_BGM.ZakoS);

	//音の再生
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);
	
	Bright();
	Background_Draw();
	
	DrawGraph(200, 250, M_Character.PlayerShieldM, TRUE);
	if (Mcount < 100)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (Mcount >= 100 && Mcount < 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[1], TRUE);
	}
	if (Mcount >= 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
		DrawGraph(670 - (int)EXM, EY, M_Enemy.EnemyoM[0], TRUE);
		EXM += 1.5;
		if ((670 - (int)EXM) <= 410)
		{
			StopSoundMem(S_BGM.ZakoS);
			SceneMgr_ChangeScene(eScene_SGame);	//戦闘画面に映る
		}
	}
}

//リセット
void SGameFirst_R()
{
	Mcount = 0;
	EY = 225;
	EXM = 0;
	SGFvol = 0;
	bright = 0;
}
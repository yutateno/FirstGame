#include "SceneMgr.h"
#include "DxLib.h"
#include "Sound.h"
#include "Media.h"

//コントローラー
int Mpad;

//内容
int MenuUp;		//上移動
int MenuDown;	//下移動
bool ura;		//裏へ行けるフラッグ
bool ForceEnd;	//強制終了フラッグ
int SPoint;		// 選択カーソルの位置

//各処理の関数
void One(void);		//一人プレイをする関数
void Two(void);		//二人プレイをする関数
void End(void);		//終了する関数
void Ura(void);		//裏のメニュー画面にいくための関数

//処理ポインタ配列
void(*Method[])() =
{
	One,Two,End,Ura
};

//描画
void Menu_Draw()
{
	ForceEnd = 0;
	//選択ポインタの位置初期化
	SPoint = 0;

	SetFontSize(23);

	// ループ
	while (true)
	{
		//音量の設定
		ChangeVolumeSoundMem(255, S_BGM.MenuS);				//エンディングにて音量を下げたのを上げるための処理
		PlaySoundMem(S_BGM.MenuS, DX_PLAYTYPE_LOOP, FALSE);
		DrawGraph(0, 0, M_BackGround.TitleM, FALSE);		//ボタンのカーソルを消すためここで行う
		//キー入力に応じて処理
		Mpad = GetJoypadInputState(DX_INPUT_PAD1);
		if (ura == 0)
		{
			DrawBox(30, 320 + (SPoint * 32), 46, 336 + (SPoint * 32), GetColor(255, 255, 0), TRUE);
			DrawFormatString(62, 320, GetColor(0, 0, 0), "一人で遊ぶ");
			DrawFormatString(62, 352, GetColor(0, 0, 0), "二人プレイで遊ぶ");
			DrawFormatString(62, 384, GetColor(0, 0, 0), "ゲームを終了する");

			//Escキーが押されていたら
			if (((Mpad & PAD_INPUT_7) && (Mpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
			{
				ForceEnd = 1;
				break;
			}
			//上が押されたら
			if (((Mpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0))
			{
				MenuUp++;
			}
			else
			{
				MenuUp = 0;
			}
			//下が押されたら
			if (((Mpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0))
			{
				MenuDown++;
			}
			else
			{
				MenuDown = 0;
			}
			//上移動に関する処理
			if (MenuUp == 1 && SPoint != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint--;
			}
			//下移動に関する処理
			if (MenuDown == 1 && SPoint != 2)	//項目が３つなので
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint++;
			}
			//実際LBボタンが押されたときの処理(裏メニューに行きます
			if ((Mpad & PAD_INPUT_Y) || CheckHitKey(KEY_INPUT_G) != 0)
			{
				ura = 1;
			}
			//決定キー時処理
			if ((Mpad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_Z) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				StopSoundMem(S_BGM.MenuS);
				//項目に応じた処理を行う
				Method[SPoint]();
				break;
			}
		}
		else
		{
			DrawBox(30, 320 + (SPoint * 32), 46, 336 + (SPoint * 32), GetColor(255, 255, 0), TRUE);
			DrawFormatString(62, 320, GetColor(0, 0, 0), "一人で遊ぶ");
			DrawFormatString(62, 352, GetColor(0, 0, 0), "二人プレイで遊ぶ");
			DrawFormatString(62, 384, GetColor(0, 0, 0), "ゲームを終了する");
			DrawFormatString(62, 416, GetColor(255, 0, 0), "裏");				//裏へのコマンドを追加

			//Escキーが押されていたら
			if (((Mpad & PAD_INPUT_7) && (Mpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
			{
				ForceEnd = 1;
				break;
			}
			//上ボタンが押されたら
			if (((Mpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0))
			{
				MenuUp++;
			}
			else
			{
				MenuUp = 0;
			}
			//下ボタンが押されたら
			if (((Mpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0))
			{
				MenuDown++;
			}
			else
			{
				MenuDown = 0;
			}
			//上移動に関する処理
			if (MenuUp == 1 && SPoint != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint--;
			}
			//下移動に関する処理
			if (MenuDown == 1 && SPoint != 3)	//項目が４つなので
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint++;
			}
			//戻る
			if (CheckHitKey(KEY_INPUT_H) != 0)
			{
				ura = 0;
			}
			//決定キー時処理
			if ((Mpad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_Z) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				StopSoundMem(S_BGM.MenuS);
				//項目に応じた処理を行う
				Method[SPoint]();
				break;
			}
		}
	}
	//強制終了
	if (ForceEnd == 1)
	{
		DxLib_End(); //DXライブラリを終了する
	}
}

void One()
{
	SceneMgr_ChangeScene(eScene_Opening);   //シーンを一人プレイ
}

void Two()
{
	SceneMgr_ChangeScene(eScene_SOpening);  //シーンを二人プレイ
}

void End()
{
	DxLib_End();							//DXライブラリを終了する
}

void Ura()
{
	SceneMgr_ChangeScene(eScene_MenuSub);	//シーンを裏メニュー
}
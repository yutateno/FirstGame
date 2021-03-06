#include "DxLib.h"
#include "scenemgr.h"
#include "Media.h"
#include "Sound.h"

#define oneX 5			//選択の一番目のX座標
#define twoX 165		//選択の二番目のX座標
#define threeX 325		//選択の三番目のX座標
#define forX 485		//選択の四番目のX座標
#define botanY 412		//選択のY座標

//コントローラー
int Urapad;

//内容(勇者関連
int UraPswX, UraPswY;		//剣の勇者の動き
int UraPshY;				//盾の勇者の動き
int UraRand;				//勇者の攻撃する乱数
bool UraPSFlag;				//勇者の攻撃フラッグ
int UraRandF;				//乱数を入れる変数(剣
bool UraPAttackFlag;		//攻撃するフラッグ
int UraRandFF;				//乱数を入れる変数(盾

//内容(敵関連
int UraEnemyX, UraEnemyY;	//敵の動き
bool UraEnemyAFlag;			//敵(自分)が攻撃するフラッグ

//内容(選択関連
int UraSentakunum;			//選択する順番
int UraSentakuY;			//選択ボタンの座標
int UraSentakunumX;			//縦で選択したところ
int UraSentakuUp;			//選択での上移動を一回にするため
int UraSentakuDown;			//選択での下移動を一回にするため
int UraSentakuK;			//決定を一回だけにするため
int UraSentakuKFlag;		//選択が決まったフラッグ
int UraSentakuB;			//戻る
int UraGage = 240;			//ゲージ
int UraCount;				//カウント

//リセット
void UraGame_R()
{
	UraPswX = 0;
	UraPswY = 0;
	UraPshY = 0;
	UraEnemyX = 0;
	UraEnemyY = 0;
	UraEnemyAFlag = 0;
	UraRand = 7;
	UraPSFlag = 0;
	UraRandF = 0;
	UraPAttackFlag = 0;
	UraRandFF = 0;
	UraSentakunum = 0;
	UraSentakuY = 0;
	UraSentakuUp = 0;
	UraSentakuDown = 0;
	UraSentakuK = 0;
	UraSentakuKFlag = 0;
	UraSentakuB = 0;
	UraGage = 240;
	UraCount = 0;
}

//メインの更新
void UraGame_Update()
{
	UraRand = GetRand(8) + 1;	//1〜9にした
	Urapad = GetJoypadInputState(DX_INPUT_PAD1);
	PlaySoundMem(S_BGM.UraBGMM, DX_PLAYTYPE_LOOP, FALSE);
	if (((Urapad & PAD_INPUT_7) && (Urapad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	if (Urapad & PAD_INPUT_7)   //BACKキーが押されていたら
	{
		StopSoundMem(S_BGM.UraBGMM);
		UraGame_R();
		SceneMgr_ChangeScene(eScene_MenuSub); //裏メニューに戻る
	}
}

//剣の勇者関連
void UraGame_PlayerSword()
{
	//攻撃頻度のカウント
	if (UraEnemyAFlag == 0)
	{
		UraGage --;
	}
	//277で割り切れた時に攻撃
	if (UraGage % 101 == 0 && UraGage != 0)
	{
		UraPSFlag = 1;
	}

	//動きに関して
	//攻撃するときになったらどこに攻撃するか乱数を代入
	if (UraPSFlag == 1)
	{
		UraRandF = UraRand;
		UraRandFF = UraRandF;
		UraPSFlag = 0;
	}
	//1.上攻
	if (UraRandF == 1)
	{
		UraPAttackFlag = 1;
		UraPswY = -2;
	}
	//2.真攻
	else if (UraRandF == 2)
	{
		UraPAttackFlag = 1;
		UraPswY = 0;
	}
	//3.下攻
	else if (UraRandF == 3)
	{
		UraPAttackFlag = 1;
		UraPswY = 2;
	}
	//4.上防
	else if (UraRandF == 4)
	{
		UraPswX = 0;
		UraPswY = -2;
	}
	//5.真防
	else if (UraRandF == 5)
	{
		UraPswX = 0;
		UraPswY = 0;
	}
	//6.下防
	else if (UraRandF == 6)
	{
		UraPswX = 0;
		UraPswY = 2;
	}
	//7.上魔
	else if (UraRandF == 7)
	{
		UraPswX = 0;
		UraPswY = -2;
	}
	//8.真魔
	else if (UraRandF == 8)
	{
		UraPswX = 0;
		UraPswY = 0;
	}
	//9.下魔
	else if (UraRandF == 9)
	{
		UraPswX = 0;
		UraPswY = 2;
	}
	//攻撃するとき
	if (UraPAttackFlag == 1)
	{
		UraPswX += 10;
		if (UraPswX >= 250)
		{
			UraPswX = 0;
			UraRandF = 0;
			UraPAttackFlag = 0;
		}
	}

	//絵に関して
	//通常時(攻撃していない
	if(UraPswX <= 0)
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//攻撃位置まで移動中
	else if (UraPswX > 0 && UraPswX <= 200)
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
	}
	//攻撃する
	else
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
	}
}

//盾の勇者関連
void UraGame_PlayerShield()
{
	//動きに関して
	//1.上攻
	if (UraRandFF == 1)
	{
		UraPshY = 0;
	}
	//2.真攻
	else if (UraRandFF == 2)
	{
		UraPshY = 2;
	}
	//3.下攻
	else if (UraRandFF == 3)
	{
		UraPshY = 0;
	}
	//4.上防
	else if (UraRandFF == 4)
	{
		UraPshY = -2;
	}
	//5.真防
	else if (UraRandFF == 5)
	{
		UraPshY = 0;
	}
	//6.下防
	else if (UraRandFF == 6)
	{
		UraPshY = 2;
	}
	//7.上魔
	else if (UraRandFF == 7)
	{
		UraPshY = -2;
	}
	//8.真魔
	else if (UraRandFF == 8)
	{
		UraPshY = 0;
	}
	//9.下魔
	else if (UraRandFF == 9)
	{
		UraPshY = 2;
	}

	//絵に関して
	//魔法防御時
	if (UraRandFF >= 7 && UraRandFF <= 9)
	{
		DrawGraph(200, 250 + UraPshY * 40, M_Character.PlayerShieldMahouM, TRUE);
	}
	//通常時
	else
	{
		DrawGraph(200, 250 + UraPshY * 40, M_Character.PlayerShieldM, TRUE);
	}
}

//敵関連(自分)
void UraGame_Enemy()
{
	DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
	DrawRectGraph(380, 120, UraGage / 2, 0, 120, 20, M_Character.PAttackBarM, FALSE, FALSE);
	if (UraEnemyAFlag == 0)
	{
		DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
	}
	//攻撃させる
	if (UraGage == 0)
	{
		UraEnemyAFlag = 1;
	}
	//攻撃のフラッグが立った
	if (UraEnemyAFlag == 1)
	{
		//コントロールに関して
		//上移動
		if ((Urapad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)
		{
			UraSentakuUp++;
		}
		else
		{
			UraSentakuUp = 0;
		}
		if (UraSentakuUp == 1 && UraSentakuY != 0)
		{
			UraSentakuY--;
		}
		//下移動
		if ((Urapad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)
		{
			UraSentakuDown++;
		}
		else
		{
			UraSentakuDown = 0;
		}
		if (UraSentakuDown == 1 && ((UraSentakunum == 0 && UraSentakuY != 1) 
			|| (UraSentakunum == 1 && UraSentakuY != 1) 
			|| (UraSentakunum == 2 && UraSentakuY != 2) 
			|| (UraSentakunum == 3 && UraSentakuY != 2)))
		{
			UraSentakuY++;
		}
		//決定キーを押したら
		if ((Urapad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)
		{
			UraSentakuK++;
		}
		else
		{
			UraSentakuK = 0;
		}
		if (UraSentakuK == 1)
		{
			UraSentakuKFlag++;
		}
		if ((Urapad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_S) != 0)
		{
			UraSentakuB++;
		}
		else
		{
			UraSentakuB = 0;
		}
		if (UraSentakuB == 5 && UraSentakunum != 0)
		{
			UraSentakunumX = 1;
			UraSentakuB = 0;
			UraSentakuY = 0;
			UraSentakunum--;
		}
		DrawFormatString(oneX + 30, 412, GetColor(255, 255, 255), "命 令");
		DrawFormatString(oneX + 30, 436, GetColor(255, 255, 255), "放 置");
		//最初の選択
		if (UraSentakunum == 0)
		{
			DrawGraph(oneX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
			//１段目で押されたら
			if (UraSentakuY == 0 && UraSentakuKFlag == 1)
			{
				UraSentakuKFlag = 0;
				UraSentakuY = 0;
				UraSentakunumX = 1;
				UraSentakunum = 1;
			}
			//２段目で押されたら
			if (UraSentakuY == 1 && UraSentakuKFlag == 1)
			{
				UraSentakuKFlag = 0;
				UraSentakuY = 0;
				UraSentakunumX = 2;
				UraSentakunum = 1;
			}
		}
		//二番目の選択
		if (UraSentakunum == 1)
		{
			DrawGraph(twoX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			//命令が選択されたら
			if (UraSentakunumX == 1)
			{
				DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "攻 撃");
				DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "挑 発");
				if (UraSentakuY == 0)
				{
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				if (UraSentakuY == 1)
				{
					SetDrawBright(125, 0, 0);
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 0)
				{
					UraSentakuKFlag = 0;
					UraSentakuY = 0;
					UraSentakunumX = 1;
					UraSentakunum = 2;
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 1)
				{
					UraSentakuKFlag = 0;
					UraSentakuY = 0;
					UraSentakunumX = 2;
					UraSentakunum = 2;
				}
			}
			//放置が選択されたら
			else if (UraSentakunumX == 2)
			{
				UraSentakunumX = 0;
				UraSentakunum = 0;
				UraEnemyAFlag = 0;
				UraGage = 240;
			}
		}
		//三番目の選択
		if (UraSentakunum == 2)
		{
			DrawGraph(threeX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "攻 撃");
			DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "挑 発");
			//攻撃が選択されたら
			if (UraSentakunumX == 1)
			{
				DrawFormatString(threeX + 30, 412, GetColor(255, 255, 255), "魔法攻撃");
				DrawFormatString(threeX + 30, 436, GetColor(255, 255, 255), " 強 攻撃");
				DrawFormatString(threeX + 30, 460, GetColor(255, 255, 255), "通常攻撃");
				if (UraSentakuY == 0)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					if (UraSentakuKFlag == 1)
					{
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 1;
						UraSentakunum = 3;
					}
				}
				if (UraSentakuY == 1)
				{
					UraEnemyX = 60;
					DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 255, 0);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					if (UraSentakuKFlag == 1)
					{
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 2;
						UraSentakunum = 3;
					}
				}
				if (UraSentakuY == 2)
				{
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					if (UraSentakuKFlag == 1)
					{
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 3;
						UraSentakunum = 3;
					}
				}
			}
			//挑発が選択されたら
			else if (UraSentakunumX == 2)
			{
				UraSentakunumX = 0;
				UraSentakunum = 0;
				UraEnemyAFlag = 0;
				UraGage = 240;
			}
		}
		//最後の選択
		if (UraSentakunum == 3)
		{
			DrawGraph(forX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "攻 撃");
			DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "挑 発");
			DrawFormatString(threeX + 30, 412, GetColor(255, 255, 255), "魔法攻撃");
			DrawFormatString(threeX + 30, 436, GetColor(255, 255, 255), " 強 攻撃");
			DrawFormatString(threeX + 30, 460, GetColor(255, 255, 255), "通常攻撃");
			//魔法攻撃が選択されたら
			if (UraSentakunumX == 1)
			{
				DrawFormatString(forX + 30, 412, GetColor(255, 255, 255), "上から");
				DrawFormatString(forX + 30, 436, GetColor(255, 255, 255), "直 進");
				DrawFormatString(forX + 30, 460, GetColor(255, 255, 255), "下から");
				if (UraSentakuY == 0)
				{
					UraEnemyX = 30;
					UraEnemyY = -60;
					DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuY == 1)
				{
					UraEnemyX = 30;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuY == 2)
				{
					UraEnemyX = 30;
					UraEnemyY = 60;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 0)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "！？");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "魔法を忘れてしまったようだ///");
					}
					if (UraCount > 50)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraCount = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 1)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "wwww");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "魔法を忘れてしまったようだ.....");
					}
					if (UraCount > 50)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraCount = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 2)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "！？//");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "МＰがなくなったようだ///");
					}
					if (UraCount > 50)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraCount = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
			}
			//通常攻撃が選択されたら
			if (UraSentakunumX == 3)
			{
				DrawFormatString(forX + 30, 412, GetColor(255, 255, 255), "上から");
				DrawFormatString(forX + 30, 436, GetColor(255, 255, 255), "直 進");
				DrawFormatString(forX + 30, 460, GetColor(255, 255, 255), "下から");
				if (UraSentakuY == 0)
				{
					UraEnemyX = 30;
					UraEnemyY = -60;
					DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				else if (UraSentakuY == 1)
				{
					UraEnemyX = 30;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				else if (UraSentakuY == 2)
				{
					UraEnemyX = 30;
					UraEnemyY = 60;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 0)
				{
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "突進つかれました。");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "メンテナンス中ということで");
					}
					if (UraCount > 50)
					{
						UraCount = 0;
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				else if (UraSentakuKFlag == 1 && UraSentakuY == 1)
				{
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "突進痛いです");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "疲労困憊中です");
					}
					if (UraCount > 50)
					{
						UraCount = 0;
						UraEnemyX = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				else if (UraSentakuKFlag == 1 && UraSentakuY == 2)
				{
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "突進疲れた");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "ちょっと休憩させて");
					}
					if (UraCount > 50)
					{
						UraCount = 0;
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
			}
			//強攻撃が選択されたら
			if (UraSentakunumX == 2)
			{
				SetDrawBright(255, 255, 0);
				DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
				SetDrawBright(255, 255, 255);
				//守ってない
				if (UraPshY != UraPswY)
				{
					UraEnemyX -= 30;
					if (UraEnemyX <= -240)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				//守ってる
				else if (UraPshY == UraPswY)
				{
					UraEnemyX -= 25;
					if (UraEnemyX <= -190)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
			}
		}
	}
}

//メインの描画
void UraGame_Draw()
{
	DrawGraph(0, 0, M_BackGround.UraBackGroundM, FALSE);	//背景
	UraGame_PlayerSword();
	UraGame_PlayerShield();
	UraGame_Enemy();
}
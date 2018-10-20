#include "Dxlib.h"
#include "PlayerSword.h"
#include "Enemy.h"
#include "Sound.h"
#include "Media.h"

#define UP -2		//上移動値
#define DOWN 2		//下移動値

//内容
int shieldpoint;			//位置
int PShMoveUBFlag = 0;		//上ボタンが押されたかどうか判定
int PShMoveDBFlag = 0;		//下ボタンが押されたかどうか判定
int leftcount = 0;			//放置してた時間
int LCD = 0;				//放置時間が600になったらダメージ受けさせる
int LeftDcount = 0;			//放置してダメージ受けた時の画像変化
bool LDC = 0;				//放置してダメージ受けた時の画像変化
int DamageY;				//ダメージ受けた表示をちょっとかっこよく
int resetDamageY;			//DamageYが活動終えたら0になるように
bool ShieldMahouFlag;		//魔法シールド持ってる
int ShieldMahou;			//クールダウンが解けるまでのカウント
int CoolDown = 100;			//魔法シールド持つためのクールダウン
bool CoolDownFlag;			//魔法シールドが使われたあと

//コントローラー
int PShpad;		//コントローラー


//プレイヤーの描画
void PlayerShield_Draw()
{
	//通常
	if (ShieldMahouFlag == 0 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
	}
	else if (ShieldMahouFlag == 1 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldMahouM, TRUE);
	}
	else
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldCoolDownM, TRUE);
	}
	//剣の勇者の攻撃が当たった
	if (AttackShield() == 1)
	{
		PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10秒");
	}
	if (PShATTACKHIT() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10秒");
	}
	//放置してダメージを受けた
	if (LDC == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		SetDrawBright(125, 0, 125);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10秒");
	}
	if (resetDamageY == 19)
	{
		DamageY = 0;
		resetDamageY = 0;
	}
}

//ラスボス描画
void PlayerShield_LastDraw()
{
	//通常
	if (ShieldMahouFlag == 0 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
	}
	else if (ShieldMahouFlag == 1 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldMahouM, TRUE);
	}
	else
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldCoolDownM, TRUE);
	}

	//剣の勇者の攻撃が当たった
	if (AttackShield() == 1)
	{
		PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10秒");
	}
	//罠が当たった
	if (PShATTACKHIT() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10秒");
	}
	//放置してダメージを受けた
	if (LDC == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		SetDrawBright(125, 0, 125);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10秒");
	}
	if (resetDamageY == 19)
	{
		DamageY = 0;
		resetDamageY = 0;
	}
}

//更新
void PlayerShield_Update()
{
	PShpad = GetJoypadInputState(DX_INPUT_PAD2);
	leftcount++;
	// カーソル移動
	if ((PShpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//上ボタンが押された(コントローラー上,キーボード上)
	{
		PShMoveUBFlag++;
		leftcount = 0;
	}
	else  //上ボタンが押されていない
	{
		PShMoveUBFlag = 0;
	}
	if ((PShpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)  //下ボタンが押された(コントローラー下,キーボード下)
	{
		PShMoveDBFlag++;
		leftcount = 0;
	}
	else  //下ボタンが押されていない
	{
		PShMoveDBFlag = 0;
	}
	if (EnemyBreak() >= 3 && CoolDown == 100 && ((PShpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_D) != 0) && ShieldMahou == 0)	//魔法シールド持ってる
	{
		ShieldMahouFlag = 1;
		leftcount = 0;
	}
	//上に移動
	if (PShMoveUBFlag == 1 && shieldpoint != UP)
	{
		shieldpoint -= 2;
	}
	//下に移動
	if (PShMoveDBFlag == 1 && shieldpoint != DOWN)
	{
		shieldpoint += 2;
	}
	if (ShieldMahouFlag == 1)
	{
		ShieldMahou++;
		if (ShieldMahou >= 30)
		{
			ShieldMahouFlag = 0;
			ShieldMahou = 0;
			CoolDownFlag = 1;
		}
	}
	if (CoolDownFlag == 1)
	{
		CoolDown--;
		if (CoolDown <= 0)
		{
			CoolDown = 100;
			CoolDownFlag = 0;
		}
	}
	//放置によるダメージ追加
	if (leftcount % 600 == 0 && leftcount != 0)
	{
		LCD++;
		LeftDcount++;
	}
	//画像変化させる
	if (LeftDcount >= 1)
	{
		LDC = 1;
		LeftDcount++;
		if (LeftDcount == 20)
		{
			DamageY = 0;
			LDC = 0;
			LeftDcount = 0;
		}
	}
}

//ラスボスでの更新
void PlayerShield_LastUpdate()
{
	PShpad = GetJoypadInputState(DX_INPUT_PAD2);
	leftcount++;
	// カーソル移動
	if ((PShpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//上ボタンが押された(コントローラー上,キーボード上)
	{
		PShMoveUBFlag++;
		leftcount = 0;
	}
	else  //上ボタンが押されていない
	{
		PShMoveUBFlag = 0;
	}
	if ((PShpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)  //下ボタンが押された(コントローラー下,キーボード下)
	{
		PShMoveDBFlag++;
		leftcount = 0;
	}
	else  //下ボタンが押されていない
	{
		PShMoveDBFlag = 0;
	}
	if (CoolDown == 100 && ((PShpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_D) != 0) && ShieldMahou == 0)	//魔法シールド持ってる
	{
		ShieldMahouFlag = 1;
		leftcount = 0;
	}
	//上に移動
	if (PShMoveUBFlag == 1 && shieldpoint != UP)
	{
		shieldpoint -= 2;
	}
	//下に移動
	if (PShMoveDBFlag == 1 && shieldpoint != DOWN)
	{
		shieldpoint += 2;
	}
	if (ShieldMahouFlag == 1)
	{
		ShieldMahou++;
		if (ShieldMahou >= 30)
		{
			ShieldMahouFlag = 0;
			ShieldMahou = 0;
			CoolDownFlag = 1;
		}
	}
	if (CoolDownFlag == 1)
	{
		CoolDown--;
		if (CoolDown <= 0)
		{
			CoolDown = 100;
			CoolDownFlag = 0;
		}
	}
	//放置によるダメージ追加
	if (leftcount % 600 == 0 && leftcount != 0)
	{
		LCD++;
		LeftDcount++;
	}
	//画像変化させる
	if (LeftDcount >= 1)
	{
		LDC = 1;
		LeftDcount++;
		if (LeftDcount == 20)
		{
			DamageY = 0;
			LDC = 0;
			LeftDcount = 0;
		}
	}
}

//リセット
void PlayerShield_R()
{
	shieldpoint = 0;
	PShMoveUBFlag = 0;
	PShMoveDBFlag = 0;
	leftcount = 0;
	LCD = 0;
	LeftDcount = 0;
	LDC = 0;
	DamageY = 0;
	resetDamageY = 0;
	ShieldMahouFlag = 0;
	ShieldMahou = 0;
	CoolDown = 100;
	CoolDownFlag = 0;
}

//位置を得る関数
int Getshieldpoint(void)
{
	return shieldpoint;
}

//放置時間によるダメージを得る関数
int Getleftcount(void)
{
	return LCD;
}

//魔法シールドを持っているかどうかを得る関数
bool GetShieldMahouFlag()
{
	return ShieldMahouFlag;
}
#include "DxLib.h"
#include "scenemgr.h"
#include "Media.h"
#include "Sound.h"

#define oneX 5			//ëIëÇÃàÍî‘ñ⁄ÇÃXç¿ïW
#define twoX 165		//ëIëÇÃìÒî‘ñ⁄ÇÃXç¿ïW
#define threeX 325		//ëIëÇÃéOî‘ñ⁄ÇÃXç¿ïW
#define forX 485		//ëIëÇÃélî‘ñ⁄ÇÃXç¿ïW
#define botanY 412		//ëIëÇÃYç¿ïW

//ÉRÉìÉgÉçÅ[ÉâÅ[
int Urapad;

//ì‡óe(óEé“ä÷òA
int UraPswX, UraPswY;		//åïÇÃóEé“ÇÃìÆÇ´
int UraPshY;				//èÇÇÃóEé“ÇÃìÆÇ´
int UraRand;				//óEé“ÇÃçUåÇÇ∑ÇÈóêêî
bool UraPSFlag;				//óEé“ÇÃçUåÇÉtÉâÉbÉO
int UraRandF;				//óêêîÇì¸ÇÍÇÈïœêî(åï
bool UraPAttackFlag;		//çUåÇÇ∑ÇÈÉtÉâÉbÉO
int UraRandFF;				//óêêîÇì¸ÇÍÇÈïœêî(èÇ

//ì‡óe(ìGä÷òA
int UraEnemyX, UraEnemyY;	//ìGÇÃìÆÇ´
bool UraEnemyAFlag;			//ìG(é©ï™)Ç™çUåÇÇ∑ÇÈÉtÉâÉbÉO

//ì‡óe(ëIëä÷òA
int UraSentakunum;			//ëIëÇ∑ÇÈèáî‘
int UraSentakuY;			//ëIëÉ{É^ÉìÇÃç¿ïW
int UraSentakunumX;			//ècÇ≈ëIëÇµÇΩÇ∆Ç±ÇÎ
int UraSentakuUp;			//ëIëÇ≈ÇÃè„à⁄ìÆÇàÍâÒÇ…Ç∑ÇÈÇΩÇﬂ
int UraSentakuDown;			//ëIëÇ≈ÇÃâ∫à⁄ìÆÇàÍâÒÇ…Ç∑ÇÈÇΩÇﬂ
int UraSentakuK;			//åàíËÇàÍâÒÇæÇØÇ…Ç∑ÇÈÇΩÇﬂ
int UraSentakuKFlag;		//ëIëÇ™åàÇ‹Ç¡ÇΩÉtÉâÉbÉO
int UraSentakuB;			//ñﬂÇÈ
int UraGage = 240;			//ÉQÅ[ÉW
int UraCount;				//ÉJÉEÉìÉg

//ÉäÉZÉbÉg
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

//ÉÅÉCÉìÇÃçXêV
void UraGame_Update()
{
	UraRand = GetRand(8) + 1;	//1Å`9Ç…ÇµÇΩ
	Urapad = GetJoypadInputState(DX_INPUT_PAD1);
	PlaySoundMem(S_BGM.UraBGMM, DX_PLAYTYPE_LOOP, FALSE);
	if (((Urapad & PAD_INPUT_7) && (Urapad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //EscÉLÅ[Ç™âüÇ≥ÇÍÇƒÇ¢ÇΩÇÁ
	{
		DxLib_End(); // DXÉâÉCÉuÉâÉäÇèIóπÇ∑ÇÈ
	}
	if (Urapad & PAD_INPUT_7)   //BACKÉLÅ[Ç™âüÇ≥ÇÍÇƒÇ¢ÇΩÇÁ
	{
		StopSoundMem(S_BGM.UraBGMM);
		UraGame_R();
		SceneMgr_ChangeScene(eScene_MenuSub); //ó†ÉÅÉjÉÖÅ[Ç…ñﬂÇÈ
	}
}

//åïÇÃóEé“ä÷òA
void UraGame_PlayerSword()
{
	//çUåÇïpìxÇÃÉJÉEÉìÉg
	if (UraEnemyAFlag == 0)
	{
		UraGage --;
	}
	//277Ç≈äÑÇËêÿÇÍÇΩéûÇ…çUåÇ
	if (UraGage % 101 == 0 && UraGage != 0)
	{
		UraPSFlag = 1;
	}

	//ìÆÇ´Ç…ä÷ÇµÇƒ
	//çUåÇÇ∑ÇÈÇ∆Ç´Ç…Ç»Ç¡ÇΩÇÁÇ«Ç±Ç…çUåÇÇ∑ÇÈÇ©óêêîÇë„ì¸
	if (UraPSFlag == 1)
	{
		UraRandF = UraRand;
		UraRandFF = UraRandF;
		UraPSFlag = 0;
	}
	//1.è„çU
	if (UraRandF == 1)
	{
		UraPAttackFlag = 1;
		UraPswY = -2;
	}
	//2.ê^çU
	else if (UraRandF == 2)
	{
		UraPAttackFlag = 1;
		UraPswY = 0;
	}
	//3.â∫çU
	else if (UraRandF == 3)
	{
		UraPAttackFlag = 1;
		UraPswY = 2;
	}
	//4.è„ñh
	else if (UraRandF == 4)
	{
		UraPswX = 0;
		UraPswY = -2;
	}
	//5.ê^ñh
	else if (UraRandF == 5)
	{
		UraPswX = 0;
		UraPswY = 0;
	}
	//6.â∫ñh
	else if (UraRandF == 6)
	{
		UraPswX = 0;
		UraPswY = 2;
	}
	//7.è„ñÇ
	else if (UraRandF == 7)
	{
		UraPswX = 0;
		UraPswY = -2;
	}
	//8.ê^ñÇ
	else if (UraRandF == 8)
	{
		UraPswX = 0;
		UraPswY = 0;
	}
	//9.â∫ñÇ
	else if (UraRandF == 9)
	{
		UraPswX = 0;
		UraPswY = 2;
	}
	//çUåÇÇ∑ÇÈÇ∆Ç´
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

	//äGÇ…ä÷ÇµÇƒ
	//í èÌéû(çUåÇÇµÇƒÇ¢Ç»Ç¢
	if(UraPswX <= 0)
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//çUåÇà íuÇ‹Ç≈à⁄ìÆíÜ
	else if (UraPswX > 0 && UraPswX <= 200)
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
	}
	//çUåÇÇ∑ÇÈ
	else
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
	}
}

//èÇÇÃóEé“ä÷òA
void UraGame_PlayerShield()
{
	//ìÆÇ´Ç…ä÷ÇµÇƒ
	//1.è„çU
	if (UraRandFF == 1)
	{
		UraPshY = 0;
	}
	//2.ê^çU
	else if (UraRandFF == 2)
	{
		UraPshY = 2;
	}
	//3.â∫çU
	else if (UraRandFF == 3)
	{
		UraPshY = 0;
	}
	//4.è„ñh
	else if (UraRandFF == 4)
	{
		UraPshY = -2;
	}
	//5.ê^ñh
	else if (UraRandFF == 5)
	{
		UraPshY = 0;
	}
	//6.â∫ñh
	else if (UraRandFF == 6)
	{
		UraPshY = 2;
	}
	//7.è„ñÇ
	else if (UraRandFF == 7)
	{
		UraPshY = -2;
	}
	//8.ê^ñÇ
	else if (UraRandFF == 8)
	{
		UraPshY = 0;
	}
	//9.â∫ñÇ
	else if (UraRandFF == 9)
	{
		UraPshY = 2;
	}

	//äGÇ…ä÷ÇµÇƒ
	//ñÇñ@ñhå‰éû
	if (UraRandFF >= 7 && UraRandFF <= 9)
	{
		DrawGraph(200, 250 + UraPshY * 40, M_Character.PlayerShieldMahouM, TRUE);
	}
	//í èÌéû
	else
	{
		DrawGraph(200, 250 + UraPshY * 40, M_Character.PlayerShieldM, TRUE);
	}
}

//ìGä÷òA(é©ï™)
void UraGame_Enemy()
{
	DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
	DrawRectGraph(380, 120, UraGage / 2, 0, 120, 20, M_Character.PAttackBarM, FALSE, FALSE);
	if (UraEnemyAFlag == 0)
	{
		DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
	}
	//çUåÇÇ≥ÇπÇÈ
	if (UraGage == 0)
	{
		UraEnemyAFlag = 1;
	}
	//çUåÇÇÃÉtÉâÉbÉOÇ™óßÇ¡ÇΩ
	if (UraEnemyAFlag == 1)
	{
		//ÉRÉìÉgÉçÅ[ÉãÇ…ä÷ÇµÇƒ
		//è„à⁄ìÆ
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
		//â∫à⁄ìÆ
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
		//åàíËÉLÅ[ÇâüÇµÇΩÇÁ
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
		DrawFormatString(oneX + 30, 412, GetColor(255, 255, 255), "ñΩ óﬂ");
		DrawFormatString(oneX + 30, 436, GetColor(255, 255, 255), "ï˙ íu");
		//ç≈èâÇÃëIë
		if (UraSentakunum == 0)
		{
			DrawGraph(oneX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
			//ÇPíiñ⁄Ç≈âüÇ≥ÇÍÇΩÇÁ
			if (UraSentakuY == 0 && UraSentakuKFlag == 1)
			{
				UraSentakuKFlag = 0;
				UraSentakuY = 0;
				UraSentakunumX = 1;
				UraSentakunum = 1;
			}
			//ÇQíiñ⁄Ç≈âüÇ≥ÇÍÇΩÇÁ
			if (UraSentakuY == 1 && UraSentakuKFlag == 1)
			{
				UraSentakuKFlag = 0;
				UraSentakuY = 0;
				UraSentakunumX = 2;
				UraSentakunum = 1;
			}
		}
		//ìÒî‘ñ⁄ÇÃëIë
		if (UraSentakunum == 1)
		{
			DrawGraph(twoX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			//ñΩóﬂÇ™ëIëÇ≥ÇÍÇΩÇÁ
			if (UraSentakunumX == 1)
			{
				DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "çU åÇ");
				DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "íß î≠");
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
			//ï˙íuÇ™ëIëÇ≥ÇÍÇΩÇÁ
			else if (UraSentakunumX == 2)
			{
				UraSentakunumX = 0;
				UraSentakunum = 0;
				UraEnemyAFlag = 0;
				UraGage = 240;
			}
		}
		//éOî‘ñ⁄ÇÃëIë
		if (UraSentakunum == 2)
		{
			DrawGraph(threeX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "çU åÇ");
			DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "íß î≠");
			//çUåÇÇ™ëIëÇ≥ÇÍÇΩÇÁ
			if (UraSentakunumX == 1)
			{
				DrawFormatString(threeX + 30, 412, GetColor(255, 255, 255), "ñÇñ@çUåÇ");
				DrawFormatString(threeX + 30, 436, GetColor(255, 255, 255), " ã≠ çUåÇ");
				DrawFormatString(threeX + 30, 460, GetColor(255, 255, 255), "í èÌçUåÇ");
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
			//íßî≠Ç™ëIëÇ≥ÇÍÇΩÇÁ
			else if (UraSentakunumX == 2)
			{
				UraSentakunumX = 0;
				UraSentakunum = 0;
				UraEnemyAFlag = 0;
				UraGage = 240;
			}
		}
		//ç≈å„ÇÃëIë
		if (UraSentakunum == 3)
		{
			DrawGraph(forX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "çU åÇ");
			DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "íß î≠");
			DrawFormatString(threeX + 30, 412, GetColor(255, 255, 255), "ñÇñ@çUåÇ");
			DrawFormatString(threeX + 30, 436, GetColor(255, 255, 255), " ã≠ çUåÇ");
			DrawFormatString(threeX + 30, 460, GetColor(255, 255, 255), "í èÌçUåÇ");
			//ñÇñ@çUåÇÇ™ëIëÇ≥ÇÍÇΩÇÁ
			if (UraSentakunumX == 1)
			{
				DrawFormatString(forX + 30, 412, GetColor(255, 255, 255), "è„Ç©ÇÁ");
				DrawFormatString(forX + 30, 436, GetColor(255, 255, 255), "íº êi");
				DrawFormatString(forX + 30, 460, GetColor(255, 255, 255), "â∫Ç©ÇÁ");
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
						DrawFormatString(320, 240, GetColor(0, 0, 0), "ÅIÅH");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "ñÇñ@ÇñYÇÍÇƒÇµÇ‹Ç¡ÇΩÇÊÇ§Çæ///");
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
						DrawFormatString(240, 240, GetColor(0, 0, 0), "ñÇñ@ÇñYÇÍÇƒÇµÇ‹Ç¡ÇΩÇÊÇ§Çæ.....");
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
						DrawFormatString(320, 240, GetColor(0, 0, 0), "ÅIÅH//");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "ÑMÇoÇ™Ç»Ç≠Ç»Ç¡ÇΩÇÊÇ§Çæ///");
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
			//í èÌçUåÇÇ™ëIëÇ≥ÇÍÇΩÇÁ
			if (UraSentakunumX == 3)
			{
				DrawFormatString(forX + 30, 412, GetColor(255, 255, 255), "è„Ç©ÇÁ");
				DrawFormatString(forX + 30, 436, GetColor(255, 255, 255), "íº êi");
				DrawFormatString(forX + 30, 460, GetColor(255, 255, 255), "â∫Ç©ÇÁ");
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
						DrawFormatString(320, 240, GetColor(0, 0, 0), "ìÀêiÇ¬Ç©ÇÍÇ‹ÇµÇΩÅB");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "ÉÅÉìÉeÉiÉìÉXíÜÇ∆Ç¢Ç§Ç±Ç∆Ç≈");
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
						DrawFormatString(320, 240, GetColor(0, 0, 0), "ìÀêií…Ç¢Ç≈Ç∑");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "îÊòJç¢úﬁíÜÇ≈Ç∑");
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
						DrawFormatString(320, 240, GetColor(0, 0, 0), "ìÀêiîÊÇÍÇΩ");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "ÇøÇÂÇ¡Ç∆ãxåeÇ≥ÇπÇƒ");
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
			//ã≠çUåÇÇ™ëIëÇ≥ÇÍÇΩÇÁ
			if (UraSentakunumX == 2)
			{
				SetDrawBright(255, 255, 0);
				DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
				SetDrawBright(255, 255, 255);
				//éÁÇ¡ÇƒÇ»Ç¢
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
				//éÁÇ¡ÇƒÇÈ
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

//ÉÅÉCÉìÇÃï`âÊ
void UraGame_Draw()
{
	DrawGraph(0, 0, M_BackGround.UraBackGroundM, FALSE);	//îwåi
	UraGame_PlayerSword();
	UraGame_PlayerShield();
	UraGame_Enemy();
}
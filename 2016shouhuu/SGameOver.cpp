#include "Dxlib.h"
#include "Scenemgr.h"
#include "Background.h"
#include "PlayerShield.h"
#include "PlayerSword.h"
#include "Enemy.h"
#include "SgameFirst.h"
#include "SgameLast.h"
#include "SgameSecond.h"
#include "Sound.h"

//�R���g���[���[
int SGOpad;	//�R���g���[���[

//�X�V
void SGameOver_Update()
{
	PlaySoundMem(S_BGM.GameOverS, DX_PLAYTYPE_LOOP, FALSE);
	SGOpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGOpad & PAD_INPUT_7) && (SGOpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	if (SGOpad & PAD_INPUT_7)   //Back
	{
		StopSoundMem(S_BGM.GameOverS);
		Background_R();
		Enemy_R();
		PlayerShield_R();
		PlayerSword_R();
		SGameFirst_R();
		SGameLast_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);
	}
}

//�`��
void SGameOver_Draw()
{
	Background_Draw();
	PlayerShield_Draw();
	PlayerSword_Draw();
	DrawFormatString(300, 220, GetColor(255, 0, 255), "Game Over");
	DrawFormatString(300, 270, GetColor(255, 0, 255), "%d / 6 �̓|���܂���", EnemyBreak());
	DrawFormatString(300, 320, GetColor(255, 255, 255), "BACK �Ń^�C�g���֖߂�܂�");
}
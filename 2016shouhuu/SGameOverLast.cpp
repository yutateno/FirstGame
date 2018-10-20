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
int SGLOpad;	//�R���g���[���[

//�X�V
void SGameOverLast_Update()
{
	PlaySoundMem(S_BGM.GameOverS, DX_PLAYTYPE_LOOP, FALSE);
	SGLOpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGLOpad & PAD_INPUT_7) && (SGLOpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	if (SGLOpad & PAD_INPUT_7)   //BACK��������Ă�����
	{
		StopSoundMem(S_BGM.GameOverS);
		Background_R();
		Enemy_R();
		PlayerShield_R();
		PlayerSword_R();
		SGameFirst_R();
		SGameLast_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);	//���j���[���
	}
}

//�`��
void SGameOverLast_Draw()
{
	LastBackground_Draw();
	PlayerSword_LastDraw();
	PlayerShield_LastDraw();
	DrawFormatString(300, 220, GetColor(255, 0, 255), "Game Over");
	DrawFormatString(300, 270, GetColor(150, 150, 150), "�����悭���āI�A�g���K���o��I�I");
	DrawFormatString(300, 320, GetColor(255, 255, 255), "BACK �Ń^�C�g���֖߂�܂�");
}
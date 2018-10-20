#include "Background.h"
#include "PlayerSword.h"
#include "PlayerShield.h"
#include "Enemy.h"
#include "DxLib.h"

//�R���g���[���[
int SGpad;

//�X�V
void SGame_Update()
{
	SGpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGpad & PAD_INPUT_7) && (SGpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	PlayerShield_Update();	//��l�v���C�̏��̗E�҂̍X�V
	Enemy_Update();			//��l�v���C�̓G�̍X�V
	PlayerSword_Update();	//��l�v���C�̌��̗E�҂̍X�V
}

//�`��
void SGame_Draw()
{
	Background_Draw();		//�w�i�̕`��
	Limit();				//�������Ԃ̂�BPlayerSword�ɂĂ���Ă܂��B�_���[�W�֘A�͂������Ȃ̂ŁB
	Enemy_Draw();			//��l�v���C�̓G�̕`��
	PlayerSword_Draw();		//��l�v���C�̌��̗E�҂̕`��
	PlayerShield_Draw();	//��l�v���C�̏��̗E�҂̕`��
}
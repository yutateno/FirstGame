#include "DxLib.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"

//�R���g���[���[
int Gpad;

//�X�V
void Game_Update()
{
	Gpad = GetJoypadInputState(DX_INPUT_PAD1);
	//�R���g���[���[start&back���L�[�{�[�hESC��������Ă�����
	if (((Gpad & PAD_INPUT_7) && (Gpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
	{
		DxLib_End(); // DX���C�u�������I������
	}
	Player_Update();	//��l�p�̃v���C���[�̍X�V
	ENEMY_Update();		//��l�p�̓G�̍X�V
}

//�`��
void Game_Draw()
{
	Background_Draw();	//�w�i�̕`��
	LIMIT();			//player�ɂčs���Ă��܂��B
	ENEMY_Draw();		//��l�p�̓G�̕`��
	Player_Draw();		//��l�p�̃v���C���[�̕`��
}
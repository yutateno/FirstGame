#include "DxLib.h"
#include "Background.h"
#include "Enemy.h"
#include "Player.h"
#include "Media.h"

//�R���g���[���[
int GLpad;

//���e
bool LastFlag = 0;	//�Q�[���I�[�o�[���������肷��悤��

//�X�V
void GameLast_Update()
{
	GLpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GLpad & PAD_INPUT_7) && (GLpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	ENEMY_Update();			//��l�p�̃��X�{�X�̍X�V
	Player_LastUpdate();	//��l�p�̃v���C���[�i���X�{�X�p�j�̍X�V
	LastFlag = 1;
}

//�`��
void GameLast_Draw()
{
	LastBackground_Draw();								//��l�p�̔w�i�̕`��
	LIMIT();											//��l�p�̐�������
	LastENEMY_Draw();									//��l�p�̃��X�{�X�̕`��
	Player_LastDraw();									//��l�p�̃v���C���[�̕`��
	DrawGraph(590, 300, M_Character.PrincessM, TRUE);	//�P
}

//���Z�b�g
void GameLast_R()
{
	LastFlag = 0;
}

//���X�{�X�ɓ����Ă�
bool GetLastFlag()
{
	return LastFlag;
}
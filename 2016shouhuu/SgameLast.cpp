#include "DxLib.h"
#include "Background.h"
#include "Enemy.h"
#include "PlayerShield.h"
#include "PlayerSword.h"
#include "Media.h"

//�R���g���[���[
int SGLpad;	//�R���g���[���[

//���e
bool SLastFlag = 0;	//���X�{�X�ɓ��B���Ă��邩�ǂ���


//�X�V
void SgameLast_Update()
{
	SGLpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGLpad & PAD_INPUT_7) && (SGLpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	Enemy_Update();				//��l�p�̃��X�{�X�̍X�V
	PlayerSword_LastUpdate();	//��l�p�̃��X�{�X�ł̌��̗E�҂̍X�V
	PlayerShield_LastUpdate();	//��l�p�̃��X�{�X�ł̏��̗E�҂̍X�V
	SLastFlag = 1;
}

//�`��
void SgameLast_Draw()
{
	LastBackground_Draw();								//���X�{�X�ł̔w�i�̕`��
	Limit();											//��l�p�̐�������
	LastEnemy_Draw();									//��l�p�̃��X�{�X�̕`��
	PlayerSword_LastDraw();								//��l�p�̃��X�{�X�ł̌��̗E�҂̕`��
	PlayerShield_LastDraw();							//��l�p�̃��X�{�X�ł̏��̗E�҂̕`��
	DrawGraph(590, 300, M_Character.PrincessM, TRUE);	//�P
}

//���Z�b�g
void SGameLast_R()
{
	SLastFlag = 0;
}

//�Q�[���I�[�o�[���������肷�邽�߂̂���
bool GetSLastFlag()
{
	return SLastFlag;
}
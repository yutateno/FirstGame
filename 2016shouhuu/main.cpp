#include "DxLib.h"
#include "SceneMgr.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�������Ɨ���ʐݒ�Ɖ�ʂ̐ݒ�
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(640, 480, 16);
	SceneMgr_Read();	////�摜�̓ǂݍ���

	//��ʍX�V & ���b�Z�[�W���� & ��ʏ���
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		SceneMgr_Update();  //�X�V
		SceneMgr_Draw();    //�`��
	}

	DxLib_End(); // DX���C�u�����I������

	return 0;
}
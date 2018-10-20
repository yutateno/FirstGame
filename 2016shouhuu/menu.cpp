#include "SceneMgr.h"
#include "DxLib.h"
#include "Sound.h"
#include "Media.h"

//�R���g���[���[
int Mpad;

//���e
int MenuUp;		//��ړ�
int MenuDown;	//���ړ�
bool ura;		//���֍s����t���b�O
bool ForceEnd;	//�����I���t���b�O
int SPoint;		// �I���J�[�\���̈ʒu

//�e�����̊֐�
void One(void);		//��l�v���C������֐�
void Two(void);		//��l�v���C������֐�
void End(void);		//�I������֐�
void Ura(void);		//���̃��j���[��ʂɂ������߂̊֐�

//�����|�C���^�z��
void(*Method[])() =
{
	One,Two,End,Ura
};

//�`��
void Menu_Draw()
{
	ForceEnd = 0;
	//�I���|�C���^�̈ʒu������
	SPoint = 0;

	SetFontSize(23);

	// ���[�v
	while (true)
	{
		//���ʂ̐ݒ�
		ChangeVolumeSoundMem(255, S_BGM.MenuS);				//�G���f�B���O�ɂĉ��ʂ��������̂��グ�邽�߂̏���
		PlaySoundMem(S_BGM.MenuS, DX_PLAYTYPE_LOOP, FALSE);
		DrawGraph(0, 0, M_BackGround.TitleM, FALSE);		//�{�^���̃J�[�\�����������߂����ōs��
		//�L�[���͂ɉ����ď���
		Mpad = GetJoypadInputState(DX_INPUT_PAD1);
		if (ura == 0)
		{
			DrawBox(30, 320 + (SPoint * 32), 46, 336 + (SPoint * 32), GetColor(255, 255, 0), TRUE);
			DrawFormatString(62, 320, GetColor(0, 0, 0), "��l�ŗV��");
			DrawFormatString(62, 352, GetColor(0, 0, 0), "��l�v���C�ŗV��");
			DrawFormatString(62, 384, GetColor(0, 0, 0), "�Q�[�����I������");

			//Esc�L�[��������Ă�����
			if (((Mpad & PAD_INPUT_7) && (Mpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
			{
				ForceEnd = 1;
				break;
			}
			//�オ�����ꂽ��
			if (((Mpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0))
			{
				MenuUp++;
			}
			else
			{
				MenuUp = 0;
			}
			//���������ꂽ��
			if (((Mpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0))
			{
				MenuDown++;
			}
			else
			{
				MenuDown = 0;
			}
			//��ړ��Ɋւ��鏈��
			if (MenuUp == 1 && SPoint != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint--;
			}
			//���ړ��Ɋւ��鏈��
			if (MenuDown == 1 && SPoint != 2)	//���ڂ��R�Ȃ̂�
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint++;
			}
			//����LB�{�^���������ꂽ�Ƃ��̏���(�����j���[�ɍs���܂�
			if ((Mpad & PAD_INPUT_Y) || CheckHitKey(KEY_INPUT_G) != 0)
			{
				ura = 1;
			}
			//����L�[������
			if ((Mpad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_Z) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				StopSoundMem(S_BGM.MenuS);
				//���ڂɉ������������s��
				Method[SPoint]();
				break;
			}
		}
		else
		{
			DrawBox(30, 320 + (SPoint * 32), 46, 336 + (SPoint * 32), GetColor(255, 255, 0), TRUE);
			DrawFormatString(62, 320, GetColor(0, 0, 0), "��l�ŗV��");
			DrawFormatString(62, 352, GetColor(0, 0, 0), "��l�v���C�ŗV��");
			DrawFormatString(62, 384, GetColor(0, 0, 0), "�Q�[�����I������");
			DrawFormatString(62, 416, GetColor(255, 0, 0), "��");				//���ւ̃R�}���h��ǉ�

			//Esc�L�[��������Ă�����
			if (((Mpad & PAD_INPUT_7) && (Mpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
			{
				ForceEnd = 1;
				break;
			}
			//��{�^���������ꂽ��
			if (((Mpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0))
			{
				MenuUp++;
			}
			else
			{
				MenuUp = 0;
			}
			//���{�^���������ꂽ��
			if (((Mpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0))
			{
				MenuDown++;
			}
			else
			{
				MenuDown = 0;
			}
			//��ړ��Ɋւ��鏈��
			if (MenuUp == 1 && SPoint != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint--;
			}
			//���ړ��Ɋւ��鏈��
			if (MenuDown == 1 && SPoint != 3)	//���ڂ��S�Ȃ̂�
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				SPoint++;
			}
			//�߂�
			if (CheckHitKey(KEY_INPUT_H) != 0)
			{
				ura = 0;
			}
			//����L�[������
			if ((Mpad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_Z) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				StopSoundMem(S_BGM.MenuS);
				//���ڂɉ������������s��
				Method[SPoint]();
				break;
			}
		}
	}
	//�����I��
	if (ForceEnd == 1)
	{
		DxLib_End(); //DX���C�u�������I������
	}
}

void One()
{
	SceneMgr_ChangeScene(eScene_Opening);   //�V�[������l�v���C
}

void Two()
{
	SceneMgr_ChangeScene(eScene_SOpening);  //�V�[�����l�v���C
}

void End()
{
	DxLib_End();							//DX���C�u�������I������
}

void Ura()
{
	SceneMgr_ChangeScene(eScene_MenuSub);	//�V�[���𗠃��j���[
}
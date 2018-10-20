#include "Dxlib.h"
#include "Scenemgr.h"
#include "Sound.h"
#include "Media.h"

#define MOJI_SIZE 23	//�����̑傫��

//���e
int DrawPointX, DrawPointY;		//�����̈ʒu
int SP, CP;						//�����̕\������
char  SOMoji;					//���̑}����
bool SOForceEnd;				//�����I��
bool SOEndFlag;					//�I���t���O
char SOOneMojiBuf[3];			//�P�������ꎞ�L���z��

//�R���g���[���[
int SOpad;		//�R���g���[���[


char String[][256] =
{
	//�e�͓��͑҂��A���͉��s�A�b�͏����čŏ�����A�d�͏I���A�c�́��ɒǉ��Ƃ��ĊG������
	"�@�Ƃ����łƂĂ����ꂢ�D���ȕP����炵�Ă���BF",
	"@�܂��A�Ƃ��鏊�Ŗ���������������ɂ��Ă���BF",
	"@���̏�͏L���Ă��܂�Ȃ��Ɛl�X�̊ԂŒm��n���Ă����BF",
	"@@�@����Ȃ�����A�P�������ɂ�����Ă��܂����BF",
	"@�ƂĂ��L�����F",
	"C�@�ƁA�����킯�ŌN�����͗E�҂Ƃ��ĕP���~���Ăق����BF",
	"D��l�͍ŋ��̌��������A��l�͍ŋ��̏��������Ă���BF",
	"@�������Ă���Ǝv�����N�������P���L����ł͒��������Ȃ�F",
	"@���������Ă��T�����ŃR���b�Ǝ���ł��܂��ƍl���Ă���BF",
	"@���񂾂��BF",
	"C����Ɛ퓬����F",
	"@�@�܂��͔z�u�Ƃ��č����猕�̗E�ҁA���̗E�ҁA�G�ƂȂ��Ă���BF",
	"@�@�����������E�҂͍��X�e�B�b�N�ŏc�̈ړ����ł��A�a�ōU������BF",
	"@�@�����������E�҂͍��X�e�B�b�N�̏c�ړ��݂̂��BF",
	"@�@���܂ɏ������o�܂��BF"
	"@@�Ƃ������ƂŊ撣���Ă��������B"
	"@�n�܂�܂��BFE"
};

//���s���鏈��
void SOpening_Update()
{
	// �`��s�ʒu���������
	DrawPointY++;

	// �`�����ŏ��ɖ߂�
	DrawPointX = 0;
}

//�`��
void SOpening_Draw()
{
	SOpad = GetJoypadInputState(DX_INPUT_PAD1);
	DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
	PlaySoundMem(S_BGM.SetumeiS, DX_PLAYTYPE_LOOP, FALSE);
	//�`��ʒu�̏����ʒu�Z�b�g
	DrawPointX = 0;
	DrawPointY = 0;

	//�Q�ƕ����ʒu���Z�b�g
	SP = 0;	// �P�s�ڂ�
	CP = 0;	// �O����

	//�I���t���O��|��
	SOEndFlag = 0;
	SOForceEnd = 0;

	//�L�[�{�[�h�̃X�y�[�X���R���g���[���[�̃X�^�[�g�������ꂽ��X�L�b�v����
	while (SOEndFlag == 0 && (SOpad & PAD_INPUT_8) == 0 && CheckHitKey(KEY_INPUT_SPACE) == 0)
	{
		SOpad = GetJoypadInputState(DX_INPUT_PAD1);
		//Esc�L�[��������Ă�����
		if (((SOpad & PAD_INPUT_7) && (SOpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
		{
			SOForceEnd = 1;
			break;
		}

		//�����̕`��
		SOMoji = String[SP][CP];
		switch (SOMoji)
		{
		case '@':	//���s����

			//���s��������юQ�ƕ����ʒu����i�߂�
			SOpening_Update();
			CP++;

			break;

		case 'F':	//�{�^�������҂�����

			//K����͂���Ɛi��
			if ((SOpad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_K) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				CP++;
			}
			break;

		case 'E':	//�I������

			//�I���t���O�𗧂Ă邨��юQ�ƕ����ʒu����i�߂�
			SOEndFlag = 1;
			CP++;

			break;

		case 'C':	//�N���A����

			//��ʂ����������ĕ`�敶���ʒu�������ʒu�ɖ߂�����юQ�ƕ����ʒu����i�߂�
			DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
			DrawPointY = 0;
			DrawPointX = 0;
			CP++;

			break;

		case 'D':  //'Q'�̓���ɒǉ��ŗE�҂̊G������

			//���s��������юQ�ƕ����ʒu����i�߂�
			SOpening_Update();
			CP++;
			
			//�E�҂̃O���t�B�b�N
			DrawGraph(210, 300, M_Character.PlayerSwordM[2], TRUE);
			DrawGraph(400, 300, M_Character.PlayerShieldM, TRUE);

			break;

		default:	//���̑��̕���

			//�P�����������o��
			SOOneMojiBuf[0] = String[SP][CP];
			SOOneMojiBuf[1] = String[SP][CP + 1];
			SOOneMojiBuf[2] = '\0';

			//�P�����`��
			DrawString(DrawPointX * MOJI_SIZE, DrawPointY * MOJI_SIZE, SOOneMojiBuf, GetColor(0, 0, 0));	//�����F��������

			//�Q�ƕ����ʒu���Q�o�C�g���߂�
			CP += 2;

			//�J�[�\�����ꕶ�����i�߂�
			DrawPointX++;

			//�����҂�
			WaitTimer(30);

			//��ʂ���͂ݏo������s����
			if (DrawPointX * MOJI_SIZE + MOJI_SIZE > 640)
			{
				SOpening_Update();
			}

			break;
		}

		if (String[SP][CP] == '\0')
		{
			SP++;
			CP = 0;
		}
		if (SOForceEnd == 1)
		{
			DxLib_End(); //DX���C�u�������I������
		}
		if (SOEndFlag == 1)
		{
			break;
		}
	}
	StopSoundMem(S_BGM.SetumeiS);
	SceneMgr_ChangeScene(eScene_SgameFirst);   //�V�[�����l�p�̐퓬�O�ɂɈڂ�
}
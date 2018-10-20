#include "Dxlib.h"
#include "Scenemgr.h"
#include "Sound.h"
#include "Media.h"

#define MOJI_SIZE 23

//���e
int DrawPX, DrawPY;		//��Ɋւ��邱��
int sp, cp;				//�����Ɋւ��邱��
char  OMoji;			//���̑}����
bool OEndFlag;			//�I���t���O
char OOneMojiBuf[3];	//�P�������ꎞ�L���z��
bool OForceEnd;			//�����I��

//�R���g���[���[
int Opad;


char string[][256] =
{
	//�e�͓��͑҂��A���͉��s�A�b�͏����čŏ�����A�d�͏I���A�c�́��ɒǉ��Ƃ��ĊG������
	"�@�Ƃ����ŕP����炵�Ă���BF",
	"@�܂��A�Ƃ��鏊�Ŗ���������������ɂ��Ă���BF",
	"@���̏�͓ł������Ă��āA�ނ�݂ɐl���߂Â��Ȃ��Ƃ���ɂȂ��Ă���BF",
	"@�@����Ȃ�����A�ƂĂ��߂������ƂɕP�������ɂ�����Ă��܂����BF",
	"C�@�ƁA�����킯�ŌN�ɂ͗E�҂Ƃ��ĕP���~���Ăق����BF",
	"D�N�͍ŋ��̌��������Ă���BF",
	"@�������Ă���Ƃ͎v�����P���N���ł̏�ł͒��������Ȃ��BF",
	"@���������ĂT���ŃR���b�Ǝ���ł��܂��ƍl���Ă���BF",
	"@���񂾂��BF",
	"C����Ɛ퓬����F",
	"@�@�܂��z�u�Ƃ��Ă͍��ɗE�ҁA�E�ɓG������BF",
	"@�@�N�͍U���݂̂Ŋ撣��Ȃ���΂Ȃ�Ȃ��BF",
	"@@�@���X�e�B�b�N�ŏc�Ɉړ����Ăa�ōU�����BF",
	"@�@�����čU�����������ɂȂ�����Ƃɂ���������BF",
	"@@��ς����撣���Ă���B"
	"@�n�܂�܂��BFE"
};

//���s���鏈��
void Opening_Update()
{
	//�`��s�ʒu���������
	DrawPY++;

	//�`�����ŏ��ɖ߂�
	DrawPX = 0;
}

//�`��
void Opening_Draw()
{
	Opad = GetJoypadInputState(DX_INPUT_PAD1);
	DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
	PlaySoundMem(S_BGM.SetumeiS, DX_PLAYTYPE_LOOP, FALSE);
	//�`��ʒu�̏����ʒu�Z�b�g
	DrawPX = 0;
	DrawPY = 0;

	//�Q�ƕ����ʒu���Z�b�g
	sp = 0;	//�P�s�ڂ�
	cp = 0;	//�O����

	//�I���t���O��|��
	OEndFlag = 0;
	//�����I���t���O��|��
	OForceEnd = 0;

	//�L�[�{�[�h�̃X�y�[�X���R���g���[���[�̃X�^�[�g�������ꂽ��X�L�b�v����
	while (OEndFlag == 0 && CheckHitKey(KEY_INPUT_SPACE) == 0 && (Opad & PAD_INPUT_8) == 0)
	{
		Opad = GetJoypadInputState(DX_INPUT_PAD1);
		//Esc�L�[��������Ă�����
		if (((Opad & PAD_INPUT_7) && (Opad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
		{
			OForceEnd = 1;
			break;
		}

		//�����̕`��
		OMoji = string[sp][cp];
		switch (OMoji)
		{
		case '@':	//���s����

			//���s��������юQ�ƕ����ʒu����i�߂�
			Opening_Update();
			cp++;

			break;

		case 'F':	//�{�^�������҂�����

			//�R���g���[���[B���L�[�{�[�hK����͂���Ɛi��
			if ((Opad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_K) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				cp++;
			}
			break;

		case 'E':	//�I������

			//�I���t���O�𗧂Ă邨��юQ�ƕ����ʒu����i�߂�
			OEndFlag = 1;
			cp++;

			break;

		case 'C':	//�N���A����

			//��ʂ����������ĕ`�敶���ʒu�������ʒu�ɖ߂�����юQ�ƕ����ʒu����i�߂�
			DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
			DrawPY = 0;
			DrawPX = 0;
			cp++;

			break;

		case 'D':  //'Q'�̓���ɒǉ��ŗE�҂̊G������

			//���s��������юQ�ƕ����ʒu����i�߂�
			Opening_Update();
			DrawGraph(300, 300, M_Character.PlayerSwordM[2], TRUE);
			cp++;

			break;

		default:	//���̑��̕���

			//�P�����������o��
			OOneMojiBuf[0] = string[sp][cp];
			OOneMojiBuf[1] = string[sp][cp + 1];
			OOneMojiBuf[2] = '\0';

			//�P�����`��
			DrawString(DrawPX * MOJI_SIZE, DrawPY * MOJI_SIZE, OOneMojiBuf, GetColor(0, 0, 0));	//�����F��������

			//�Q�ƕ����ʒu���Q�o�C�g���߂�
			cp += 2;

			//�J�[�\�����ꕶ�����i�߂�
			DrawPX++;

			//�����҂�
			WaitTimer(30);

			//��ʂ���͂ݏo������s����
			if (DrawPX * MOJI_SIZE + MOJI_SIZE > 640)
			{
				Opening_Update();
			}

			break;
		}
		if (string[sp][cp] == '\0')
		{
			sp++;
			cp = 0;
		}
		//�����I��
		if (OForceEnd == 1)
		{
			DxLib_End(); // DX���C�u�������I������
		}
		if (OEndFlag == 1)
		{
			break;
		}
	}
	StopSoundMem(S_BGM.SetumeiS);
	SceneMgr_ChangeScene(eScene_GameFirst);   //�V�[������l�p�̐퓬�O�Ɉڂ�
}
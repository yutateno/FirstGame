#include "DxLib.h"
#include "SGame.h"
#include "Game.h"
#include "Menu.h"
#include "SOpening.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "SgameFirst.h"
#include "Opening.h"
#include "SGameOver.h"
#include "SgameSecond.h"
#include "SgameLast.h"
#include "SGameOverLast.h"
#include "SgameEnd.h"
#include "GameFirst.h"
#include "GameOver.h"
#include "GameSecond.h"
#include "GameLast.h"
#include "GameLastOver.h"
#include "GameEnd.h"
#include "Sound.h"
#include "Media.h"
#include "Menusub.h"
#include "UraGame.h"

//�摜�̓ǂݍ���
void SceneMgr_Read()
{
	Enemy_Read();
	Sound_Read();
	Media_Read();
}

static eScene Scene = eScene_Menu;    //�V�[���Ǘ��ύX

//�X�V
void SceneMgr_Update()
{
	//�V�[���ɂ���ď����𕪊�
	switch (Scene)
	{ 
	case eScene_UraGame:		//���Q�[��
		UraGame_Update();		//�X�V����
		break;
	
	case eScene_Game:			//��l�v���C���
		Game_Update();			//�X�V����
		break;

	case eScene_GameFirst:		//��l�p�̃I�[�v�j���O��
		GameFirst_Update();		//�X�V����
		break;

	case eScene_GameOver:		//��l�p�̃Q�[���I�[�o�[
		GameOver_Update();		//�X�V����
		break;

	case eScene_GameSecond:		//��l�p�̎G�����
		GameSecond_Update();	//�X�V����
		break;

	case eScene_GameLast:		//��l�p�̃��X�{�X
		GameLast_Update();		//�X�V����
		break;

	case eScene_GameOverLast:	//��l�p�̃{�X��Q�[���I�[�o�[
		GameOverLast_Update();	//�X�V����
		break;

	case eScene_GameEnd:		//��l�p�̃G���f�B���O
		GameEnd_Update();		//�X�V����
		break;

	case eScene_SGame:			//��l�v���C���
		SGame_Update();			//�X�V����
		break;

	case eScene_SgameFirst:		//��l�v���C�̃I�[�v�j���O��̉��
		SgameFirst_Update();	//�X�V����
		break;

	case eScene_SGameOver:		//��l�v���C�̎G����ł̃Q�[���I�[�o�[���
		SGameOver_Update();		//�X�V����
		break;

	case eScene_SgameSecond:	//��l�v���C�̃{�X�O���
		SgameSecond_Update();	//�X�V����
		break;

	case eScene_SgameLast:		//��l�v���C�̃{�X��
		SgameLast_Update();		//�X�V����
		break;

	case eScene_SGameOverLast:	//��l�v���C�̃��X�{�X�̃Q�[���I�[�o�[
		SGameOverLast_Update();	//�X�V����
		break;

	case eScene_SgameEnd:		//��l�p�̃G���f�B���O
		SgameEnd_Update();		//�X�V����
		break;

	default:
		break;
	}
}

//�`��
void SceneMgr_Draw()
{
	//�V�[���ɂ���ď����𕪊�
	switch (Scene)
	{ 
	case eScene_Menu:			//���j���[���
		Menu_Draw();			//�`�揈��
		break;

	case eScene_MenuSub:		//�����j���[
		MenuSub_Draw();			//�`�揈��
		break;

	case eScene_UraGame:		//���Q�[��
		UraGame_Draw();			//�`�揈��
		break;

	case eScene_Game:			//��l�v���C���
		Game_Draw();			//�`�揈��
		break;

	case eScene_Opening:		//��l�v���C�̃I�[�v�j���O���
		Opening_Draw();			//�`�揈��
		break;

	case eScene_GameFirst:		//��l�p�̍ŏ�
		GameFirst_Draw();		//�`�揈��
		break;

	case eScene_GameOver:		//��l�p�̃Q�[���I�[�o�[
		GameOver_Draw();		//�`�揈��
		break;

	case eScene_GameSecond:		//��l�p�̎G�����
		GameSecond_Draw();		//�`�揈��
		break;

	case eScene_GameLast:		//��l�p�̃��X�{�X��
		GameLast_Draw();		//�`�揈��
		break;

	case eScene_GameOverLast:	//��l�p�̃Q�[���I�[�o�[
		GameOverLast_Draw();	//�`�揈��
		break;

	case eScene_GameEnd:		//��l�p�̃G���f�B���O
		GameEnd_Draw();			//�`�揈��
		break;

	case eScene_SGame:			//��l�v���C���
		SGame_Draw();			//�`�揈��
		break;

	case eScene_SOpening:		//��l�v���C�̃I�[�v�j���O���
		SOpening_Draw();		//�`�揈��
		break;

	case eScene_SgameFirst:		//��l�v���C�̍ŏ��̉��
		SgameFirst_Draw();		//�`�揈��
		break;

	case eScene_SGameOver:		//��l�v���C�̃Q�[���I�[�o�[���
		SGameOver_Draw();		//�`�揈��
		break;

	case eScene_SgameSecond:	//��l�v���C�̃{�X�O���
		SgameSecond_Draw();		//�`�揈��	
		break;

	case eScene_SgameLast:		//��l�p�̃��X�{�X��
		SgameLast_Draw();		//�`�揈��
		break;

	case eScene_SGameOverLast:	//��l�p�̃��X�{�X�̃Q�[���I�[�o�[
		SGameOverLast_Draw();	//�`�揈��
		break;

	case eScene_SgameEnd:		//��l�p�̃G���f�B���O
		SgameEnd_Draw();		//�`�揈��
		break;

	default:
		break;
	}
}

//nextScene �ɃV�[����ύX����
void SceneMgr_ChangeScene(eScene NextScene)
{
	Scene = NextScene;
}
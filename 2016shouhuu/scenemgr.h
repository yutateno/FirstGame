#pragma once

typedef enum
{
	eScene_Menu,			//���j���[���
	eScene_MenuSub,			//�����j���[���
	eScene_UraGame,			//���Q�[��
	
	//��l�Ɋւ��邱��
	eScene_Game,			//��l�v���C���
	eScene_Opening,			//��l�p�̃I�[�v�j���O
	eScene_GameFirst,		//��l�p�̍ŏ�
	eScene_GameOver,		//��l�p�̎G����ł̃Q�[���I�[�o�[
	eScene_GameSecond,		//��l�p�̃��X�{�X�O
	eScene_GameLast,		//��l�p�̃Q�[���Ō�
	eScene_GameOverLast,	//��l�p�̃��X�{�X�ł̃Q�[���I�[�o�[
	eScene_GameEnd,			//��l�p�̃G���h

	//��l�v���C�Ɋւ��邱��
	eScene_SGame,			//��l�v���C���(second)
	eScene_SOpening,		//��l�v���C�̃I�[�v�j���O���
	eScene_SgameFirst,		//��l�v���C�p�Q�[���̍ŏ�
	eScene_SGameOver,		//��l�p�̎G���퓬���̃Q�[���I�[�o�[
	eScene_SgameSecond,		//��l�p�̃{�X�O
	eScene_SgameLast,		//��l�p�̃��X�{�X
	eScene_SGameOverLast,	//��l�p�̃��X�{�X�̃Q�[���I�[�o�[
	eScene_SgameEnd,		//��l�p�̍Ō�
} eScene;

//�摜�̓ǂݍ���
void SceneMgr_Read();

//�V�[���̍X�V
void SceneMgr_Update();

//�V�[���̕`��
void SceneMgr_Draw();

//nextScene �ɃV�[����ύX����
void SceneMgr_ChangeScene(eScene nextScene);
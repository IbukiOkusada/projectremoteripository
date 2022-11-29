//==========================================================
//
//�v���C���[�`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"			//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//==========================================================
//�}�N����`
//==========================================================
#define MAX_PLAYERPARTS		(64)	//�v���C���[�̃��f���ő吔
#define MAX_PLAYERKEY		(64)	//�v���C���[�̃L�[�ő吔
#define MAX_PLAYERMOTION	(128)	//�v���C���[���[�V�����̍ő吔
#define MAX_PLAYERFRAME		(64)	//�v���C���[���[�V�����t���[���ő吔

//==========================================================
//�񋓌^�̒�`
//==========================================================
//���[�V�����p�^�[��
typedef enum
{
	PLAYERMOTION_NEUTRAL = 0,	//�ҋ@���[�V����
	PLAYERMOTION_WALK,			//�������[�V����
	PLAYERMOTION_ACTION,		//�A�N�V�������[�V����
	PLAYERMOTION_JUMP,			//�W�����v���[�V����
	PLAYERMOTION_LAND,			//���n���[�V����
	PLAYERMOTION_MAX
}PLAYERMOTION;

//���[�v
typedef enum
{
	PLAYERLOOP_OFF = 0,	//���[�v�I�t
	PLAYERLOOP_ON,		//���[�v�I��
	PLAYERLOOP_MAX
}PLAYERLOOP;

//==========================================================
//�\���̂̒�`
//==========================================================
//�L�[�����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
}KeyData;

//�L�[���
typedef struct
{
	KeyData aKeyData[MAX_PLAYERMOTION];		//�L�[���̔z��
	D3DXVECTOR3 rotDiff[MAX_PLAYERMOTION];	//�����̍���
	D3DXVECTOR3 posDiff[MAX_PLAYERMOTION];	//���W�̍���
}Key;

//���[�V�����f�[�^
typedef struct
{
	int aNumKey[MAX_PLAYERMOTION];		//�e���[�V�����̃L�[��
	int aNumFrame[MAX_PLAYERMOTION][MAX_PLAYERKEY];		//�e���[�V�����̃L�[�t���[����
	int PatternKey;	//���݂̃L�[��
	int nNowFrame;	//���݂̃t���[����
	int nMotionNum;	//���݂̃��[�V�����ԍ�
	int aLoop[MAX_PLAYERMOTION];		//���[�v���邩�ǂ���
}MotionData;

//�v���C���[
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 movespeed;	//�ړ����x
	float fJumpPower;		//�v���C���[�̃W�����v��
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nShadow;			//�e�̔ԍ�
	MotionData MotionData;	//���[�V�������
	bool bMove;				//�ړ����Ă��邩�ǂ���
	D3DXVECTOR3 vtxMin;		//�ŏ��l
	D3DXVECTOR3 vtxMax;		//�ő�l
	float fRotMove;			//���݂̊p�x
	float fRotDiff;			//�ړI�̊p�x
	float fRotDest;			//�p�x�v�Z�p
}Player;

//�v���C���[�̃p�[�c
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 Setpos;		//������ʒu
	D3DXVECTOR3 rot;		//����
	Key aKey[MAX_PLAYERKEY];	//�L�[���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nPartNum;			//�g�p����p�[�c�̔ԍ�
	int nParent;			//�e�̔ԍ�

	LPD3DXMESH pMesh = {};	//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = {};	//�}�e���A���ւ̃|�C���^
	LPDIRECT3DTEXTURE9 apTexture[20] = {};	//�e�N�X�`���̃|�C���^
	DWORD dwNumMat = 0;		//�}�e���A���̐�
}PlayerParts;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif

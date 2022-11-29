//==========================================================
//
//�Ǖ`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"			//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//==========================================================
//�}�N����`
//==========================================================
#define MAX_WALL	(5)		//�ǂ̍ő吔

//==========================================================
//�ǂ̍\����
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p����Ă��邩�ǂ���
}Wall;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
Wall *GetWall(void);

#endif

//===================================================
//
//�e�̔��ˏ���
//Author ����ɐ�
//
//===================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"			//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//===================================================
//�}�N����`
//===================================================
#define MAX_BULLET	(128)	//�e�̍ő吔

//===================================================
//�e�̍\����
//===================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nShadow;			//�g�p����e�̔ԍ�
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p����Ă��邩�ǂ���
}Bullet;

//===================================================
//�v���g�^�C�v�錾
//===================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fWidth);
int GetTrueBullet(void);
Bullet *GetBullet(void);

#endif 

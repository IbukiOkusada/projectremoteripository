//==========================================================
//
//�e�`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"		//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include


//==========================================================
//�e�̍\����
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;					//�g�p����Ă��邩�ǂ���
}Shadow;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);
void SetPositionShadow(int nIdexShadow, D3DXVECTOR3 pos);
void ResetShadow(int nShadow);

#endif

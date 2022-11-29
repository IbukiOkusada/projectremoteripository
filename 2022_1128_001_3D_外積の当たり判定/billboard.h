//==========================================================
//
//�r���{�[�h�`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"		//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//==========================================================
//�ǂ̍\����
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p����Ă��邩�ǂ���
}Billboard;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight);

#endif

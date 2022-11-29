//===================================================
//
//��������
//Author ����ɐ�
//
//===================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"		//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//===================================================
//�}�N����`
//===================================================
#define MAX_EXPLOSION	(128)	//�����̍ő吔

//==========================================================
//�e�̍\����
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p����Ă��邩�ǂ���
	int nCounterAnim;		//�A�j���[�V�����J�E���^�[
	int nPatternAnim;		//�A�j���[�V�����p�^�[��No.
	float fRadian;			//���a
}Explosion;

//===================================================
//�v���g�^�C�v�錾
//===================================================
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadian);

#endif 

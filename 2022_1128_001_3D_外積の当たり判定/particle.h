//===================================================
//
//�p�[�e�B�N������
//Author ����ɐ�
//
//===================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"		//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, float fRange);
void ResetParticle(void);

#endif

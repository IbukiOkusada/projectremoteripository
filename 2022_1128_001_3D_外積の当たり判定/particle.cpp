//===================================================
//
//�p�[�e�B�N������
//Author ����ɐ�
//
//===================================================
#include "particle.h"
#include "effect.h"
#include <stdlib.h>

//===================================================
//�}�N����`
//===================================================
#define MAX_PARTICLE			(128)		//�p�[�e�B�N���̍ő吔
#define MIN_MOVEPARTICLE		(3.8f)		//�p�[�e�B�N���̍Œ�ړ���
#define MIN_BLOCKMOVEPARTICLE	(0.8f)		//�u���b�N�Œ�ړ���
#define DOUBLEPI				(628)		//������̐����p�x
#define PI						(314)		//�������̐����p�x
#define HALFPI					(157)		//�l���̈�����̐����p�x

//===================================================
//�\���̂̒�`
//===================================================
//�p�[�e�B�N��
typedef struct
{
	D3DXVECTOR3 pos;	//���W
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�F
	float fRadius;		//���a
	float fRange;		//���a
	int nLife;			//����
	int nType;			//���
	int nAppear;		//1�t���[���ɏo����
	int nMeLife;		//����ɗ^�������
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Particle;

//===================================================
//�O���[�o���ϐ�
//===================================================
Particle g_aParticle[MAX_PARTICLE];	//�p�[�e�B�N���̏��

//===================================================
//�v���g�^�C�v�錾
//===================================================
void SetChangeParticle(Particle * pParticle);
void SetMoveParticle(Particle * pParticle);
void SetJumpParticle(Particle * pParticle);
void SetDiveParticle(Particle * pParticle);

//===================================================
//�p�[�e�B�N���̏�����
//===================================================
void InitParticle(void)
{
	//�z��̏�񏉊���
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(50.0, 0.0f, 0.0f);		//���W��������
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ�������
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F��������
		g_aParticle[nCntParticle].fRadius = 0.0f;							//���a��������
		g_aParticle[nCntParticle].nLife = 0;								//�̗͂�������
		g_aParticle[nCntParticle].nMeLife = 0;								//����ɗ^����̗͂�������
		g_aParticle[nCntParticle].bUse = false;								//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//===================================================
//�p�[�e�B�N���̏I��
//===================================================
void UninitParticle(void)
{

}

//===================================================
//�p�[�e�B�N���̍X�V
//===================================================
void UpdateParticle(void)
{
	Particle *pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == true)
		{//�g�p����Ă�����
			switch (pParticle->nType)
			{
			case EFFECTTYPE_NORMAL:
				for (int nCntAppear = 0; nCntAppear < 15; nCntAppear++)
				{
					//�����ʒu�̐ݒ�
					pParticle->pos = pParticle->pos;
					//�ړ��ʂ̐ݒ�
					pParticle->move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 51 - 25)) / 10.0f;
					pParticle->move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 51 - 25)) / 10.0f;
					pParticle->move.z = sinf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 51 - 25)) / 10.0f;
					//�F�̐ݒ�
					pParticle->col = D3DXCOLOR(1.0f, (float)(rand() % 6) / 10.0f, 0.0f, 1.0f);
					//���a�̐ݒ�
					pParticle->fRadius = 10.0f;
					//�����̐ݒ�
					pParticle->nMeLife = 20;

					//�G�t�F�N�g�̐ݒ�
					SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);

				}
				break;
			case EFFECTTYPE_MOVE:	//�ړ�
				SetMoveParticle(pParticle);
				break;
			case EFFECTTYPE_JUMP:	//�W�����v
				SetJumpParticle(pParticle);
				break;
			case EFFECTTYPE_CHANGE:	//�d�͏�ԕύX
				SetChangeParticle(pParticle);
				break;
			case EFFECTTYPE_DIVE:	//��э��ݒ�
				SetDiveParticle(pParticle);
				break;
			}

			//���������炷
			pParticle->nLife--;

			if (pParticle->nLife <= 0)
			{//�����������Ȃ����ꍇ
				pParticle->bUse = false;
			}
		}
	}
}

//===================================================
//�p�[�e�B�N���̐ݒ�
//===================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, float fRange)
{
	Particle *pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			pParticle->pos = pos;			//���W��ݒ�
			pParticle->nType = nType;		//��ނ�ݒ�
			pParticle->fRange = fRange;		//�z�u���a��ݒ�
			pParticle->move = move;
			//�̗�
			switch (pParticle->nType)
			{
			case EFFECTTYPE_NORMAL:
				pParticle->nLife = 0;
				break;
			}

			pParticle->bUse = true;		//�g�p���Ă����Ԃ�

			break;
		}
	}
}

//===================================================
//�p�[�e�B�N���̃��Z�b�g
//===================================================
void ResetParticle(void)
{
	Particle *pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		pParticle->bUse = false;
	}
}

//===================================================
//
//�p�[�e�B�N���̊e�ݒ�
//
//===================================================
//�d�͕ω��p�[�e�B�N���̐ݒ�
//===================================================
void SetChangeParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;
	for (int nCntAppear = 0; nCntAppear < 25; nCntAppear++)
	{
		//�����ʒu�̐ݒ�
		pParticle->pos = pParticle->pos;
		//�ړ��ʂ̐ݒ�
		pParticle->move.x = sinf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 30.0f;
		pParticle->move.y = cosf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 30.0f;
		//�F�̐ݒ�
		pParticle->col = D3DXCOLOR(rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 0.9f);
		//���a�̐ݒ�
		pParticle->fRadius = rand() % 4000 / 100.0f;
		//�����̐ݒ�
		pParticle->nMeLife = 25;

		//�G�t�F�N�g�̐ݒ�
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);

	}
}

//===================================================
//�ړ��p�[�e�B�N���̐ݒ�
//===================================================
void SetMoveParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;

	for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++)
	{
		float fRotMove;	//���݂̈ړ���

		fRotMove = atan2f(pParticle->move.x, pParticle->move.y);	//���݂̈ړ�����(�p�x)

																	//�����ʒu�̐ݒ�
		pParticle->pos = pParticle->pos;
		//�ړ��ʂ̐ݒ�
		pParticle->move.x = sinf((float)(rand() % 315 - 157) / 100.0f) * (float)(rand() % 100) * 0.1f;
		pParticle->move.z = sinf((float)(rand() % 315 - 157) / 100.0f) * (float)(rand() % 100) * 0.1f;
		pParticle->move.y = cosf((float)(rand() % 315 - 157) / 100.0f) * (float)(rand() % 100) * 0.1f;
		//�F�̐ݒ�
		pParticle->col = D3DXCOLOR(1.0f, (rand() % 5 + 5) * 0.1f, 0.0f, 1.0f);
		//���a�̐ݒ�
		pParticle->fRadius = (float)(rand() % 100) * 0.1f;
		//�����̐ݒ�
		pParticle->nMeLife = 100;
		//�G�t�F�N�g�̐ݒ�
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);
	}
}

//===================================================
//�W�����v�p�[�e�B�N���̐ݒ�
//===================================================
void SetJumpParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;

	for (int nCntAppear = 0; nCntAppear < 
		2; nCntAppear++)
	{
		//�����ʒu�̐ݒ�
		pParticle->pos = pParticle->pos;
		//�ړ��ʂ̐ݒ�
		pParticle->move.y = cosf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 300.0f;
		pParticle->move.x = cosf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 50.0f;
		//�F�̐ݒ�
		pParticle->col = D3DXCOLOR(0.5f, 0.4f, 0.4f, 0.9f);
		//���a�̐ݒ�
		pParticle->fRadius = 20.0f;
		//�����̐ݒ�
		pParticle->nMeLife = 20;

		//�G�t�F�N�g�̐ݒ�
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);
	}
}

//===================================================
//��э��ݒ��p�[�e�B�N���̐ݒ�
//===================================================
void SetDiveParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;

	int nMove = (int)pParticle->move.y * 2;

	if (nMove < 0)
	{
		nMove *= -1;
	}

	for (int nCntAppear = 0; nCntAppear < nMove; nCntAppear++)
	{
		//�ړ��ʂ̐ݒ�
		pParticle->move.x = 0.0f;
		//�F�̐ݒ�
		pParticle->col = D3DXCOLOR(0.4f, 0.7f, 0.8f, 0.05f);
		//���a�̐ݒ�
		pParticle->fRadius = rand() % 500 / 10.0f;
		//�����̐ݒ�
		pParticle->nMeLife = 10;

		//�G�t�F�N�g�̐ݒ�
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);
	}
}
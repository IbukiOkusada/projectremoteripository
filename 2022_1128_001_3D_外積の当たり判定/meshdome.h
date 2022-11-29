//==========================================================
//
//���b�V���h�[���`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

#include "main.h"

//==========================================================
//�}�N����`
//==========================================================

//==========================================================
//�ǂ̍\����
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fLength;			//����
	float fRot;				//�꒸�_�̊p�x
	float fHeightRot;		//y���p�x
	float fHeight;			//����
	int nWidthMesh;			//������
	int nVertex;			//���_��
	int nIndex;				//�`��ԍ���
	bool bUse;				//�g�p����Ă��邩�ǂ���
}MeshDome;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitMeshDome(void);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth);
void SetIndexMeshDome(int nCntWall);
void SetpVtxMeshDome(int nCntWall);

#endif

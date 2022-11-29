//==========================================================
//
//���b�V���V�����_�[�`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_MESHCYLINDER	(5)		//�ǂ̍ő吔

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
	float fHeight;			//����
	int nWidthMesh;			//������
	int nVertex;			//���_��
	int nIndex;				//�`��ԍ���
	bool bUse;				//�g�p����Ă��邩�ǂ���
}MeshCylinder;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth);
void SetIndexMeshCylinder(int nCntWall);
void SetpVtxMeshCylinder(int nCntWall);

#endif

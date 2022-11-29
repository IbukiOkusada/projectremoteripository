//==========================================================
//
//���b�V���E�H�[���`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_MESHWALL	(5)		//�ǂ̍ő吔

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
	int nHeightMesh;		//��������
	int nWidthMesh;			//������
	int nVertex;			//���_��
	int nIndex;				//�`��ԍ���
	bool bUse;				//�g�p����Ă��邩�ǂ���
}MeshWall;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth);
void SetIndexMeshWall(int nCntWall);
void SetpVtxMeshWall(int nCntWall);
MeshWall *GetMeshWall(void);

#endif

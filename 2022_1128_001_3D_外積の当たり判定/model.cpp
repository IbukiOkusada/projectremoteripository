//==========================================================
//
//���f���`�揈��
//Author ����ɐ�
//
//==========================================================
#include "main.h"
#include "Model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "bullet.h"
#include "particle.h"
#include "effect.h"

//==========================================================
//�}�N����`
//==========================================================
#define MODELFILE_DATA	"data\\MODEL\\rock000.x"		//���f���f�[�^

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void CollisonModel(void);
void CollisionModelBullet(void);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPD3DXMESH g_pMeshModel = NULL;			//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;	//�}�e���A���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureModel[20] = {};	//�e�N�X�`���̃|�C���^
DWORD g_dwNumMatModel = 0;				//�}�e���A���̐�
D3DXVECTOR3 g_posModel;					//�ʒu
D3DXVECTOR3 g_rotModel;					//����
D3DXMATRIX g_mtxWorldModel;				//���[���h�}�g���b�N�X
D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;	//���f���̍ŏ��l�ƍő�l
int g_nShadow = -1;

//==========================================================
//���f���̏���������
//==========================================================
void InitModel(void)
{
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�̃|�C���^

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//�f�o�C�X�ւ̃|�C���^

	//�e��ϐ��̏�����
	g_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_vtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODELFILE_DATA,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel);

	//���_�����擾
	nNumVtx = g_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		if (vtx.x < g_vtxMinModel.x)
		{//�ŏ��l�����l���������ꍇ
			g_vtxMinModel.x = vtx.x;
		}
		else if (vtx.x > g_vtxMaxModel.x)
		{//�ő�l�����l���傫���ꍇ
			g_vtxMaxModel.x = vtx.x;
		}

		if (vtx.z < g_vtxMinModel.z)
		{//�ŏ��l�����l���������ꍇ
			g_vtxMinModel.z = vtx.z;
		}
		else if (vtx.z > g_vtxMaxModel.z)
		{//�ő�l�����l���傫���ꍇ
			g_vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshModel->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂��Ă���
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureModel[nCntMat]);
		}
	}

	//�e�̐ݒ�
	g_nShadow = SetShadow();
}

//==========================================================
//���f���̏I������
//==========================================================
void UninitModel(void)
{
	//���b�V���̔p��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A���̔p��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}

}

//==========================================================
//���f���̍X�V����
//==========================================================
void UpdateModel(void)
{
	Camera *pCamera = GetCamera();

	//�v���C���[�̍X�V
	/*if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_posModel.x -= cosf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z += sinf(pCamera->rot.y) * pCamera->moveV.z;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_posModel.x += cosf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z -= sinf(pCamera->rot.y) * pCamera->moveV.z;
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{
		g_posModel.x += sinf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z += cosf(pCamera->rot.y) * pCamera->moveV.z;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_posModel.x -= sinf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z -= cosf(pCamera->rot.y) * pCamera->moveV.z;
	}*/

	//�e�̈ʒu�ݒ�
	//SetPositionShadow(g_nShadow, g_posModel);

	//�v���C���[�Ƃ̓����蔻��
	CollisonModel();

	//���f���ƒe�̂����蔻��
	CollisionModelBullet();

}

//==========================================================
//���f���̕`�揈��
//==========================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldModel);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		//���f��(�p�[�c)�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==========================================================
//���f���ƃv���C���[�̔���
//==========================================================
void CollisonModel(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��̃|�C���^

	if (pPlayer->pos.z + pPlayer->vtxMax.z > g_posModel.z + g_vtxMinModel.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_posModel.z + g_vtxMaxModel.z)
	{
		if (pPlayer->posOld.x + pPlayer->vtxMax.x <= g_posModel.x + g_vtxMinModel.x && pPlayer->pos.x + pPlayer->vtxMax.x > g_posModel.x + g_vtxMinModel.x)
		{//������߂荞�񂾏ꍇ
			pPlayer->pos.x = g_posModel.x + g_vtxMinModel.x - pPlayer->vtxMax.x;
		}
		if (pPlayer->posOld.x + pPlayer->vtxMin.x >= g_posModel.x + g_vtxMaxModel.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_posModel.x + g_vtxMaxModel.x)
		{//�E����߂荞�񂾏ꍇ
			pPlayer->pos.x = g_posModel.x + g_vtxMaxModel.x - pPlayer->vtxMin.x;
		}
	}
	if (pPlayer->pos.x + pPlayer->vtxMax.x > g_posModel.x + g_vtxMinModel.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_posModel.x + g_vtxMaxModel.x)
	{
		if (pPlayer->posOld.z + pPlayer->vtxMax.z <= g_posModel.z + g_vtxMinModel.z && pPlayer->pos.z + pPlayer->vtxMax.z > g_posModel.z + g_vtxMinModel.z)
		{//��O����߂荞�񂾏ꍇ
			pPlayer->pos.z = g_posModel.z + g_vtxMinModel.z - pPlayer->vtxMax.z;
		}
		if (pPlayer->posOld.z + pPlayer->vtxMin.z >= g_posModel.z + g_vtxMaxModel.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_posModel.z + g_vtxMaxModel.z)
		{//������߂荞�񂾏ꍇ
			pPlayer->pos.z = g_posModel.z + g_vtxMaxModel.z - pPlayer->vtxMin.z;
		}
	}
}

//==========================================================
//���f���ƒe�̓����蔻��
//==========================================================
void CollisionModelBullet(void)
{
	Bullet *pBullet = GetBullet();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{//�G���g�p����Ă���ꍇ
			if (pBullet->pos.x + pBullet->fWidth >= g_posModel.x + g_vtxMinModel.x &&
				pBullet->pos.x - pBullet->fWidth <= g_posModel.x + g_vtxMaxModel.x &&
				pBullet->pos.z + pBullet->fWidth >= g_posModel.z + g_vtxMinModel.z &&
				pBullet->pos.z - pBullet->fWidth <= g_posModel.z + g_vtxMaxModel.z)
			{//�G�͈͓̔��ɓ������ꍇ
				pBullet->bUse = false;
				ResetShadow(pBullet->nShadow);	//�g�p���Ă����e���g�p����Ă��Ȃ���Ԃɂ���
				SetParticle(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_MOVE, 5.0f);
			}
		}
	}
}
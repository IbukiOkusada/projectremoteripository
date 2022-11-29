//==========================================================
//
//���f���`�揈��
//Author ����ɐ�
//
//==========================================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "explosion.h"
#include "particle.h"

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void CollisonEnemy(void);
void GetSizeEnemy(int nCntEnemy);
void SetStateEnemy(int nCntEnemy);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
Enemy g_aEnemy[MAX_ENEMY];		//�G�̏��

//==========================================================
//�t�@�C����
//==========================================================
const char *c_apEnemynameEnemy[] =
{
	"data\\MODEL\\streetlamp.x",
	"data\\MODEL\\woodbox000.x",
	"data\\MODEL\\bench002.x",
};

//==========================================================
//���f���̏���������
//==========================================================
void InitEnemy(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//�f�o�C�X�ւ̃|�C���^

	//�e��ϐ��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_aEnemy[nCntEnemy].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_aEnemy[nCntEnemy].nShadow = -1;
	}

	//�G�̐ݒ�
	SetEnemy(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 0);
	SetEnemy(D3DXVECTOR3(-300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 1);
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 2);
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 2);
}

//==========================================================
//���f���̏I������
//==========================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���b�V���̔p��
		if (g_aEnemy[nCntEnemy].pMesh != NULL)
		{
			g_aEnemy[nCntEnemy].pMesh->Release();
			g_aEnemy[nCntEnemy].pMesh = NULL;
		}

		//�}�e���A���̔p��
		if (g_aEnemy[nCntEnemy].pBuffMat != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMat->Release();
			g_aEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}

}

//==========================================================
//���f���̍X�V����
//==========================================================
void UpdateEnemy(void)
{
	Camera *pCamera = GetCamera();

	//�v���C���[�Ƃ̓����蔻��
	CollisonEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���ꍇ
			//�e�̈ʒu�ݒ�
			SetPositionShadow(g_aEnemy[nCntEnemy].nShadow, g_aEnemy[nCntEnemy].pos);

			//��Ԑݒ�
			SetStateEnemy(nCntEnemy);
		}
	}
}

//==========================================================
//���f���̕`�揈��
//==========================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aEnemy[nCntEnemy].apTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==========================================================
//���f���ƃv���C���[�̔���
//==========================================================
void CollisonEnemy(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��̃|�C���^
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���ꍇ
			if (pPlayer->pos.z + pPlayer->vtxMax.z > g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z)
			{
				if (pPlayer->posOld.x + pPlayer->vtxMax.x <= g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x && pPlayer->pos.x + pPlayer->vtxMax.x > g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x)
				{//������߂荞�񂾏ꍇ
					pPlayer->pos.x = g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x - pPlayer->vtxMax.x;
				}
				if (pPlayer->posOld.x + pPlayer->vtxMin.x >= g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x)
				{//�E����߂荞�񂾏ꍇ
					pPlayer->pos.x = g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x - pPlayer->vtxMin.x;
				}
			}
			if (pPlayer->pos.x + pPlayer->vtxMax.x > g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x)
			{
				if (pPlayer->posOld.z + pPlayer->vtxMax.z <= g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z && pPlayer->pos.z + pPlayer->vtxMax.z > g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z)
				{//��O����߂荞�񂾏ꍇ
					pPlayer->pos.z = g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z - pPlayer->vtxMax.z;
				}
				if (pPlayer->posOld.z + pPlayer->vtxMin.z >= g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z)
				{//������߂荞�񂾏ꍇ
					pPlayer->pos.z = g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z - pPlayer->vtxMin.z;
				}
			}
		}
	}
}

//==========================================================
//�G�̐ݒ菈��
//==========================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, int nType)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//�f�o�C�X�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�G���g�p����Ă��Ȃ��ꍇ
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nType;
			g_aEnemy[nCntEnemy].nShadow = SetShadow();
			g_aEnemy[nCntEnemy].bUse = true;

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(c_apEnemynameEnemy[nType],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemy[nCntEnemy].pBuffMat,
				NULL,
				&g_aEnemy[nCntEnemy].dwNumMat,
				&g_aEnemy[nCntEnemy].pMesh);

			D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//�e�N�X�`���t�@�C�������݂��Ă���
				 //�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aEnemy[nCntEnemy].apTexture[nCntMat]);
				}
			}

			//�ő�l�ƍŏ��l���擾
			GetSizeEnemy(nCntEnemy);

			break;
		}
	}
}

//==========================================================
//�G�̍ő�l�ƍŏ��l�̎擾
//==========================================================
void GetSizeEnemy(int nCntEnemy)
{
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�̃|�C���^

	//���_�����擾
	nNumVtx = g_aEnemy[nCntEnemy].pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	g_aEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		if (vtx.x < g_aEnemy[nCntEnemy].vtxMin.x)
		{//�ŏ��l�����l���������ꍇ
			g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;
		}
		else if (vtx.x > g_aEnemy[nCntEnemy].vtxMax.x)
		{//�ő�l�����l���傫���ꍇ
			g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;
		}

		if (vtx.z < g_aEnemy[nCntEnemy].vtxMin.z)
		{//�ŏ��l�����l���������ꍇ
			g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;
		}
		else if (vtx.z > g_aEnemy[nCntEnemy].vtxMax.z)
		{//�ő�l�����l���傫���ꍇ
			g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_aEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
}

//==========================================================
//�G�̏��擾
//==========================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==========================================================
//�G�̃_���[�W���f
//==========================================================
void HitEnemy(int nDamage, Enemy *pEnemy)
{
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾
	pEnemy->nLife -= nDamage;

	if (pEnemy->nLife <= 0)
	{//�̗͂��Ȃ��Ȃ����ꍇ
		pEnemy->bUse = false;
		ResetShadow(pEnemy->nShadow);

		//�����̐ݒu
		SetExplosion(pEnemy->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50.0f);

		//�p�[�e�B�N���̐ݒ�
		SetParticle(pEnemy->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f);
	}
	else
	{
		D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)pEnemy->pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)pEnemy->dwNumMat; nCntMat++)
		{
			pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		}
		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);

		pEnemy->nStateCounter = 10;
		pEnemy->nState = ENEMYSTATE_DAMAGE;
	}
}

//==========================================================
//�G�̏�Ԑݒ�
//==========================================================
void SetStateEnemy(int nCntEnemy)
{
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	switch (g_aEnemy[nCntEnemy].nState)
	{
	case ENEMYSTATE_NORMAL:
		break;
	case ENEMYSTATE_DAMAGE:
		g_aEnemy[nCntEnemy].nStateCounter--;
		if (g_aEnemy[nCntEnemy].nStateCounter <= 0)
		{
			g_aEnemy[nCntEnemy].nStateCounter = 0;
			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_NORMAL;

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			}
			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}

		break;
	case ENEMYSTATE_DEATH:
		break;
	}
}
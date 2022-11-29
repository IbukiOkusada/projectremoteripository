//==========================================================
//
//���b�V���h�[���`�揈��
//Author ����ɐ�
//
//==========================================================
#include "main.h"
#include "meshdome.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\sky000.jpg"		//�e�N�X�`���f�[�^
#define MAX_FIELD		(28)			//�t�B�[���h�̒��_��
#define FIELD_WIDTH		(16)			//�t�B�[���h�̕�
#define FIELD_HEIGHT	(8)				//�t�B�[���h�̍���
#define FIELD_TYOUTEN	((FIELD_WIDTH + 1) * (FIELD_HEIGHT + 1))			//���_��
#define FIELD_WIDTHWIDTH	(4000.0f)	//������
#define FIELD_HEIGHTHEIGHT	(50.0f)	//���s����
#define MAX_WALLINDEX	(300)			//�ǂ̃C���f�b�N�X�ő吔
#define MAX_WALLVERTEX	(300)			//�ǂ̒��_�ő吔

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshDome = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshDome = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXMATRIX g_mtxWorldMeshDome;							//���[���h�}�g���b�N�X
MeshDome g_MeshDome;	//���b�V���V�����_�[�̏��

//==========================================================
//�|���S���̏���������
//==========================================================
void InitMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	D3DXVECTOR3 vecDir;				//�ݒ�ύX�p�x�N�g��

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureMeshDome);

	//�e��ϐ��̏�����
	g_MeshDome.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshDome.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshDome.fLength = FIELD_WIDTHWIDTH;
	g_MeshDome.fRot = (D3DX_PI * 2) / FIELD_WIDTH;
	g_MeshDome.fHeightRot = (D3DX_PI * 0.5f) / (FIELD_HEIGHT + 2);
	g_MeshDome.nVertex = FIELD_TYOUTEN;
	g_MeshDome.nIndex = (FIELD_WIDTH + 1) * (FIELD_HEIGHT) * 2 + (FIELD_HEIGHT) * 2;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_WALLVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshDome,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshDome->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//���_���W�̐ݒ�(��������E��O�Ɍ������Ē��_����ݒ肷��
	for (int nCntpVtx = 0; nCntpVtx < FIELD_TYOUTEN; nCntpVtx++)
	{
		float fRot = g_MeshDome.fRot * (nCntpVtx % (FIELD_WIDTH + 1) );

		if (fRot > D3DX_PI || fRot < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (fRot > D3DX_PI)
			{
				fRot += (-D3DX_PI * 2);
			}
			else if (fRot < -D3DX_PI)
			{
				fRot += (D3DX_PI * 2);
			}
		}

		float fHeightRot = g_MeshDome.fHeightRot * (nCntpVtx / (FIELD_WIDTH + 1));

		if (fHeightRot > D3DX_PI || fHeightRot < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (fHeightRot > D3DX_PI)
			{
				fHeightRot += (-D3DX_PI * 2);
			}
			else if (fHeightRot < -D3DX_PI)
			{
				fHeightRot += (D3DX_PI * 2);
			}
		}

		//���_���W
		pVtx[nCntpVtx].pos.x = (cosf(fHeightRot) * cosf(fRot)) * g_MeshDome.fLength;
		pVtx[nCntpVtx].pos.y = sinf(fHeightRot) * g_MeshDome.fLength;
		pVtx[nCntpVtx].pos.z = (cosf(fHeightRot) * sinf(fRot)) * g_MeshDome.fLength;

		vecDir = pVtx[nCntpVtx].pos - g_MeshDome.pos;

		//�x�N�g���𐳋K������
		D3DXVec3Normalize(&vecDir, &vecDir);

		//�@��
		pVtx[nCntpVtx].nor = vecDir;

		//�F
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2((1.0f / FIELD_WIDTH) * (nCntpVtx % (FIELD_WIDTH + 1)), (1.0f / FIELD_HEIGHT) * (nCntpVtx / (FIELD_WIDTH + 1)));
	}

	/*pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.375f, 0.0f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.625f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.0f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.875f, 0.0f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 1.0f);*/

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshDome->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_WALLINDEX,	//�K�v�ȃC���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshDome,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N�����_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdexBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < g_MeshDome.nIndex * 0.5f; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + FIELD_WIDTH + 1) - (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 2)) - (nCntIdx / (FIELD_WIDTH + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx + (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 1)) - (nCntIdx / (FIELD_WIDTH + 2));
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdexBuffMeshDome->Unlock();

}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitMeshDome(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔p��
	if (g_pIdexBuffMeshDome != NULL)
	{
		g_pIdexBuffMeshDome->Release();
		g_pIdexBuffMeshDome = NULL;
	}

}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdateMeshDome(void)
{
	
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	int nCntIndex = 0;

	//���C�e�B���O���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshDome.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshDome.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMeshDome,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdexBuffMeshDome);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_MeshDome.nVertex,		//�p�ӂ������_��
		nCntIndex,
		g_MeshDome.nIndex - 2	//�`�悷��v���~�e�B�u��
	);

	//���C�e�B���O���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================
//���b�V���E�H�[���̐ݒ�
//==========================================================
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth)
{
	
}

//==========================================================
//���b�V���E�H�[���̃C���f�b�N�X�ԍ��ݒ�
//==========================================================
void SetIndexMeshDome(int nCntWall)
{

}

//==========================================================
//���b�V���E�H�[����pVtx�ݒ�
//==========================================================
void SetpVtxMeshDome(int nCntWall)
{
	
}
//==========================================================
//
//���b�V���E�H�[���`�揈��
//Author ����ɐ�
//
//==========================================================
#include "main.h"
#include "meshcylinder.h"
#include "input.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\mountain000.png"		//�e�N�X�`���f�[�^
#define MAX_FIELD		(28)			//�t�B�[���h�̒��_��
#define FIELD_WIDTH		(16)				//�t�B�[���h�̕�
#define FIELD_HEIGHT	(1)				//�t�B�[���h�̍���
#define FIELD_TYOUTEN	((FIELD_WIDTH + 1) * (FIELD_HEIGHT + 1))			//���_��
#define FIELD_WIDTHWIDTH	(3800.0f)	//������
#define FIELD_HEIGHTHEIGHT	(400.0f)	//���s����
#define MAX_WALLINDEX	(256)			//�ǂ̃C���f�b�N�X�ő吔
#define MAX_WALLVERTEX	(256)			//�ǂ̒��_�ő吔

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshCylinder = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXMATRIX g_mtxWorldMeshCylinder;							//���[���h�}�g���b�N�X
MeshCylinder g_MeshCylinder;	//���b�V���V�����_�[�̏��

//==========================================================
//�|���S���̏���������
//==========================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	D3DXVECTOR3 vecDir;				//�ݒ�ύX�p�x�N�g��

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureMeshCylinder);

	//�e��ϐ��̏�����
	g_MeshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshCylinder.fLength = FIELD_WIDTHWIDTH;
	g_MeshCylinder.fRot = (D3DX_PI * 2) / FIELD_WIDTH;
	g_MeshCylinder.nVertex = FIELD_TYOUTEN;
	g_MeshCylinder.nIndex = (FIELD_WIDTH + 1) * (FIELD_HEIGHT) * 2 + (FIELD_HEIGHT) * 2;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_WALLVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//���_���W�̐ݒ�(��������E��O�Ɍ������Ē��_����ݒ肷��
	for (int nCntpVtx = 0; nCntpVtx < FIELD_TYOUTEN; nCntpVtx++)
	{
		float fRot = g_MeshCylinder.fRot * (nCntpVtx % (FIELD_WIDTH + 1) );

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

		//���_���W
		pVtx[nCntpVtx].pos.x = sinf(fRot) * g_MeshCylinder.fLength;
		pVtx[nCntpVtx].pos.y = ((FIELD_HEIGHTHEIGHT * 2) * FIELD_HEIGHT) + ((nCntpVtx / (FIELD_WIDTH + 1) * (-FIELD_HEIGHTHEIGHT * 2)));
		pVtx[nCntpVtx].pos.z = cosf(fRot) * g_MeshCylinder.fLength;

		vecDir = (pVtx[nCntpVtx].pos - g_MeshCylinder.pos);

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
	g_pVtxBuffMeshCylinder->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_WALLINDEX,	//�K�v�ȃC���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshCylinder,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N�����_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdexBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < g_MeshCylinder.nIndex / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + FIELD_WIDTH + 1) - (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 2)) - (nCntIdx / (FIELD_WIDTH + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx + (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 1)) - (nCntIdx / (FIELD_WIDTH + 2));
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdexBuffMeshCylinder->Unlock();

}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitMeshCylinder(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔p��
	if (g_pIdexBuffMeshCylinder != NULL)
	{
		g_pIdexBuffMeshCylinder->Release();
		g_pIdexBuffMeshCylinder = NULL;
	}

}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdateMeshCylinder(void)
{
	
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	int nCntIndex = 0;

	//���C�e�B���O���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshCylinder.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshCylinder.rot.y, g_MeshCylinder.rot.x, g_MeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshCylinder.pos.x, g_MeshCylinder.pos.y, g_MeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshCylinder.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMeshCylinder,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdexBuffMeshCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_MeshCylinder.nVertex,		//�p�ӂ������_��
		nCntIndex,
		g_MeshCylinder.nIndex - 2	//�`�悷��v���~�e�B�u��
	);

	//���C�e�B���O���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//==========================================================
//���b�V���E�H�[���̐ݒ�
//==========================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth)
{
	
}

//==========================================================
//���b�V���E�H�[���̃C���f�b�N�X�ԍ��ݒ�
//==========================================================
void SetIndexMeshCylinder(int nCntWall)
{

}

//==========================================================
//���b�V���E�H�[����pVtx�ݒ�
//==========================================================
void SetpVtxMeshCylinder(int nCntWall)
{
	
}
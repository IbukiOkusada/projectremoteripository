//==========================================================
//
//���b�V���E�H�[���`�揈��
//Author ����ɐ�
//
//==========================================================
#include "meshwall.h"
#include "input.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\field000.jpg"		//�e�N�X�`���f�[�^
#define MAX_FIELD		(28)			//�t�B�[���h�̒��_��
#define FIELD_WIDTH		(3)				//�t�B�[���h�̕�
#define FIELD_HEIGHT	(3)				//�t�B�[���h�̍���
#define FIELD_TYOUTEN	(4 * 4)			//���_��
#define FIELD_WIDTHWIDTH	(100.0f)	//������
#define FIELD_HEIGHTHEIGHT	(100.0f)	//���s����
#define MAX_WALLINDEX	(2048)			//�ǂ̃C���f�b�N�X�ő吔
#define MAX_WALLVERTEX	(256)			//�ǂ̒��_�ő吔

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshWall = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshWall;								//�ʒu
D3DXVECTOR3 g_rotMeshWall;								//����
D3DXMATRIX g_mtxWorldMeshWall;							//���[���h�}�g���b�N�X
int g_nVertex; //���_�̎g�p���m�F
int g_nIndex;	//�C���f�b�N�X�̎d�l���m�F
MeshWall g_aMeshWall[MAX_MESHWALL];	//���b�V���E�H�[���̏��

//==========================================================
//�|���S���̏���������
//==========================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureMeshWall);

	//�e��ϐ��̏�����
	g_posMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		g_aMeshWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].fHeight = 0.0f;
		g_aMeshWall[nCntWall].fWidth =  0.0f;
		g_aMeshWall[nCntWall].nHeightMesh = 0;
		g_aMeshWall[nCntWall].nWidthMesh = 0;
		g_aMeshWall[nCntWall].nVertex = 0;
		g_aMeshWall[nCntWall].bUse = false;
	}

	g_nVertex = 0;
	g_nIndex = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_WALLVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	//VERTEX_3D *pVtx;

	////���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	//g_pVtxBuffMeshWall->Lock(
	//	0,
	//	0,
	//	(void**)&pVtx,
	//	0);

	////���_���W�̐ݒ�(��������E��O�Ɍ������Ē��_����ݒ肷��
	//for (int nCntpVtx = 0; nCntpVtx < FIELD_TYOUTEN; nCntpVtx++)
	//{
	//	//���_���W
	//	pVtx[nCntpVtx].pos.x = -(FIELD_WIDTHWIDTH * FIELD_WIDTH) + (nCntpVtx % (FIELD_WIDTH + 1) * (FIELD_WIDTHWIDTH * 2));
	//	pVtx[nCntpVtx].pos.y = ((FIELD_HEIGHTHEIGHT * 2) * FIELD_HEIGHT) + ((nCntpVtx / (FIELD_HEIGHT + 1) * (-FIELD_HEIGHTHEIGHT * 2)));
	//	pVtx[nCntpVtx].pos.z = 0.0f;

	//	//�@��
	//	pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//	//�F
	//	pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//	pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (FIELD_WIDTH + 1)), 1.0f * (nCntpVtx / (FIELD_HEIGHT + 1)));
	//}

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffMeshWall->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_WALLINDEX,	//�K�v�ȃC���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshWall,
		NULL);

	//WORD *pIdx;	//�C���f�b�N�X���̃|�C���^

	////�C���f�b�N�X�o�b�t�@�����b�N�����_�ԍ��f�[�^�ւ̃|�C���^���擾
	//g_pIdexBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	//for (int nCntIdx = 0; nCntIdx < MAX_FIELD / 2; nCntIdx++)
	//{
	//	pIdx[nCntIdx * 2] = (nCntIdx + FIELD_WIDTH + 1) - (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 2)) - (nCntIdx / (FIELD_WIDTH + 2));
	//	pIdx[nCntIdx * 2 + 1] = nCntIdx + (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 1)) - (nCntIdx / (FIELD_WIDTH + 2));
	//}

	////�C���f�b�N�X�o�b�t�@���A�����b�N����
	//g_pIdexBuffMeshWall->Unlock();

	
	//�ǂ̐ݒ�
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -1800.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f), 1200.0f, 1800.0f, 1, 1);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, 1500.0f), D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f), 200.0f, 200.0f, 3, 3);
	SetMeshWall(D3DXVECTOR3(-1800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f), 100.0f, 300.0f,2, 6);
	SetMeshWall(D3DXVECTOR3(1800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f), 100.0f, 600.0f, 4, 3);

}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitMeshWall(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔p��
	if (g_pIdexBuffMeshWall != NULL)
	{
		g_pIdexBuffMeshWall->Release();
		g_pIdexBuffMeshWall = NULL;
	}

}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdateMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1�L�[�������ꂽ�Ƃ�
		
	}
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	int nCntIndex = 0;

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//�g�p����Ă���Ƃ�
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshWall[nCntWall].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntWall].rot.y, g_aMeshWall[nCntWall].rot.x, g_aMeshWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntWall].pos.x, g_aMeshWall[nCntWall].pos.y, g_aMeshWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntWall].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffMeshWall,
				0,
				sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdexBuffMeshWall);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshWall[nCntWall].nVertex,		//�p�ӂ������_��
				nCntIndex,
				g_aMeshWall[nCntWall].nIndex - 2	//�`�悷��v���~�e�B�u��
			);

			nCntIndex += g_aMeshWall[nCntWall].nIndex;
		}
	}
}

//==========================================================
//���b�V���E�H�[���̐ݒ�
//==========================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth)
{
	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aMeshWall[nCntWall].pos = pos;
			g_aMeshWall[nCntWall].rot = rot;
			g_aMeshWall[nCntWall].fHeight = fHeight;
			g_aMeshWall[nCntWall].fWidth = fWidth;
			g_aMeshWall[nCntWall].nHeightMesh = nHeight;
			g_aMeshWall[nCntWall].nWidthMesh = nWidth;
			g_aMeshWall[nCntWall].nVertex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh + 1);
			g_aMeshWall[nCntWall].bUse = true;
			g_aMeshWall[nCntWall].nIndex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh) * 2 + (g_aMeshWall[nCntWall].nHeightMesh - 1) * 2;

			//�C���f�b�N�X�̐ݒ�
			SetIndexMeshWall(nCntWall);

			//���_���W�̐ݒ�
			SetpVtxMeshWall(nCntWall);

			break;
		}
	}
}

//==========================================================
//���b�V���E�H�[���̃C���f�b�N�X�ԍ��ݒ�
//==========================================================
void SetIndexMeshWall(int nCntWall)
{
	WORD *pIdx;	//�C���f�b�N�X���̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N�����_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdexBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	pIdx += g_nIndex;

	for (int nCntIdx = 0; nCntIdx < g_aMeshWall[nCntWall].nIndex / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + g_aMeshWall[nCntWall].nWidthMesh + 1)
			- (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 2)) 
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx
			+ (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 1))
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));
		pIdx[nCntIdx * 2] += g_nVertex;
		pIdx[nCntIdx * 2 + 1] += g_nVertex;
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdexBuffMeshWall->Unlock();

	g_nIndex += g_aMeshWall[nCntWall].nIndex;
}

//==========================================================
//���b�V���E�H�[����pVtx�ݒ�
//==========================================================
void SetpVtxMeshWall(int nCntWall)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += g_nVertex;

	//���_���W�̐ݒ�(��������E��O�Ɍ������Ē��_����ݒ肷��
	for (int nCntpVtx = 0; nCntpVtx < g_aMeshWall[nCntWall].nVertex; nCntpVtx++)
	{
		//���_���W
		pVtx[nCntpVtx].pos.x = -(g_aMeshWall[nCntWall].fWidth * g_aMeshWall[nCntWall].nWidthMesh) + (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].fWidth * 2));
		pVtx[nCntpVtx].pos.y = ((g_aMeshWall[nCntWall].fHeight * 2) * g_aMeshWall[nCntWall].nHeightMesh) + ((nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (-g_aMeshWall[nCntWall].fHeight * 2)));
		pVtx[nCntpVtx].pos.z = 0.0f;

		//�@��
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//�F
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1)), 1.0f * (nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1)));
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshWall->Unlock();

	g_nVertex += g_aMeshWall[nCntWall].nVertex;
}

//==========================================================
//���b�V���E�H�[���̏��擾
//==========================================================
MeshWall *GetMeshWall(void)
{
	return &g_aMeshWall[0];
}
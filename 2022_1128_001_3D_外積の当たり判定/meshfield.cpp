//==========================================================
//
//���b�V���t�B�[���h�`�揈��
//Author ����ɐ�
//
//==========================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\field000.jpg"		//�e�N�X�`���f�[�^
#define MAX_FIELD		(28)	//�t�B�[���h�̒��_��
#define FIELD_WIDTH		(3)		//�t�B�[���h�̕�
#define FIELD_HEIGHT	(3)		//�t�B�[���h�̍���
#define FIELD_TYOUTEN	(4 * 4)	//���_��
#define FIELD_WIDTHWIDTH	(600.0f)	//������
#define FIELD_HEIGHTHEIGHT	(600.0f)	//���s����

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshField;								//�ʒu
D3DXVECTOR3 g_rotMeshField;								//����
D3DXMATRIX g_mtxWorldMeshField;							//���[���h�}�g���b�N�X

//==========================================================
//�|���S���̏���������
//==========================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureMeshField);

	//�e��ϐ��̏�����
	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * FIELD_TYOUTEN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;
	VERTEX_3D aVtx[FIELD_TYOUTEN];

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//���_���W�̐ݒ�(��������E��O�Ɍ������Ē��_����ݒ肷��
	for (int nCntpVtx = 0; nCntpVtx < FIELD_TYOUTEN; nCntpVtx++)
	{
		//���_���W
		pVtx[nCntpVtx].pos.x = -(FIELD_WIDTHWIDTH * FIELD_WIDTH) + (nCntpVtx % (FIELD_WIDTH + 1) * (FIELD_WIDTHWIDTH * 2));
		pVtx[nCntpVtx].pos.y = 0.0f;
		pVtx[nCntpVtx].pos.z = (FIELD_HEIGHTHEIGHT * FIELD_HEIGHT) + ((nCntpVtx / (FIELD_HEIGHT + 1) * (-FIELD_HEIGHTHEIGHT * 2)));

		//�@��
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//�F
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (FIELD_WIDTH + 1)), 1.0f * (nCntpVtx / (FIELD_HEIGHT + 1)));
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_FIELD,	//�K�v�ȃC���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshField,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N�����_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdexBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MAX_FIELD / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + FIELD_WIDTH + 1) - (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 2)) - (nCntIdx / (FIELD_WIDTH + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx + (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 1)) - (nCntIdx / (FIELD_WIDTH + 2));
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdexBuffMeshField->Unlock();
}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitMeshField(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔p��
	if (g_pIdexBuffMeshField != NULL)
	{
		g_pIdexBuffMeshField->Release();
		g_pIdexBuffMeshField = NULL;
	}

}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdateMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1�L�[�������ꂽ�Ƃ�
		
	}
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMeshField,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdexBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);


	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,		//�p�ӂ������_��
		0,
		26	//�`�悷��v���~�e�B�u��
	);
}
//==========================================================
//
//�e�`�揈��
//Author ����ɐ�
//
//==========================================================
#include "shadow.h"
#include "input.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\shadow000.jpg"		//�e�N�X�`���f�[�^
#define MAX_SHADOW	(64)		//�e�̍ő吔

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void pVtxShadow(void);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Shadow g_aShadow[MAX_SHADOW];			//�e�̏��

//==========================================================
//�e�̏���������
//==========================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureShadow);

	//�\���̂̊e�ϐ�������
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_aShadow[nCntShadow].bUse = false;		//�g�p����Ă��Ȃ���Ԃ�
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//�e��pVtx�ݒ�
	pVtxShadow();

}

//==========================================================
//�e�̏I������
//==========================================================
void UninitShadow(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

}

//==========================================================
//�e�̍X�V����
//==========================================================
void UpdateShadow(void)
{
	
}

//==========================================================
//�e�̕`�揈��
//==========================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffShadow,
				0,
				sizeof(VERTEX_3D));



			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntShadow * 4,
				2	//���_���\���̂̃T�C�Y
			);
		}

	}


	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================
//�e�̐ݒ菈��
//==========================================================
int SetShadow(void)
{
	int nSetShadow = -1;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aShadow[nCntShadow].bUse = true;	//�g�p����Ă����Ԃ�
			nSetShadow = nCntShadow;	//�ԍ����o����
			break;
		}
	}
	return nSetShadow;
}

//==========================================================
//�e�̈ʒu�ݒ菈��
//==========================================================
void SetPositionShadow(int nIdexShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdexShadow].pos.x = pos.x;
	g_aShadow[nIdexShadow].pos.z = pos.z;
}

//==========================================================
//�e��pVtx�ݒ菈��
//==========================================================
void pVtxShadow(void)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-10.0f, 0.0f, +10.0f);
		pVtx[1].pos = D3DXVECTOR3(+10.0f, 0.0f, +10.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
		pVtx[3].pos = D3DXVECTOR3(+10.0f, 0.0f, -10.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//==========================================================
//�e�̎g�p��ԃ��Z�b�g
//==========================================================
void ResetShadow(int nShadow)
{
	g_aShadow[nShadow].bUse = false;
}
//==========================================================
//
//�r���{�[�h�`�揈��
//Author ����ɐ�
//
//==========================================================
#include "billboard.h"
#include "input.h"
#include "camera.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\billboard.png"		//�e�N�X�`���f�[�^
#define MAX_BILLBOARD	(4)
#define BILLBOARD_MOVE	(1.0f)

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void pVtxBillboard(int nCntBillboard);
void MoveBillboard(void);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];					//�r���{�[�h�̏��

//==========================================================
//�r���{�[�h�̏���������
//==========================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureBillboard);

	//�\���̂̊e��񏉊���
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aBillboard[nCntBillboard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aBillboard[nCntBillboard].fWidth = 0.0f;						//��
		g_aBillboard[nCntBillboard].fHeight = 0.0f;						//����
		g_aBillboard[nCntBillboard].bUse = false;						//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//�r���{�[�h�̐ݒ�
	SetBillboard(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f, 50.0f);
	SetBillboard(D3DXVECTOR3(50.0f, 50.0f, 0.0f), 50.0f, 50.0f);

	//pVtx�̐ݒ�
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		pVtxBillboard(nCntBillboard);
	}
}

//==========================================================
//�r���{�[�h�̏I������
//==========================================================
void UninitBillboard(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//==========================================================
//�r���{�[�h�̍X�V����
//==========================================================
void UpdateBillboard(void)
{
	//�ړ�
	MoveBillboard();
}

//==========================================================
//�r���{�[�h�̈ړ�����
//==========================================================
void MoveBillboard(void)
{
	Camera *pCamera = GetCamera();

	if (GetKeyboardPress(DIK_J) == true)
	{//J�L�[�������ꂽ�Ƃ�
		if (GetKeyboardPress(DIK_I) == true)
		{//I�L�[�������ꂽ�Ƃ�
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (-D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (-D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//K�L�[�������ꂽ�Ƃ�
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (-D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (-D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
		}
		else
		{
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (-D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (-D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{
		if (GetKeyboardPress(DIK_I) == true)
		{//I�L�[�������ꂽ�Ƃ�
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//K�L�[�������ꂽ�Ƃ�
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
		}
		else
		{
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_I) == true)
	{//I�L�[�������ꂽ�Ƃ�
		g_aBillboard[0].pos.x += -cosf(pCamera->rot.x) * BILLBOARD_MOVE;
		g_aBillboard[0].pos.z += -sinf(pCamera->rot.x) * BILLBOARD_MOVE;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//K�L�[�������ꂽ�Ƃ�
		g_aBillboard[0].pos.x += cosf(pCamera->rot.x) * BILLBOARD_MOVE;
		g_aBillboard[0].pos.z += sinf(pCamera->rot.x) * BILLBOARD_MOVE;
	}

	g_aBillboard[0].pos.x += g_aBillboard[0].move.x;
	g_aBillboard[0].pos.z += g_aBillboard[0].move.z;

	g_aBillboard[0].move.x = 0.0f;
	g_aBillboard[0].move.z = 0.0f;
}

//==========================================================
//�r���{�[�h�̕`�揈��
//==========================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);


	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{//�g�p����Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBillboard[nCntBillboard].mtxWorld, NULL, &mtxView);
			g_aBillboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._43 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._21 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._23 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._24 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffBillboard,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBillboard * 4,
				2	//���_���\���̂̃T�C�Y
			);
		}
	}


	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//==========================================================
//�r���{�[�h�̐ݒ菈��
//==========================================================
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aBillboard[nCntBillboard].pos = pos;			//�ʒu
			g_aBillboard[nCntBillboard].fWidth = fWidth;	//��
			g_aBillboard[nCntBillboard].fHeight = fHeight;	//����
			g_aBillboard[nCntBillboard].bUse = true;		//�g�p���Ă����Ԃɂ���

			//pVtx�̐ݒ�
			pVtxBillboard(nCntBillboard);

			break;
		}
	}
}

//==========================================================
//pVtx�ݒ�
//==========================================================
void pVtxBillboard(int nCntBillboard)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += nCntBillboard * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth, +g_aBillboard[nCntBillboard].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aBillboard[nCntBillboard].fWidth, +g_aBillboard[nCntBillboard].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth, -g_aBillboard[nCntBillboard].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aBillboard[nCntBillboard].fWidth, -g_aBillboard[nCntBillboard].fHeight, 0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}
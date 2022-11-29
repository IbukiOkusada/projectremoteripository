//==========================================================
//
//�Ǖ`�揈��
//Author ����ɐ�
//
//==========================================================
#include "main.h"
#include "wall.h"
#include "input.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\block000.jpg"		//�e�N�X�`���f�[�^

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void pVtxWall(int nCntWall);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posWall;								//�ʒu
D3DXVECTOR3 g_rotWall;								//����
D3DXMATRIX g_mtxWorldWall;							//���[���h�}�g���b�N�X
Wall g_aWall[MAX_WALL];								//�ǂ̏��
int nSelectWall = 0;

//==========================================================
//�ǂ̏���������
//==========================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureWall);

	//�e��ϐ��̏�����
	g_posWall = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	g_rotWall = D3DXVECTOR3(0.0f, 1.57f, 0.0f);

	//�\���̂̊e��񏉊���
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_aWall[nCntWall].fWidth = 0.0f;						//��
		g_aWall[nCntWall].fHeight = 0.0f;						//����
		g_aWall[nCntWall].bUse = false;							//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//�ǂ̐ݒ�
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.3f * D3DX_PI, 0.0f), 150.0f, 100.0f);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f), 150.0f, 100.0f);
	SetWall(D3DXVECTOR3(-650.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.5f * -D3DX_PI, 0.0f), 150.0f, 100.0f);
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f), 150.0f, 100.0f);

	//pVtx�̐ݒ�
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		pVtxWall(nCntWall);
	}
}

//==========================================================
//�ǂ̏I������
//==========================================================
void UninitWall(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//==========================================================
//�ǂ̍X�V����
//==========================================================
void UpdateWall(void)
{
	/*if (GetKeyboardPress(DIK_0))
	{
		g_aWall[nSelectWall].rot.y += 0.01f;

		if (g_aWall[nSelectWall].rot.y > D3DX_PI)
		{
			g_aWall[nSelectWall].rot.y *= -1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_9))
	{
		g_aWall[nSelectWall].rot.y -= 0.01f;

		if (g_aWall[nSelectWall].rot.y < -D3DX_PI)
		{
			g_aWall[nSelectWall].rot.y *= -1.0f;
		}
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		nSelectWall = (nSelectWall + 1) % MAX_WALL;
	}*/
}

//==========================================================
//�ǂ̕`�揈��
//==========================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{//�g�p����Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffWall,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntWall * 4,
				2	//���_���\���̂̃T�C�Y
			);
		}
	}
}

//==========================================================
//�ǂ̐ݒ菈��
//==========================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aWall[nCntWall].pos = pos;			//�ʒu
			g_aWall[nCntWall].rot = rot;			//����
			g_aWall[nCntWall].fWidth = fWidth;		//��
			g_aWall[nCntWall].fHeight = fHeight;	//����
			g_aWall[nCntWall].bUse = true;			//�g�p���Ă����Ԃɂ���

			//pVtx�̐ݒ�
			pVtxWall(nCntWall);

			break;
		}
	}
}

//==========================================================
//pVtx�ݒ�
//==========================================================
void pVtxWall(int nCntWall)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += nCntWall * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, 0.0f, 0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���_�J���[�̐ݒ�
	if (nCntWall == 4)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//�ǂ̏����擾
Wall *GetWall(void)
{
	return &g_aWall[0];
}
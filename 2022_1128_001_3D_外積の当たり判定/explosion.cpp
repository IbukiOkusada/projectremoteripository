//===================================================
//
//��������
//Author ����ɐ�
//
//===================================================
#include "explosion.h"

//===================================================
//�}�N����`
//===================================================
#define MYEXPLOSION_TEXTURE	"data\\TEXTURE\\explosion000.png"	//�����̃e�N�X�`����

//===================================================
//�}�N����`
//===================================================
void pVtxExplosion(int nCntExplosion);

//===================================================
//�O���[�o���ϐ�
//===================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];				//�����̏��

//===================================================
//�����̏���������
//===================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MYEXPLOSION_TEXTURE, &g_pTextureExplosion);

	//�����̏��̏�����
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
		g_aExplosion[nCntExplosion].fRadian = 0.0f;	//�J�E���g��������
		g_aExplosion[nCntExplosion].nCounterAnim = 0;	//�J�E���g��������
		g_aExplosion[nCntExplosion].nPatternAnim = 0;	//�p�^�[��No.��������
	}

	//���_�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	//pVtx�̐ݒ�


}

//===================================================
//�����̏I������
//===================================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//===================================================
//�����̍X�V����
//===================================================
void UpdateExplosion(void)
{
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă���ꍇ
			g_aExplosion[nCntExplosion].nCounterAnim++;	//�A�j���[�V�����̃J�E���^�[��i�߂�

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 5) == 0)
			{
				g_aExplosion[nCntExplosion].nPatternAnim++;	//�p�^�[��No.���X�V

				//pVtx�̐ݒ�
				pVtxExplosion(nCntExplosion);

				if (g_aExplosion[nCntExplosion].nPatternAnim >= 8)
				{//�p�^�[�����𒴂����ꍇ
					g_aExplosion[nCntExplosion].bUse = false;	//�g�p����Ă��Ȃ���Ԃɂ���
				}
			}
		}
	}
}

//===================================================
//�����̕`�揈��
//===================================================
void DrawExplosion(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	if (GetZtest() == 0)
	{
		//Z�e�X�g�𖳌�������
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă���ꍇ

		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExplosion[nCntExplosion].mtxWorld, NULL, &mtxView);
			g_aExplosion[nCntExplosion].mtxWorld._41 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._42 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._43 = 0.0f;
			/*g_aExplosion[nCntExplosion].mtxWorld._31 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._32 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._33 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._34 = 0.0f;*/

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffExplosion,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntExplosion * 4,
				2	//���_���\���̂̃T�C�Y
			);
		}
	}

	if (GetZtest() == 0)
	{
		//Z�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//===================================================
//�����̐ݒ菈��
//===================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadian)
{
	int nCntExplosion;

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�������g�p����Ă��Ȃ��ꍇ
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].fRadian = fRadian;
			g_aExplosion[nCntExplosion].bUse = true;		//�g�p���Ă����Ԃɂ���

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;

			//pVtx�̐ݒ�
			pVtxExplosion(nCntExplosion);

			break;

		}
	}
}

//===================================================
//������pVtx�ݒ�
//===================================================
void pVtxExplosion(int nCntExplosion)
{
	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntExplosion * 4;	//�|�C���^���J�E���g�܂Ői�߂�

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fRadian, +g_aExplosion[nCntExplosion].fRadian, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aExplosion[nCntExplosion].fRadian, +g_aExplosion[nCntExplosion].fRadian, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fRadian, -g_aExplosion[nCntExplosion].fRadian, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aExplosion[nCntExplosion].fRadian, -g_aExplosion[nCntExplosion].fRadian, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim + 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim + 0.125f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}


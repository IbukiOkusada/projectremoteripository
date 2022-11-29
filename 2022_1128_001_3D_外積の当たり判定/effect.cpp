//===================================================
//
//�G�t�F�N�g����
//Author ����ɐ�
//
//===================================================
#include "main.h"
#include "effect.h"
#include <stdlib.h>
#include <time.h>

//===================================================
//�}�N����`
//===================================================
#define MAX_EFFECT	(20048)							//�G�t�F�N�g�̍ő吔
#define MYEFFECT	"data\\TEXTURE\\effect000.jpg"	//�G�t�F�N�g�̃e�N�X�`����

//===================================================
//�\���̂̒�`
//===================================================
//�G�t�F�N�g
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fRadius;		//���a
	int nLife;			//����
	int type;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Effect;

//===================================================
//�񋓌^�̒�`
//===================================================
//�T�C�Y�����_��
typedef enum
{
	EFFECTSIZE_NORMAL = 0,	//������
	EFFECTSIZE_SMALL,	//����ɏ�����
	EFFECTSIZE_BIG,		//�傫��
	EFFECTSIZE_MAX
}EFFECTSIZE;

//===================================================
//�O���[�o���ϐ�
//===================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];						//�G�t�F�N�g�̏��
int g_EffectCnt;									//���ݎg�p����Ă���G�t�F�N�g�̃J�E���g

//===================================================
//�v���g�^�C�v�錾
//===================================================
void SetChangeEffect(Effect *pEffect);
void SetMoveEffect(Effect *pEffect);
void SetJumpEffect(Effect *pEffect);
void pVtxEffect(int nCntEffect);

//===================================================
//�G�t�F�N�g�̏���������
//===================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MYEFFECT, &g_pTextureEffect);

	//�G�t�F�N�g�̏��̏�����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
		g_aEffect[nCntEffect].type = EFFECTTYPE_MOVE;
	}

	g_EffectCnt = 0;	//�J�E���g������

	//���_�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//�����̎�̏�����
	srand((unsigned)time(0));

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//pVtx�̐ݒ�
		pVtxEffect(nCntEffect);
	}

}

//===================================================
//�G�t�F�N�g�̏I������
//===================================================
void UninitEffect(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//===================================================
//�G�t�F�N�g�̍X�V����
//===================================================
void UpdateEffect(void)
{

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���ꍇ

			switch (g_aEffect[nCntEffect].type)
			{
			case EFFECTTYPE_CHANGE:	//�d�͕ω����
				SetChangeEffect(&g_aEffect[nCntEffect]);
				break;
			case EFFECTTYPE_MOVE:	//�ړ�
				SetMoveEffect(&g_aEffect[nCntEffect]);
				break;
			case EFFECTTYPE_JUMP:	//�W�����v
				SetJumpEffect(&g_aEffect[nCntEffect]);
				break;
			case 0:
				g_aEffect[nCntEffect].fRadius *= 0.95f;
				g_aEffect[nCntEffect].col.a *= 0.99f;
				break;
			}

			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	//���W�Ɉړ��ʂ����Z

			//�G�t�F�N�g�̎��������炷
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{//�����������Ȃ����ꍇ
				g_aEffect[nCntEffect].bUse = false;	//�g�p����Ă��Ȃ���Ԃ�
			}

			//pVtx�̐ݒ�
			pVtxEffect(nCntEffect);
		}
	}
}

//===================================================
//�G�t�F�N�g�̕`�揈��
//===================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	//���C�e�B���O���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�𖳌�������
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���ꍇ

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);
			g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

		 //���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffEffect,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntEffect * 4,
				2	//���_���\���̂̃T�C�Y
			);
		}
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	//���C�e�B���O���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===================================================
//�G�t�F�N�g�̐ݒ菈��
//===================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ��ꍇ
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].type = type;
			g_aEffect[nCntEffect].bUse = true;		//�g�p���Ă����Ԃɂ���
			g_EffectCnt++;
			break;
		}
	}
}

//===================================================
//�G�t�F�N�g��pVtx�ݒ�
//===================================================
void pVtxEffect(int nCntEffect)
{
	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntEffect * 4;	//�|�C���^���J�E���g�܂Ői�߂�

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_aEffect[nCntEffect].col;
	pVtx[1].col = g_aEffect[nCntEffect].col;
	pVtx[2].col = g_aEffect[nCntEffect].col;
	pVtx[3].col = g_aEffect[nCntEffect].col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//===================================================
//�G�t�F�N�g�̐��擾
//===================================================
int GetEffect(void)
{
	return g_EffectCnt;
}

//===================================================
//
//�G�t�F�N�g�̊e�ݒ�
//
//===================================================
//�d�͕ω�
//===================================================
void SetChangeEffect(Effect *pEffect)
{
	pEffect->col.a *= 0.9f;
}

//===================================================
//�ړ�
//===================================================
void SetMoveEffect(Effect *pEffect)
{
	pEffect->col.a *= 0.9f;
}

//===================================================
//�W�����v
//===================================================
void SetJumpEffect(Effect *pEffect)
{
	pEffect->col.a *= 0.8f;
}
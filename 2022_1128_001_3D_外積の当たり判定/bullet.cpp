//===================================================
//
//�e�̔��ˏ���
//Author ����ɐ�
//
//===================================================
#include "bullet.h"
#include "shadow.h"
#include "wall.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "enemy.h"
#include "model.h"
#include "meshwall.h"

//===================================================
//�}�N����`
//===================================================
#define MYBULLET_TEXTURE	"data\\TEXTURE\\bullet000.png"	//�e�̃e�N�X�`����

//===================================================
//�v���g�^�C�v�錾
//===================================================
void pVtxBullet(int nCntBullet);
void CollisionWall(int nCntBullet);
void CollisionEnemy(int nCntBullet);

//===================================================
//�O���[�o���ϐ�
//===================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];		//�e�̏��

//===================================================
//�e�̏���������
//===================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MYBULLET_TEXTURE, &g_pTextureBullet);

	//�e�̏��̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
		g_aBullet[nCntBullet].fHeight = 0.0f;
		g_aBullet[nCntBullet].fWidth = 0.0f;
		g_aBullet[nCntBullet].nShadow = -1;
	}

	//���_�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//pVtx�̐ݒ�
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		pVtxBullet(nCntBullet);
	}

}

//===================================================
//�e�̏I������
//===================================================
void UninitBullet(void)
{
	//�e�N�X�`���̔p��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===================================================
//�e�̍X�V����
//===================================================
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���ꍇ
			//�O��̈ʒu���擾
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//�ʒu�̍X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//�e�̍X�V
			SetPositionShadow(g_aBullet[nCntBullet].nShadow, g_aBullet[nCntBullet].pos);

			//�ǂƂ̓����蔻��
			CollisionWall(nCntBullet);

			//�G�Ƃ̓����蔻��
			CollisionEnemy(nCntBullet);

			//�G�t�F�N�g�̐ݒ�
			SetEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move * 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].fWidth, 10, 0);

		}
	}
}

//===================================================
//�e�̕`�揈��
//===================================================
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	//���C�e�B���O���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (GetAlphaTest() == 1)
	{
		//�A���t�@�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	}

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���ꍇ

		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffBullet,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBullet * 4,
				2	//���_���\���̂̃T�C�Y
			);
		}
	}

	if (GetAlphaTest() == 1)
	{
		//�A���t�@�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	}

	//���C�e�B���O���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===================================================
//�e�̐ݒ菈��
//===================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fWidth)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].fWidth = fWidth;
			g_aBullet[nCntBullet].fHeight = fHeight;

			g_aBullet[nCntBullet].bUse = true;		//�g�p���Ă����Ԃɂ���

			pVtxBullet(nCntBullet);

			g_aBullet[nCntBullet].nShadow = SetShadow();

			break;
		}

	}
}

//===================================================
//�e���擾����
//===================================================
int GetTrueBullet(void)
{
	int nCntBullet;
	int nBulletCounter = 0;		//���ݕ\������Ă���e�����o����

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���ꍇ
			nBulletCounter++;		//�J�E���g���v���X����
		}
	}

	return nBulletCounter;
}

//===================================================
//�e��pVtx�ݒ�
//===================================================
void pVtxBullet(int nCntBullet)
{
	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntBullet * 4;	//�|�C���^���J�E���g�܂Ői�߂�

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, +g_aBullet[nCntBullet].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].fWidth, +g_aBullet[nCntBullet].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, -g_aBullet[nCntBullet].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].fWidth, -g_aBullet[nCntBullet].fHeight, 0.0f);

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

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//===================================================
//�e�ƕǂ̓����蔻��
//===================================================
void CollisionWall(int nCntBullet)
{
	Wall *pWall = GetWall();
	D3DXVECTOR3 vecLine, vecToPos;	//����p�ϐ�

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
				vecLine = D3DXVECTOR3((pWall->pos.x + cosf(pWall->rot.y) * (-pWall->fWidth)) - (pWall->pos.x + cosf(pWall->rot.y) * (pWall->fWidth)),
					0.0f,
					(pWall->pos.z + sinf(pWall->rot.y) * (pWall->fWidth)) - (pWall->pos.z + sinf(pWall->rot.y) * (-pWall->fWidth)));

				vecToPos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (pWall->pos.x + cosf(pWall->rot.y) * (pWall->fWidth)),
					0.0f,
					g_aBullet[nCntBullet].pos.z - (pWall->pos.z + sinf(pWall->rot.y) * (-pWall->fWidth)));

				if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) > 0.0f)
				{
					g_aBullet[nCntBullet].bUse = false;
					ResetShadow(g_aBullet[nCntBullet].nShadow);	//�g�p���Ă����e���g�p����Ă��Ȃ���Ԃɂ���
					SetParticle(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_MOVE, 5.0f);
				}
		}
	}
}

//===================================================
//�G�ƒe�̓����蔻��
//===================================================
void CollisionEnemy(int nCntBullet)
{
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�G���g�p����Ă���ꍇ
			if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth >= pEnemy->pos.x + pEnemy->vtxMin.x &&
				g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth <= pEnemy->pos.x + pEnemy->vtxMax.x &&
				g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fWidth >= pEnemy->pos.z + pEnemy->vtxMin.z &&
				g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fWidth <= pEnemy->pos.z + pEnemy->vtxMax.z)
			{//�G�͈͓̔��ɓ������ꍇ
				HitEnemy(1, pEnemy);
				g_aBullet[nCntBullet].bUse = false;
				ResetShadow(g_aBullet[nCntBullet].nShadow);	//�g�p���Ă����e���g�p����Ă��Ȃ���Ԃɂ���
				SetParticle(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_MOVE, 5.0f);
			}
		}
	}
}

//===================================================
//�e�̏����擾
//===================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}

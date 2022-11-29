//==========================================================
//
//���f���`�揈��
//Author ����ɐ�
//
//==========================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include <stdio.h>

//==========================================================
//�}�N����`
//==========================================================
#define PLAYERFILE_DATA	"data\\MODEL\\car004.x"		//���f���f�[�^
#define PLAYER_MOVESPEED	(3.0f)	//�ړ���
#define ATR_SPEED			(0.3f)	//�ړ����x����
#define PLAYER_BULLETSPEED	(5.0f)	//�e�̑��x
#define YES_NUMPLAYERPARTS	"NUM_PARTS"	//�p�[�c���ǂݍ��݊m�F����
#define YES_SETPLAYERPARTS	"SET_PLAYERSET"	//�z�u���ǂݍ��݊m�F����
#define END_SETPLAYERPARTS	"END_PLAYERSET"	//�z�u���ǂݍ��݊m�F����

#define YES_NUMPLAYERKEY	"NUM_KEY"		//���ǂݍ��݊m�F����
#define YES_NUMPLAYERLOOP	"LOOP"			//�p�[�c���ǂݍ��݊m�F����
#define YES_SETPLAYERMOTION	"MOTIONSET"		//���[�V�������ǂݍ��݊m�F����
#define END_SETPLAYERMOTION	"END_MOTIONSET"	//���[�V�������ǂݍ��݊m�F����

#define YES_SETPLAYERKEYSET	"KEYSET"		//�L�[���ǂݍ��݊m�F����
#define END_SETPLAYERKEYSET	"END_KEYSET"	//�L�[���ǂݍ��݊m�F����

#define YES_SETPLAYERKEY	"KEY"		//�L�[�ǂݍ��݊m�F����
#define END_SETPLAYERKEY	"END_KEY"	//�L�[�ǂݍ��݊m�F����

#define SET_PLAYERPARTS	(17)			//�z�u�p�[�c��

#define MIN_MOVEMOTION	(0.4f)			//�ړ�����ҋ@�ɑJ�ڂ���܂ł̍Œ�ړ���
#define MAX_WALL		(1600.0f)

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void MovePlayer(void);
void CollisionPlayer(void);
void LoadPlayerData(void);
void LoadPlayerMotionData(void);
void MotionSetData(void);
void UpdateMotionPlayer(void);
void MotionSet(PLAYERMOTION motion);
void SetPlayerPartsPos(void);
void SetMoveMotion(void);
void SetNeutralMotion(void);

//==========================================================
//���f���t�@�C����
//==========================================================
const char *c_apModelnamePlayer[SET_PLAYERPARTS] =
{
	"data\\MODEL\\sample\\waist.x",
	"data\\MODEL\\sample\\body.x",
	"data\\MODEL\\sample\\head.x",
	"data\\MODEL\\sample\\rightshoulder.x",
	"data\\MODEL\\sample\\rightupperarm.x",
	"data\\MODEL\\sample\\rightforearm.x",
	"data\\MODEL\\sample\\righthand.x",
	"data\\MODEL\\sample\\leftshoulder.x",
	"data\\MODEL\\sample\\leftupperarm.x",
	"data\\MODEL\\sample\\leftforearm.x",
	"data\\MODEL\\sample\\lefthand.x",
	"data\\MODEL\\sample\\righthigh.x",
	"data\\MODEL\\sample\\rightcalf.x",
	"data\\MODEL\\sample\\rightleg.x",
	"data\\MODEL\\sample\\lefthigh.x",
	"data\\MODEL\\sample\\leftcalf.x",
	"data\\MODEL\\sample\\leftleg.x",
};

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
Player g_Player;	//�v���C���[���
PlayerParts g_aPlayerParts[MAX_PLAYERPARTS];	//�v���C���[�p�[�c�̏��
int g_nPlayerPartsCount;
int g_nPlayerCameraCount;

//==========================================================
//���f���̏���������
//==========================================================
void InitPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�̃|�C���^

	//�e��ϐ��̏�����
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	g_Player.posOld = g_Player.pos;
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.movespeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.fJumpPower = 10.0f;
	g_Player.nShadow = -1;
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nPlayerPartsCount = 0; 
	g_Player.MotionData.nNowFrame = 0;
	g_Player.MotionData.PatternKey = 0;
	g_Player.MotionData.nMotionNum = 0;
	g_Player.bMove = false;		//�ړ����Ă��Ȃ���Ԃɂ���
	g_Player.fRotMove = g_Player.rot.y;
	g_Player.fRotDiff = g_Player.rot.y;
	g_Player.fRotDest = g_Player.rot.y;

	for (int nCntPlayerMotion = 0; nCntPlayerMotion < MAX_PLAYERMOTION; nCntPlayerMotion++)
	{
		g_Player.MotionData.aNumKey[nCntPlayerMotion] = 0;
		g_Player.MotionData.aLoop[nCntPlayerMotion] = 0;

			for (int nCntPlayerKey = 0; nCntPlayerKey < MAX_PLAYERKEY; nCntPlayerKey++)
			{
				g_Player.MotionData.aNumFrame[nCntPlayerMotion][nCntPlayerKey] = 0;
			}
	}


	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayerParts[nCntPlayerParts].nParent = -1;
		g_aPlayerParts[nCntPlayerParts].nPartNum = 0;
		g_aPlayerParts[nCntPlayerParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntPlayerKey = 0; nCntPlayerKey < MAX_PLAYERKEY; nCntPlayerKey++)
		{
			for (int nCntPlayerMotion = 0; nCntPlayerMotion < MAX_PLAYERMOTION; nCntPlayerMotion++)
			{
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].aKeyData[nCntPlayerMotion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].aKeyData[nCntPlayerMotion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].rotDiff[nCntPlayerMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].posDiff[nCntPlayerMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	//�t�@�C���̓ǂݍ���
	LoadPlayerData();

	//�v���C���[�̃��[�V�������ǂݍ���
	LoadPlayerMotionData();

	//���[�V�����̏��ݒ�
	MotionSetData();

	//���[�V�����̏����ݒ�
	MotionSet(PLAYERMOTION_NEUTRAL);

	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].Setpos = g_aPlayerParts[nCntPlayerParts].pos;
	}

	//�p�[�c�ʒu�Z�b�g
	SetPlayerPartsPos();

	for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(c_apModelnamePlayer[nCntPlayerParts],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aPlayerParts[nCntPlayerParts].pBuffMat,
			NULL,
			&g_aPlayerParts[nCntPlayerParts].dwNumMat,
			&g_aPlayerParts[nCntPlayerParts].pMesh);

		D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aPlayerParts[nCntPlayerParts].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayerParts[nCntPlayerParts].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C�������݂��Ă���
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aPlayerParts[nCntPlayerParts].apTexture[nCntMat]);
			}
		}

	}

	//���_�����擾
	nNumVtx = g_aPlayerParts[0].pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_aPlayerParts[0].pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	g_aPlayerParts[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		if (vtx.x < g_Player.vtxMin.x)
		{//�ŏ��l�����l���������ꍇ
			g_Player.vtxMin.x = vtx.x;
		}
		else if (vtx.x > g_Player.vtxMax.x)
		{//�ő�l�����l���傫���ꍇ
			g_Player.vtxMax.x = vtx.x;
		}

		if (vtx.z < g_Player.vtxMin.z)
		{//�ŏ��l�����l���������ꍇ
			g_Player.vtxMin.z = vtx.z;
		}
		else if (vtx.z > g_Player.vtxMax.z)
		{//�ő�l�����l���傫���ꍇ
			g_Player.vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_aPlayerParts[0].pMesh->UnlockVertexBuffer();

	g_nPlayerCameraCount = 60;

	//�e�̐ݒ�
	g_Player.nShadow = SetShadow();
}

//==========================================================
//���f���̏I������
//==========================================================
void UninitPlayer(void)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		//���b�V���̔p��
		if (g_aPlayerParts[nCntPlayerParts].pMesh != NULL)
		{
			g_aPlayerParts[nCntPlayerParts].pMesh->Release();
			g_aPlayerParts[nCntPlayerParts].pMesh = NULL;
		}

		//�}�e���A���̔p��
		if (g_aPlayerParts[nCntPlayerParts].pBuffMat != NULL)
		{
			g_aPlayerParts[nCntPlayerParts].pBuffMat->Release();
			g_aPlayerParts[nCntPlayerParts].pBuffMat = NULL;
		}
	}

}

//==========================================================
//���f���̍X�V����
//==========================================================
void UpdatePlayer(void)
{
	//�O��̍��W���擾
	g_Player.posOld = g_Player.pos;

	//�ړ�����
	MovePlayer();

	//���[�V�����X�V
	UpdateMotionPlayer();
}

//==========================================================
//���f���̃��[�V�����X�V����
//==========================================================
void UpdateMotionPlayer(void)
{
	g_Player.MotionData.nNowFrame++;

	if (g_Player.MotionData.nNowFrame >= g_Player.MotionData.aNumFrame[g_Player.MotionData.nMotionNum][g_Player.MotionData.PatternKey])
	{//�t���[���������݂̃L�[�̃t���[�����𒴂����ꍇ
		g_Player.MotionData.nNowFrame = 0;	//�t���[���J�E���g���Z�b�g
		g_Player.MotionData.PatternKey = (g_Player.MotionData.PatternKey + 1) % g_Player.MotionData.aNumKey[g_Player.MotionData.nMotionNum];	//�L�[�����ɕύX
	}
	else
	{//�p����
		for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
		{
			g_aPlayerParts[nCntPlayerParts].rot += g_aPlayerParts[nCntPlayerParts].aKey[g_Player.MotionData.nMotionNum].rotDiff[g_Player.MotionData.PatternKey];
		}
	}
}

//==========================================================
//���f���̕`�揈��
//==========================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntPlayerParts = -1; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{

		switch (nCntPlayerParts)
		{
		case -1:	//���z�{��

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Player.mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
			D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
			D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);
			break;
		default:	//�p�[�c

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayerParts[nCntPlayerParts].mtxWorld);

			if (g_aPlayerParts[nCntPlayerParts].nParent == -1)
			{

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aPlayerParts[nCntPlayerParts].pos.x, g_aPlayerParts[nCntPlayerParts].pos.y, g_aPlayerParts[nCntPlayerParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayerParts[nCntPlayerParts].mtxWorld, &g_Player.mtxWorld, &mtxTrans);
			}
			else
			{

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aPlayerParts[nCntPlayerParts].pos.x, g_aPlayerParts[nCntPlayerParts].pos.y, g_aPlayerParts[nCntPlayerParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayerParts[nCntPlayerParts].mtxWorld, &mtxTrans, &g_aPlayerParts[g_aPlayerParts[nCntPlayerParts].nParent].mtxWorld);

			}

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerParts[nCntPlayerParts].rot.y, g_aPlayerParts[nCntPlayerParts].rot.x, g_aPlayerParts[nCntPlayerParts].rot.z);
			D3DXMatrixMultiply(&g_aPlayerParts[nCntPlayerParts].mtxWorld, &mtxRot, &g_aPlayerParts[nCntPlayerParts].mtxWorld);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerParts[nCntPlayerParts].mtxWorld);
			break;
		}

		if (nCntPlayerParts != -1)
		{
			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayerParts[nCntPlayerParts].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aPlayerParts[nCntPlayerParts].dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aPlayerParts[nCntPlayerParts].apTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aPlayerParts[nCntPlayerParts].pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==========================================================
//�v���C���[�̈ړ�����
//==========================================================
void MovePlayer(void)
{
	Camera *pCamera = GetCamera();

	g_Player.fRotMove = g_Player.rot.y;	//���݂̌������擾

	//�v���C���[�̍X�V
	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		if (GetKeyboardPress(DIK_UP) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = (-pCamera->rot.y + -D3DX_PI * 0.25f);
		}
		else
		{
			g_Player.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = -pCamera->rot.y;
		}

		//�ړ��ݒ�
		SetMoveMotion();
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{

		if (GetKeyboardPress(DIK_UP) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			
			g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			
			g_Player.fRotDest = (-pCamera->rot.y + -D3DX_PI * 0.75f);
		}
		else
		{
			g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 1.0f);
		}

		//�ړ��ݒ�
		SetMoveMotion();
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{
		g_Player.move.x += -cosf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.move.z += -sinf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 0.5f);

		//�ړ��ݒ�
		SetMoveMotion();

	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_Player.move.x += cosf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.move.z += sinf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.fRotDest = (-pCamera->rot.y + -D3DX_PI * 0.5f);

		//�ړ��ݒ�
		SetMoveMotion();
	}
	else
	{//�ړ����Ă��Ȃ��Ƃ�
		//�ҋ@�ݒ�
		SetNeutralMotion();
	}

	//�e�̔���
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//�X�y�[�X�L�[�������ꂽ�Ƃ�
		SetBullet(D3DXVECTOR3(g_Player.pos.x + cosf(-g_Player.rot.y + -D3DX_PI * 0.5f) * 20.0f, g_Player.pos.y + 30.0f, g_Player.pos.z + sinf(-g_Player.rot.y + -D3DX_PI * 0.5f) * 20.0f),
			D3DXVECTOR3(cosf(-g_Player.rot.y + -D3DX_PI * 0.5f) * PLAYER_BULLETSPEED, 0.0f, sinf(-g_Player.rot.y + -D3DX_PI * 0.5f) * PLAYER_BULLETSPEED),
			10.0f, 10.0f);
	}

	//����
	g_Player.move.x += (0.0f - g_Player.move.x) * ATR_SPEED;	//x���W
	g_Player.move.z += (0.0f - g_Player.move.z) * ATR_SPEED;	//y���W

	//���W�̕ύX
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.z += g_Player.move.z;

	if (g_Player.fRotDest > D3DX_PI || g_Player.fRotDest < -D3DX_PI)
	{//-3.14�`3.14�͈̔͊O�̏ꍇ
		if (g_Player.fRotDest > D3DX_PI)
		{
			g_Player.fRotDest += (-D3DX_PI * 2);
		}
		else if (g_Player.fRotDest < -D3DX_PI)
		{
			g_Player.fRotDest += (D3DX_PI * 2);
		}
	}

	g_Player.fRotDiff = g_Player.fRotDest - g_Player.fRotMove;	//�ڕW�܂ł̈ړ������̍���

	if (g_Player.fRotDiff > D3DX_PI || g_Player.fRotDiff < -D3DX_PI)
	{//-3.14�`3.14�͈̔͊O�̏ꍇ
		if (g_Player.fRotDiff > D3DX_PI)
		{
			g_Player.fRotDiff += (-D3DX_PI * 2);
		}
		else if (g_Player.fRotDiff < -D3DX_PI)
		{
			g_Player.fRotDiff += (D3DX_PI * 2);
		}
	}

	g_Player.rot.y += g_Player.fRotDiff * 0.08f;

	if (g_Player.rot.y > D3DX_PI || g_Player.rot.y < -D3DX_PI)
	{//-3.14�`3.14�͈̔͊O�̏ꍇ
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y += (-D3DX_PI * 2);
		}
		else if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += (D3DX_PI * 2);
		}
	}

	//�ǂƂ̔���
	CollisionPlayer();


	if (g_Player.move.x <= 0.1f && g_Player.move.x >= 0.0f ||
		g_Player.move.x >= -0.1f && g_Player.move.x <= 0.0f)
	{//x���W�̈ړ��ʂ��Ȃ��ꍇ
		if (g_Player.move.z <= 0.1f && g_Player.move.z >= 0.0f ||
			g_Player.move.z >= -0.1f && g_Player.move.z <= 0.0f)
		{//z���W�̈ړ��ʂ��Ȃ��ꍇ
			if (g_nPlayerCameraCount <= 0)
			{
				SetRotCamera();
			}
			else
			{
				g_nPlayerCameraCount--;
			}
		}
		else
		{
			g_nPlayerCameraCount = 60;
		}
	}
	else
	{
		g_nPlayerCameraCount = 60;
	}

	//�e�̈ʒu�ݒ�
	SetPositionShadow(g_Player.nShadow, g_Player.pos);


	
}

//==========================================================
//�v���C���[�̕ǂƂ̔���
//==========================================================
void CollisionPlayer(void)
{
	if (g_Player.pos.x > MAX_WALL)
	{
		g_Player.pos.x = MAX_WALL;
		g_Player.move.x = 0.0f;
	}
	if (g_Player.pos.x < -MAX_WALL)
	{
		g_Player.pos.x = -MAX_WALL;
		g_Player.move.x = 0.0f;
	}

	if (g_Player.pos.z > MAX_WALL)
	{
		g_Player.pos.z = MAX_WALL;
		g_Player.move.z = 0.0f;
	}
	if (g_Player.pos.z < -MAX_WALL)
	{
		g_Player.pos.z = -MAX_WALL;
		g_Player.move.z = 0.0f;
	}
}

//==========================================================
//�v���C���[�̏��擾
//==========================================================
Player *GetPlayer(void)
{
	return &g_Player;
}

//==========================================================
//�v���C���[�̃f�[�^�ǂݍ��ݏ���
//==========================================================
void LoadPlayerData(void)
{
	FILE *pFile;

	pFile = fopen("data\\TXT\\model.txt", "r");

	if (pFile != NULL)
	{//�J�����ꍇ

		char aNumStr[32] = {};	//�z�u�������i�[�p

		//�z�u���ǂݍ���
		fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(NUM_BLOCK)

		if (strcmp(&aNumStr[0], YES_NUMPLAYERPARTS) == 0)
		{//�z�u���m�F�̕������������ꍇ
			fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(=)
			fscanf(pFile, "%d", &g_nPlayerPartsCount);	//�z�u���擾

													//�z�u�����ǂݍ���
			for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
			{
				char aStartStr[40] = {};	//�J�n�̕���

				fscanf(pFile, "%s", &aStartStr[0]);
				if (strcmp(&aStartStr[0], YES_SETPLAYERPARTS) == 0)
				{//�J�n�������������ꍇ
					char aString[128] = {};
					char aEndStr[40] = {};	//�I���̕���

					//fscanf(pFile, "%s", &aString[0]);		//�R�����g(���ڂ�)

					//�p�[�c�ԍ����擾
					fscanf(pFile, "%s", &aString[0]);		//�R�����g(INDEX)
					fscanf(pFile, "%s", &aString[0]);		//�R�����g(=)
					fscanf(pFile, "%d", &g_aPlayerParts[nCntPlayerParts].nPartNum);		//�p�[�c�ԍ�

					//�e�ԍ����擾
					fscanf(pFile, "%s", &aString[0]);		//�R�����g(PARENT)
					fscanf(pFile, "%s", &aString[0]);		//�R�����g(=)
					fscanf(pFile, "%d", &g_aPlayerParts[nCntPlayerParts].nParent);		//�e

					//���W���擾
					fscanf(pFile, "%s", &aString[0]);		//�R�����g(POS)
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].pos.x);		//x���W
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].pos.y);		//y���W
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].pos.z);		//z���W

					//�������擾
					fscanf(pFile, "%s", &aString[0]);		//�R�����g(ROT)
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].rot.x);		//x���W
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].rot.y);		//y���W
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].rot.z);		//z���W

					//�I�������擾
					fscanf(pFile, "%s", &aEndStr[0]);

					if (strcmp(&aEndStr[0], END_SETPLAYERPARTS) != 0)
					{//�I���������Ȃ��ꍇ
						break;	//�J��Ԃ��𔲂���
					}
				}
				else
				{//�Ȃ������ꍇ
					break;
				}
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�J���Ȃ������ꍇ

	}
}

//==========================================================
//�v���C���[�̃��[�V�����f�[�^�ǂݍ���
//==========================================================
void LoadPlayerMotionData(void)
{
	FILE *pFile;

	pFile = fopen("data\\TXT\\motion.txt", "r");

	if (pFile != NULL)
	{//�J�����ꍇ

		char aNumStr[128] = {};	//�z�u�������i�[�p
		fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
		fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(#)
		fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(���[�V�������)
		fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)

		for (int nCntMotion = 0; nCntMotion < PLAYERMOTION_ACTION; nCntMotion++)
		{
			fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
			fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(#)
			fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(...���[�V����)
			fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
			fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(�m�F����)

			if (strcmp(&aNumStr[0], YES_SETPLAYERMOTION) == 0)
			{//�m�F�̕������������ꍇ
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(LOOP)
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(=)
				fscanf(pFile, "%d", &g_Player.MotionData.aLoop[nCntMotion]);	//���[�v�擾
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(#)
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g()
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(/)
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g()

				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(KEY)
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(=)
				fscanf(pFile, "%d", &g_Player.MotionData.aNumKey[nCntMotion]);	//���[�v�擾
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(#)
				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(����)

				for (int nCntPlayerKey = 0; nCntPlayerKey < g_Player.MotionData.aNumKey[nCntMotion]; nCntPlayerKey++)
				{
					fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(�m�F����)

					if (strcmp(&aNumStr[0], YES_SETPLAYERKEYSET) == 0)
					{//�J�n�������������ꍇ
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(#)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(<<)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(KEY)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(:)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(����)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(/)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(����)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(>>)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(FRAME)
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(=)
						fscanf(pFile, "%d", &g_Player.MotionData.aNumFrame[nCntMotion][nCntPlayerKey]);		//�R�����g(=)


						for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
						{
							fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(�m�F����)

							if (strcmp(&aNumStr[0], YES_SETPLAYERKEY) == 0)
							{//�J�n�������������ꍇ
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(#)
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g([)
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(��)

								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(POS)
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(=)
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].pos.x);		//x���W
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].pos.y);		//y���W
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].pos.z);		//z���W

																									//�������擾
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(ROT)
								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(=)
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].rot.x);		//x���W
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].rot.y);		//y���W
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].rot.z);		//z���W

								fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(�m�F����)
							}
						}
						fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(�m�F����)
					}
				}

				fscanf(pFile, "%s", &aNumStr[0]);		//�R�����g(�m�F����)
			}
			else
			{//�Ȃ������ꍇ

			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�J���Ȃ������ꍇ

	}
}

//==========================================================
//�v���C���[�̃��[�V�����ݒ�
//==========================================================
void MotionSetData(void)
{
	for (int nCntMotion = 0; nCntMotion < PLAYERMOTION_ACTION; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < g_Player.MotionData.aNumKey[nCntMotion]; nCntKey++)
		{
			//��t���[��������̔{�������߂�
			float fFrame = 1.0f / (float)g_Player.MotionData.aNumFrame[nCntMotion][nCntKey];

			for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
			{
				//�����̈ړ��ʂ̍��������߂�
				g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey] =
					(g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[(nCntKey + 1) % g_Player.MotionData.aNumKey[nCntMotion]].rot
						- g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntKey].rot);

				//�p�x�̊m�F
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
				{//-3.14�`3.14�͈̔͊O�̏ꍇ
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
				{//-3.14�`3.14�͈̔͊O�̏ꍇ
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
				{//-3.14�`3.14�͈̔͊O�̏ꍇ
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (D3DX_PI * 2);
					}
				}

				//�␳��������
				g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey] *= fFrame;

				//�p�x�̊m�F
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
				{//-3.14�`3.14�͈̔͊O�̏ꍇ
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
				{//-3.14�`3.14�͈̔͊O�̏ꍇ
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
				{//-3.14�`3.14�͈̔͊O�̏ꍇ
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (D3DX_PI * 2);
					}
				}

				//���W�̈ړ��ʂ̍��������߂�
				g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].posDiff[nCntKey] =
					(g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[(nCntKey + 1) % g_Player.MotionData.aNumKey[nCntMotion]].pos
						- g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntKey].pos)
					* fFrame;
			}
		}
	}
}

//==========================================================
//�������[�V�����p�x�ݒ�
//==========================================================
void MotionSet(PLAYERMOTION motion)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].rot = g_aPlayerParts[nCntPlayerParts].aKey[motion].aKeyData[0].rot;
	}
}

//==========================================================
//�����p�[�c���W�ݒ�
//==========================================================
void SetPlayerPartsPos(void)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].pos = g_aPlayerParts[nCntPlayerParts].Setpos;
	}
}

//==========================================================
//�ړ����[�V�����ݒ�
//==========================================================
void SetMoveMotion(void)
{
	if (g_Player.bMove == false)
	{//�ړ���Ԃł͂Ȃ��ꍇ
		g_Player.bMove = true;	//�ړ���Ԃɂ���
		g_Player.MotionData.nMotionNum = PLAYERMOTION_WALK;	//�ړ���Ԃɂ���
		g_Player.MotionData.nNowFrame = 0;					//�t���[���������Z�b�g
		g_Player.MotionData.PatternKey = 0;					//���݂̃L�[��������

		//���W�Z�b�g
		SetPlayerPartsPos();

		//���[�V�����p�x�ݒ�
		MotionSet(PLAYERMOTION_WALK);

	}
}

//==========================================================
//�ҋ@���[�V�����ݒ�
//==========================================================
void SetNeutralMotion(void)
{
	if (g_Player.move.x <= MIN_MOVEMOTION && g_Player.move.x >= 0.0f ||
		g_Player.move.x >= -MIN_MOVEMOTION && g_Player.move.x <= 0.0f)
	{//x���W�̈ړ��ʂ��Ȃ��ꍇ

		if (g_Player.move.z <= MIN_MOVEMOTION && g_Player.move.z >= 0.0f ||
			g_Player.move.z >= -MIN_MOVEMOTION && g_Player.move.z <= 0.0f)
		{//z���W�̈ړ��ʂ��Ȃ��ꍇ

			if (g_Player.bMove == true)
			{//�v���C���[���ړ���Ԃ̎�
				g_Player.bMove = false;	//�ړ����Ă��Ȃ���Ԃɂ���
				g_Player.MotionData.nMotionNum = PLAYERMOTION_NEUTRAL;	//�ҋ@��Ԃɂ���
				g_Player.MotionData.nNowFrame = 0;					//�t���[���������Z�b�g
				g_Player.MotionData.PatternKey = 0;					//���݂̃L�[��������

				//���W�Z�b�g
				SetPlayerPartsPos();

				//���[�V�����p�x�ݒ�
				MotionSet(PLAYERMOTION_NEUTRAL);
			}
		}
	}
}
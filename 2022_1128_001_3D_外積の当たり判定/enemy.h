//==========================================================
//
//�G�`�揈��
//Author ����ɐ�
//
//==========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"			//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//==========================================================
//�}�N����`
//==========================================================
#define MAX_ENEMY	(128)	//�G�̍ő吔

//==========================================================
//�񋓌^�̒�`
//==========================================================
//�G���
typedef enum
{
	ENEMYTYPE_CAR = 0,	//��
	ENEMYTYPE_ZIHANKI,	//���̋@
	ENEMYTYPE_BENCH,	//�x���`
	ENEMYTYPE_MAX
}ENEMYTYPE;

//�G���
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//��
	ENEMYSTATE_DAMAGE,	//���̋@
	ENEMYSTATE_DEATH,	//�x���`
	ENEMYSTATE_MAX
}ENEMYSTATE;


//==========================================================
//�\���̂̒�`
//==========================================================
//�G
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 vtxMin;		//�ŏ��l
	D3DXVECTOR3 vtxMax;		//�ő�l
	int nType;				//���
	int nLife;				//�̗�
	bool bUse;				//�g�p����Ă��邩�ǂ���
	LPD3DXMESH pMesh = {};	//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = {};	//�}�e���A���ւ̃|�C���^
	LPDIRECT3DTEXTURE9 apTexture[20] = {};	//�e�N�X�`���̃|�C���^
	DWORD dwNumMat = 0;		//�}�e���A���̐�
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nShadow = -1;		//�e�̔ԍ�
	int nStateCounter;		//��ԑJ�ڃJ�E���^�[
	int nState;				//���
}Enemy;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, int nType);
Enemy *GetEnemy(void);
void HitEnemy(int nDamage, Enemy *pEnemy);

#endif

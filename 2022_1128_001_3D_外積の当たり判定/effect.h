//===================================================
//
//�G�t�F�N�g����
//Author ����ɐ�
//
//===================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//===================================================
//�񋓌^�̒�`
//===================================================
//�G�t�F�N�g�g�p���
typedef enum
{
	EFFECTTYPE_NORMAL = 0,	//�ʏ�
	EFFECTTYPE_MOVE,		//�ړ�
	EFFECTTYPE_JUMP,		//�W�����v
	EFFECTTYPE_CHANGE,		//�d�͕ω�
	EFFECTTYPE_DIVE,		//�W�����v��
	EFFECTTYPE_ITEMSTAR,	//�A�C�e����
	EFFECTTYPE_ITEMCOIN,	//�A�C�e���R�C��
	EFFECTTYPE_ITEMINK,		//�A�C�e���C���N
}EFFECTTYPE;

//===================================================
//�v���g�^�C�v�錾
//===================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int ntype);
int GetEffect(void);

#endif 

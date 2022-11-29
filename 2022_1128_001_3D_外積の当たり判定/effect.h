//===================================================
//
//エフェクト処理
//Author 奥定伊吹
//
//===================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//===================================================
//列挙型の定義
//===================================================
//エフェクト使用種類
typedef enum
{
	EFFECTTYPE_NORMAL = 0,	//通常
	EFFECTTYPE_MOVE,		//移動
	EFFECTTYPE_JUMP,		//ジャンプ
	EFFECTTYPE_CHANGE,		//重力変化
	EFFECTTYPE_DIVE,		//ジャンプ中
	EFFECTTYPE_ITEMSTAR,	//アイテム星
	EFFECTTYPE_ITEMCOIN,	//アイテムコイン
	EFFECTTYPE_ITEMINK,		//アイテムインク
}EFFECTTYPE;

//===================================================
//プロトタイプ宣言
//===================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int ntype);
int GetEffect(void);

#endif 

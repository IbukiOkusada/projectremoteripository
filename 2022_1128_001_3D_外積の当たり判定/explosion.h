//===================================================
//
//爆発処理
//Author 奥定伊吹
//
//===================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"		//main.hで定義しているものが必要なためinclude

//===================================================
//マクロ定義
//===================================================
#define MAX_EXPLOSION	(128)	//爆発の最大数

//==========================================================
//影の構造体
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用されているかどうか
	int nCounterAnim;		//アニメーションカウンター
	int nPatternAnim;		//アニメーションパターンNo.
	float fRadian;			//半径
}Explosion;

//===================================================
//プロトタイプ宣言
//===================================================
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadian);

#endif 

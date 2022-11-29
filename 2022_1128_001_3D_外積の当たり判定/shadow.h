//==========================================================
//
//影描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"		//main.hで定義しているものが必要なためinclude


//==========================================================
//影の構造体
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;					//使用されているかどうか
}Shadow;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);
void SetPositionShadow(int nIdexShadow, D3DXVECTOR3 pos);
void ResetShadow(int nShadow);

#endif

//==========================================================
//
//ビルボード描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"		//main.hで定義しているものが必要なためinclude

//==========================================================
//壁の構造体
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用されているかどうか
}Billboard;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight);

#endif

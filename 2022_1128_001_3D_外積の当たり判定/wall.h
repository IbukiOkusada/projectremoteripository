//==========================================================
//
//壁描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"			//main.hで定義しているものが必要なためinclude

//==========================================================
//マクロ定義
//==========================================================
#define MAX_WALL	(5)		//壁の最大数

//==========================================================
//壁の構造体
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用されているかどうか
}Wall;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
Wall *GetWall(void);

#endif

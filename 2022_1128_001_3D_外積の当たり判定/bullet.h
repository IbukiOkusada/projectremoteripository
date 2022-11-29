//===================================================
//
//弾の発射処理
//Author 奥定伊吹
//
//===================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"			//main.hで定義しているものが必要なためinclude

//===================================================
//マクロ定義
//===================================================
#define MAX_BULLET	(128)	//弾の最大数

//===================================================
//弾の構造体
//===================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nShadow;			//使用する影の番号
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用されているかどうか
}Bullet;

//===================================================
//プロトタイプ宣言
//===================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fWidth);
int GetTrueBullet(void);
Bullet *GetBullet(void);

#endif 

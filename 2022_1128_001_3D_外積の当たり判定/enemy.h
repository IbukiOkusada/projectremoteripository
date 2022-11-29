//==========================================================
//
//敵描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"			//main.hで定義しているものが必要なためinclude

//==========================================================
//マクロ定義
//==========================================================
#define MAX_ENEMY	(128)	//敵の最大数

//==========================================================
//列挙型の定義
//==========================================================
//敵種類
typedef enum
{
	ENEMYTYPE_CAR = 0,	//車
	ENEMYTYPE_ZIHANKI,	//自販機
	ENEMYTYPE_BENCH,	//ベンチ
	ENEMYTYPE_MAX
}ENEMYTYPE;

//敵状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//車
	ENEMYSTATE_DAMAGE,	//自販機
	ENEMYSTATE_DEATH,	//ベンチ
	ENEMYSTATE_MAX
}ENEMYSTATE;


//==========================================================
//構造体の定義
//==========================================================
//敵
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 vtxMin;		//最小値
	D3DXVECTOR3 vtxMax;		//最大値
	int nType;				//種類
	int nLife;				//体力
	bool bUse;				//使用されているかどうか
	LPD3DXMESH pMesh = {};	//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat = {};	//マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 apTexture[20] = {};	//テクスチャのポインタ
	DWORD dwNumMat = 0;		//マテリアルの数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nShadow = -1;		//影の番号
	int nStateCounter;		//状態遷移カウンター
	int nState;				//状態
}Enemy;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, int nType);
Enemy *GetEnemy(void);
void HitEnemy(int nDamage, Enemy *pEnemy);

#endif

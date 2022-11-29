//==========================================================
//
//メッシュウォール描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_MESHWALL	(5)		//壁の最大数

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
	int nHeightMesh;		//高さ枚数
	int nWidthMesh;			//幅枚数
	int nVertex;			//頂点数
	int nIndex;				//描画番号数
	bool bUse;				//使用されているかどうか
}MeshWall;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth);
void SetIndexMeshWall(int nCntWall);
void SetpVtxMeshWall(int nCntWall);
MeshWall *GetMeshWall(void);

#endif

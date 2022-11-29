//==========================================================
//
//メッシュドーム描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

#include "main.h"

//==========================================================
//マクロ定義
//==========================================================

//==========================================================
//壁の構造体
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fLength;			//距離
	float fRot;				//一頂点の角度
	float fHeightRot;		//y軸角度
	float fHeight;			//高さ
	int nWidthMesh;			//幅枚数
	int nVertex;			//頂点数
	int nIndex;				//描画番号数
	bool bUse;				//使用されているかどうか
}MeshDome;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitMeshDome(void);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth);
void SetIndexMeshDome(int nCntWall);
void SetpVtxMeshDome(int nCntWall);

#endif

//==========================================================
//
//メッシュシリンダー描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_MESHCYLINDER	(5)		//壁の最大数

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
	float fHeight;			//高さ
	int nWidthMesh;			//幅枚数
	int nVertex;			//頂点数
	int nIndex;				//描画番号数
	bool bUse;				//使用されているかどうか
}MeshCylinder;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth);
void SetIndexMeshCylinder(int nCntWall);
void SetpVtxMeshCylinder(int nCntWall);

#endif

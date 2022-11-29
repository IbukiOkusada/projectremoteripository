//==========================================================
//
//カメラ処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"			//main.hで定義しているものが必要なためinclude

//==========================================================
//カメラの構造体
//==========================================================
typedef struct
{
	D3DXVECTOR3 posV;		//視点
	D3DXVECTOR3 posR;		//注視点
	D3DXVECTOR3 posVDest;	//目的の視点
	D3DXVECTOR3 posRDest;	//目的の注視点
	D3DXVECTOR3 vecU;		//上方向ベクトル
	D3DXVECTOR3 move;		//移動量
	float fLength;			//長さ
	float fHeight;			//高さ
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotDest;	//目的の向き
}Camera;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);
void SetRotCamera(void);

#endif
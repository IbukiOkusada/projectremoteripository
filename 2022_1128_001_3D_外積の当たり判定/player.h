//==========================================================
//
//プレイヤー描画処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"			//main.hで定義しているものが必要なためinclude

//==========================================================
//マクロ定義
//==========================================================
#define MAX_PLAYERPARTS		(64)	//プレイヤーのモデル最大数
#define MAX_PLAYERKEY		(64)	//プレイヤーのキー最大数
#define MAX_PLAYERMOTION	(128)	//プレイヤーモーションの最大数
#define MAX_PLAYERFRAME		(64)	//プレイヤーモーションフレーム最大数

//==========================================================
//列挙型の定義
//==========================================================
//モーションパターン
typedef enum
{
	PLAYERMOTION_NEUTRAL = 0,	//待機モーション
	PLAYERMOTION_WALK,			//歩きモーション
	PLAYERMOTION_ACTION,		//アクションモーション
	PLAYERMOTION_JUMP,			//ジャンプモーション
	PLAYERMOTION_LAND,			//着地モーション
	PLAYERMOTION_MAX
}PLAYERMOTION;

//ループ
typedef enum
{
	PLAYERLOOP_OFF = 0,	//ループオフ
	PLAYERLOOP_ON,		//ループオン
	PLAYERLOOP_MAX
}PLAYERLOOP;

//==========================================================
//構造体の定義
//==========================================================
//キー内情報
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
}KeyData;

//キー情報
typedef struct
{
	KeyData aKeyData[MAX_PLAYERMOTION];		//キー情報の配列
	D3DXVECTOR3 rotDiff[MAX_PLAYERMOTION];	//向きの差分
	D3DXVECTOR3 posDiff[MAX_PLAYERMOTION];	//座標の差分
}Key;

//モーションデータ
typedef struct
{
	int aNumKey[MAX_PLAYERMOTION];		//各モーションのキー数
	int aNumFrame[MAX_PLAYERMOTION][MAX_PLAYERKEY];		//各モーションのキーフレーム数
	int PatternKey;	//現在のキー数
	int nNowFrame;	//現在のフレーム数
	int nMotionNum;	//現在のモーション番号
	int aLoop[MAX_PLAYERMOTION];		//ループするかどうか
}MotionData;

//プレイヤー
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 movespeed;	//移動速度
	float fJumpPower;		//プレイヤーのジャンプ力
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nShadow;			//影の番号
	MotionData MotionData;	//モーション情報
	bool bMove;				//移動しているかどうか
	D3DXVECTOR3 vtxMin;		//最小値
	D3DXVECTOR3 vtxMax;		//最大値
	float fRotMove;			//現在の角度
	float fRotDiff;			//目的の角度
	float fRotDest;			//角度計算用
}Player;

//プレイヤーのパーツ
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 Setpos;		//初期基準位置
	D3DXVECTOR3 rot;		//向き
	Key aKey[MAX_PLAYERKEY];	//キー情報
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nPartNum;			//使用するパーツの番号
	int nParent;			//親の番号

	LPD3DXMESH pMesh = {};	//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat = {};	//マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 apTexture[20] = {};	//テクスチャのポインタ
	DWORD dwNumMat = 0;		//マテリアルの数
}PlayerParts;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif

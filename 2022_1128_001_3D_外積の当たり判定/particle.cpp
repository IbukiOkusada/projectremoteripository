//===================================================
//
//パーティクル処理
//Author 奥定伊吹
//
//===================================================
#include "particle.h"
#include "effect.h"
#include <stdlib.h>

//===================================================
//マクロ定義
//===================================================
#define MAX_PARTICLE			(128)		//パーティクルの最大数
#define MIN_MOVEPARTICLE		(3.8f)		//パーティクルの最低移動量
#define MIN_BLOCKMOVEPARTICLE	(0.8f)		//ブロック最低移動量
#define DOUBLEPI				(628)		//一周分の整数角度
#define PI						(314)		//半周分の整数角度
#define HALFPI					(157)		//四分の一周分の整数角度

//===================================================
//構造体の定義
//===================================================
//パーティクル
typedef struct
{
	D3DXVECTOR3 pos;	//座標
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//色
	float fRadius;		//半径
	float fRange;		//半径
	int nLife;			//寿命
	int nType;			//種類
	int nAppear;		//1フレームに出す量
	int nMeLife;		//相手に与える寿命
	bool bUse;			//使用しているかどうか
}Particle;

//===================================================
//グローバル変数
//===================================================
Particle g_aParticle[MAX_PARTICLE];	//パーティクルの情報

//===================================================
//プロトタイプ宣言
//===================================================
void SetChangeParticle(Particle * pParticle);
void SetMoveParticle(Particle * pParticle);
void SetJumpParticle(Particle * pParticle);
void SetDiveParticle(Particle * pParticle);

//===================================================
//パーティクルの初期化
//===================================================
void InitParticle(void)
{
	//配列の情報初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(50.0, 0.0f, 0.0f);		//座標を初期化
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量を初期化
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色を初期化
		g_aParticle[nCntParticle].fRadius = 0.0f;							//半径を初期化
		g_aParticle[nCntParticle].nLife = 0;								//体力を初期化
		g_aParticle[nCntParticle].nMeLife = 0;								//相手に与える体力を初期化
		g_aParticle[nCntParticle].bUse = false;								//使用していない状態にする
	}
}

//===================================================
//パーティクルの終了
//===================================================
void UninitParticle(void)
{

}

//===================================================
//パーティクルの更新
//===================================================
void UpdateParticle(void)
{
	Particle *pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == true)
		{//使用されていたら
			switch (pParticle->nType)
			{
			case EFFECTTYPE_NORMAL:
				for (int nCntAppear = 0; nCntAppear < 15; nCntAppear++)
				{
					//発生位置の設定
					pParticle->pos = pParticle->pos;
					//移動量の設定
					pParticle->move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 51 - 25)) / 10.0f;
					pParticle->move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 51 - 25)) / 10.0f;
					pParticle->move.z = sinf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 51 - 25)) / 10.0f;
					//色の設定
					pParticle->col = D3DXCOLOR(1.0f, (float)(rand() % 6) / 10.0f, 0.0f, 1.0f);
					//半径の設定
					pParticle->fRadius = 10.0f;
					//寿命の設定
					pParticle->nMeLife = 20;

					//エフェクトの設定
					SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);

				}
				break;
			case EFFECTTYPE_MOVE:	//移動
				SetMoveParticle(pParticle);
				break;
			case EFFECTTYPE_JUMP:	//ジャンプ
				SetJumpParticle(pParticle);
				break;
			case EFFECTTYPE_CHANGE:	//重力状態変更
				SetChangeParticle(pParticle);
				break;
			case EFFECTTYPE_DIVE:	//飛び込み中
				SetDiveParticle(pParticle);
				break;
			}

			//寿命を減らす
			pParticle->nLife--;

			if (pParticle->nLife <= 0)
			{//寿命が無くなった場合
				pParticle->bUse = false;
			}
		}
	}
}

//===================================================
//パーティクルの設定
//===================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, float fRange)
{
	Particle *pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == false)
		{//使用されていない場合
			pParticle->pos = pos;			//座標を設定
			pParticle->nType = nType;		//種類を設定
			pParticle->fRange = fRange;		//配置半径を設定
			pParticle->move = move;
			//体力
			switch (pParticle->nType)
			{
			case EFFECTTYPE_NORMAL:
				pParticle->nLife = 0;
				break;
			}

			pParticle->bUse = true;		//使用している状態に

			break;
		}
	}
}

//===================================================
//パーティクルのリセット
//===================================================
void ResetParticle(void)
{
	Particle *pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		pParticle->bUse = false;
	}
}

//===================================================
//
//パーティクルの各設定
//
//===================================================
//重力変化パーティクルの設定
//===================================================
void SetChangeParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;
	for (int nCntAppear = 0; nCntAppear < 25; nCntAppear++)
	{
		//発生位置の設定
		pParticle->pos = pParticle->pos;
		//移動量の設定
		pParticle->move.x = sinf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 30.0f;
		pParticle->move.y = cosf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 30.0f;
		//色の設定
		pParticle->col = D3DXCOLOR(rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 0.9f);
		//半径の設定
		pParticle->fRadius = rand() % 4000 / 100.0f;
		//寿命の設定
		pParticle->nMeLife = 25;

		//エフェクトの設定
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);

	}
}

//===================================================
//移動パーティクルの設定
//===================================================
void SetMoveParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;

	for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++)
	{
		float fRotMove;	//現在の移動量

		fRotMove = atan2f(pParticle->move.x, pParticle->move.y);	//現在の移動方向(角度)

																	//発生位置の設定
		pParticle->pos = pParticle->pos;
		//移動量の設定
		pParticle->move.x = sinf((float)(rand() % 315 - 157) / 100.0f) * (float)(rand() % 100) * 0.1f;
		pParticle->move.z = sinf((float)(rand() % 315 - 157) / 100.0f) * (float)(rand() % 100) * 0.1f;
		pParticle->move.y = cosf((float)(rand() % 315 - 157) / 100.0f) * (float)(rand() % 100) * 0.1f;
		//色の設定
		pParticle->col = D3DXCOLOR(1.0f, (rand() % 5 + 5) * 0.1f, 0.0f, 1.0f);
		//半径の設定
		pParticle->fRadius = (float)(rand() % 100) * 0.1f;
		//寿命の設定
		pParticle->nMeLife = 100;
		//エフェクトの設定
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);
	}
}

//===================================================
//ジャンプパーティクルの設定
//===================================================
void SetJumpParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;

	for (int nCntAppear = 0; nCntAppear < 
		2; nCntAppear++)
	{
		//発生位置の設定
		pParticle->pos = pParticle->pos;
		//移動量の設定
		pParticle->move.y = cosf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 300.0f;
		pParticle->move.x = cosf((float)(rand() % nRange - ((nRange - 1) * 0.5f)) / 100.0f) + ((float)(rand() % 201 - 100)) / 50.0f;
		//色の設定
		pParticle->col = D3DXCOLOR(0.5f, 0.4f, 0.4f, 0.9f);
		//半径の設定
		pParticle->fRadius = 20.0f;
		//寿命の設定
		pParticle->nMeLife = 20;

		//エフェクトの設定
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);
	}
}

//===================================================
//飛び込み中パーティクルの設定
//===================================================
void SetDiveParticle(Particle * pParticle)
{
	int nRange = DOUBLEPI + 1;

	int nMove = (int)pParticle->move.y * 2;

	if (nMove < 0)
	{
		nMove *= -1;
	}

	for (int nCntAppear = 0; nCntAppear < nMove; nCntAppear++)
	{
		//移動量の設定
		pParticle->move.x = 0.0f;
		//色の設定
		pParticle->col = D3DXCOLOR(0.4f, 0.7f, 0.8f, 0.05f);
		//半径の設定
		pParticle->fRadius = rand() % 500 / 10.0f;
		//寿命の設定
		pParticle->nMeLife = 10;

		//エフェクトの設定
		SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fRadius, pParticle->nMeLife, pParticle->nType);
	}
}
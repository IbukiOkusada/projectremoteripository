//===================================================
//
//エフェクト処理
//Author 奥定伊吹
//
//===================================================
#include "main.h"
#include "effect.h"
#include <stdlib.h>
#include <time.h>

//===================================================
//マクロ定義
//===================================================
#define MAX_EFFECT	(20048)							//エフェクトの最大数
#define MYEFFECT	"data\\TEXTURE\\effect000.jpg"	//エフェクトのテクスチャ名

//===================================================
//構造体の定義
//===================================================
//エフェクト
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fRadius;		//半径
	int nLife;			//寿命
	int type;			//種類
	bool bUse;			//使用しているかどうか
}Effect;

//===================================================
//列挙型の定義
//===================================================
//サイズランダム
typedef enum
{
	EFFECTSIZE_NORMAL = 0,	//小さく
	EFFECTSIZE_SMALL,	//さらに小さく
	EFFECTSIZE_BIG,		//大きく
	EFFECTSIZE_MAX
}EFFECTSIZE;

//===================================================
//グローバル変数
//===================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];						//エフェクトの情報
int g_EffectCnt;									//現在使用されているエフェクトのカウント

//===================================================
//プロトタイプ宣言
//===================================================
void SetChangeEffect(Effect *pEffect);
void SetMoveEffect(Effect *pEffect);
void SetJumpEffect(Effect *pEffect);
void pVtxEffect(int nCntEffect);

//===================================================
//エフェクトの初期化処理
//===================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MYEFFECT, &g_pTextureEffect);

	//エフェクトの情報の初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].bUse = false;		//使用していない状態にする
		g_aEffect[nCntEffect].type = EFFECTTYPE_MOVE;
	}

	g_EffectCnt = 0;	//カウント初期化

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//乱数の種の初期化
	srand((unsigned)time(0));

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//pVtxの設定
		pVtxEffect(nCntEffect);
	}

}

//===================================================
//エフェクトの終了処理
//===================================================
void UninitEffect(void)
{
	//テクスチャの廃棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//===================================================
//エフェクトの更新処理
//===================================================
void UpdateEffect(void)
{

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている場合

			switch (g_aEffect[nCntEffect].type)
			{
			case EFFECTTYPE_CHANGE:	//重力変化状態
				SetChangeEffect(&g_aEffect[nCntEffect]);
				break;
			case EFFECTTYPE_MOVE:	//移動
				SetMoveEffect(&g_aEffect[nCntEffect]);
				break;
			case EFFECTTYPE_JUMP:	//ジャンプ
				SetJumpEffect(&g_aEffect[nCntEffect]);
				break;
			case 0:
				g_aEffect[nCntEffect].fRadius *= 0.95f;
				g_aEffect[nCntEffect].col.a *= 0.99f;
				break;
			}

			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	//座標に移動量を加算

			//エフェクトの寿命を減らす
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{//寿命が無くなった場合
				g_aEffect[nCntEffect].bUse = false;	//使用されていない状態に
			}

			//pVtxの設定
			pVtxEffect(nCntEffect);
		}
	}
}

//===================================================
//エフェクトの描画処理
//===================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//ライティングをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効化する
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている場合

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);
			g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

		 //頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffEffect,
				0,
				sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntEffect * 4,
				2	//頂点情報構造体のサイズ
			);
		}
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	//ライティングをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===================================================
//エフェクトの設定処理
//===================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//エフェクトが使用されていない場合
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].type = type;
			g_aEffect[nCntEffect].bUse = true;		//使用している状態にする
			g_EffectCnt++;
			break;
		}
	}
}

//===================================================
//エフェクトのpVtx設定
//===================================================
void pVtxEffect(int nCntEffect)
{
	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntEffect * 4;	//ポインタをカウントまで進める

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = g_aEffect[nCntEffect].col;
	pVtx[1].col = g_aEffect[nCntEffect].col;
	pVtx[2].col = g_aEffect[nCntEffect].col;
	pVtx[3].col = g_aEffect[nCntEffect].col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//===================================================
//エフェクトの数取得
//===================================================
int GetEffect(void)
{
	return g_EffectCnt;
}

//===================================================
//
//エフェクトの各設定
//
//===================================================
//重力変化
//===================================================
void SetChangeEffect(Effect *pEffect)
{
	pEffect->col.a *= 0.9f;
}

//===================================================
//移動
//===================================================
void SetMoveEffect(Effect *pEffect)
{
	pEffect->col.a *= 0.9f;
}

//===================================================
//ジャンプ
//===================================================
void SetJumpEffect(Effect *pEffect)
{
	pEffect->col.a *= 0.8f;
}
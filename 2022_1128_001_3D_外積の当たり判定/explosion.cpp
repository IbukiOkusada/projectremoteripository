//===================================================
//
//爆発処理
//Author 奥定伊吹
//
//===================================================
#include "explosion.h"

//===================================================
//マクロ定義
//===================================================
#define MYEXPLOSION_TEXTURE	"data\\TEXTURE\\explosion000.png"	//爆発のテクスチャ名

//===================================================
//マクロ定義
//===================================================
void pVtxExplosion(int nCntExplosion);

//===================================================
//グローバル変数
//===================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];				//爆発の情報

//===================================================
//爆発の初期化処理
//===================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MYEXPLOSION_TEXTURE, &g_pTextureExplosion);

	//爆発の情報の初期化
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].bUse = false;		//使用していない状態にする
		g_aExplosion[nCntExplosion].fRadian = 0.0f;	//カウントを初期化
		g_aExplosion[nCntExplosion].nCounterAnim = 0;	//カウントを初期化
		g_aExplosion[nCntExplosion].nPatternAnim = 0;	//パターンNo.を初期化
	}

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	//pVtxの設定


}

//===================================================
//爆発の終了処理
//===================================================
void UninitExplosion(void)
{
	//テクスチャの廃棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//===================================================
//爆発の更新処理
//===================================================
void UpdateExplosion(void)
{
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている場合
			g_aExplosion[nCntExplosion].nCounterAnim++;	//アニメーションのカウンターを進める

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 5) == 0)
			{
				g_aExplosion[nCntExplosion].nPatternAnim++;	//パターンNo.を更新

				//pVtxの設定
				pVtxExplosion(nCntExplosion);

				if (g_aExplosion[nCntExplosion].nPatternAnim >= 8)
				{//パターン数を超えた場合
					g_aExplosion[nCntExplosion].bUse = false;	//使用されていない状態にする
				}
			}
		}
	}
}

//===================================================
//爆発の描画処理
//===================================================
void DrawExplosion(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	if (GetZtest() == 0)
	{
		//Zテストを無効化する
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている場合

		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aExplosion[nCntExplosion].mtxWorld, NULL, &mtxView);
			g_aExplosion[nCntExplosion].mtxWorld._41 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._42 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._43 = 0.0f;
			/*g_aExplosion[nCntExplosion].mtxWorld._31 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._32 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._33 = 0.0f;
			g_aExplosion[nCntExplosion].mtxWorld._34 = 0.0f;*/

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffExplosion,
				0,
				sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntExplosion * 4,
				2	//頂点情報構造体のサイズ
			);
		}
	}

	if (GetZtest() == 0)
	{
		//Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//===================================================
//爆発の設定処理
//===================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadian)
{
	int nCntExplosion;

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発が使用されていない場合
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].fRadian = fRadian;
			g_aExplosion[nCntExplosion].bUse = true;		//使用している状態にする

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;

			//pVtxの設定
			pVtxExplosion(nCntExplosion);

			break;

		}
	}
}

//===================================================
//爆発のpVtx設定
//===================================================
void pVtxExplosion(int nCntExplosion)
{
	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntExplosion * 4;	//ポインタをカウントまで進める

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fRadian, +g_aExplosion[nCntExplosion].fRadian, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aExplosion[nCntExplosion].fRadian, +g_aExplosion[nCntExplosion].fRadian, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fRadian, -g_aExplosion[nCntExplosion].fRadian, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aExplosion[nCntExplosion].fRadian, -g_aExplosion[nCntExplosion].fRadian, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim + 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim + 0.125f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffExplosion->Unlock();
}


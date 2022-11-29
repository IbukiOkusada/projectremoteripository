//===================================================
//
//弾の発射処理
//Author 奥定伊吹
//
//===================================================
#include "bullet.h"
#include "shadow.h"
#include "wall.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "enemy.h"
#include "model.h"
#include "meshwall.h"

//===================================================
//マクロ定義
//===================================================
#define MYBULLET_TEXTURE	"data\\TEXTURE\\bullet000.png"	//弾のテクスチャ名

//===================================================
//プロトタイプ宣言
//===================================================
void pVtxBullet(int nCntBullet);
void CollisionWall(int nCntBullet);
void CollisionEnemy(int nCntBullet);

//===================================================
//グローバル変数
//===================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];		//弾の情報

//===================================================
//弾の初期化処理
//===================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MYBULLET_TEXTURE, &g_pTextureBullet);

	//弾の情報の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
		g_aBullet[nCntBullet].fHeight = 0.0f;
		g_aBullet[nCntBullet].fWidth = 0.0f;
		g_aBullet[nCntBullet].nShadow = -1;
	}

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//pVtxの設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		pVtxBullet(nCntBullet);
	}

}

//===================================================
//弾の終了処理
//===================================================
void UninitBullet(void)
{
	//テクスチャの廃棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===================================================
//弾の更新処理
//===================================================
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている場合
			//前回の位置を取得
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//位置の更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//影の更新
			SetPositionShadow(g_aBullet[nCntBullet].nShadow, g_aBullet[nCntBullet].pos);

			//壁との当たり判定
			CollisionWall(nCntBullet);

			//敵との当たり判定
			CollisionEnemy(nCntBullet);

			//エフェクトの設定
			SetEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move * 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].fWidth, 10, 0);

		}
	}
}

//===================================================
//弾の描画処理
//===================================================
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//ライティングをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (GetAlphaTest() == 1)
	{
		//アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	}

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている場合

		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffBullet,
				0,
				sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntBullet * 4,
				2	//頂点情報構造体のサイズ
			);
		}
	}

	if (GetAlphaTest() == 1)
	{
		//アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	}

	//ライティングをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===================================================
//弾の設定処理
//===================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fWidth)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない場合
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].fWidth = fWidth;
			g_aBullet[nCntBullet].fHeight = fHeight;

			g_aBullet[nCntBullet].bUse = true;		//使用している状態にする

			pVtxBullet(nCntBullet);

			g_aBullet[nCntBullet].nShadow = SetShadow();

			break;
		}

	}
}

//===================================================
//弾数取得処理
//===================================================
int GetTrueBullet(void)
{
	int nCntBullet;
	int nBulletCounter = 0;		//現在表示されている弾数を覚える

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている場合
			nBulletCounter++;		//カウントをプラスする
		}
	}

	return nBulletCounter;
}

//===================================================
//弾のpVtx設定
//===================================================
void pVtxBullet(int nCntBullet)
{
	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntBullet * 4;	//ポインタをカウントまで進める

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, +g_aBullet[nCntBullet].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].fWidth, +g_aBullet[nCntBullet].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, -g_aBullet[nCntBullet].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].fWidth, -g_aBullet[nCntBullet].fHeight, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//===================================================
//弾と壁の当たり判定
//===================================================
void CollisionWall(int nCntBullet)
{
	Wall *pWall = GetWall();
	D3DXVECTOR3 vecLine, vecToPos;	//判定用変数

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
				vecLine = D3DXVECTOR3((pWall->pos.x + cosf(pWall->rot.y) * (-pWall->fWidth)) - (pWall->pos.x + cosf(pWall->rot.y) * (pWall->fWidth)),
					0.0f,
					(pWall->pos.z + sinf(pWall->rot.y) * (pWall->fWidth)) - (pWall->pos.z + sinf(pWall->rot.y) * (-pWall->fWidth)));

				vecToPos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (pWall->pos.x + cosf(pWall->rot.y) * (pWall->fWidth)),
					0.0f,
					g_aBullet[nCntBullet].pos.z - (pWall->pos.z + sinf(pWall->rot.y) * (-pWall->fWidth)));

				if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) > 0.0f)
				{
					g_aBullet[nCntBullet].bUse = false;
					ResetShadow(g_aBullet[nCntBullet].nShadow);	//使用していた影を使用されていない状態にする
					SetParticle(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_MOVE, 5.0f);
				}
		}
	}
}

//===================================================
//敵と弾の当たり判定
//===================================================
void CollisionEnemy(int nCntBullet)
{
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されている場合
			if (g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth >= pEnemy->pos.x + pEnemy->vtxMin.x &&
				g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth <= pEnemy->pos.x + pEnemy->vtxMax.x &&
				g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fWidth >= pEnemy->pos.z + pEnemy->vtxMin.z &&
				g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fWidth <= pEnemy->pos.z + pEnemy->vtxMax.z)
			{//敵の範囲内に入った場合
				HitEnemy(1, pEnemy);
				g_aBullet[nCntBullet].bUse = false;
				ResetShadow(g_aBullet[nCntBullet].nShadow);	//使用していた影を使用されていない状態にする
				SetParticle(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_MOVE, 5.0f);
			}
		}
	}
}

//===================================================
//弾の情報を取得
//===================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}

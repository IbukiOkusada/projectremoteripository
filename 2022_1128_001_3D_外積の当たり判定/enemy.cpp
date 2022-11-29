//==========================================================
//
//モデル描画処理
//Author 奥定伊吹
//
//==========================================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "explosion.h"
#include "particle.h"

//==========================================================
//プロトタイプ宣言
//==========================================================
void CollisonEnemy(void);
void GetSizeEnemy(int nCntEnemy);
void SetStateEnemy(int nCntEnemy);

//==========================================================
//グローバル変数宣言
//==========================================================
Enemy g_aEnemy[MAX_ENEMY];		//敵の情報

//==========================================================
//ファイル名
//==========================================================
const char *c_apEnemynameEnemy[] =
{
	"data\\MODEL\\streetlamp.x",
	"data\\MODEL\\woodbox000.x",
	"data\\MODEL\\bench002.x",
};

//==========================================================
//モデルの初期化処理
//==========================================================
void InitEnemy(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//デバイスへのポインタ

	//各種変数の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_aEnemy[nCntEnemy].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_aEnemy[nCntEnemy].nShadow = -1;
	}

	//敵の設定
	SetEnemy(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 0);
	SetEnemy(D3DXVECTOR3(-300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 1);
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 2);
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5, 2);
}

//==========================================================
//モデルの終了処理
//==========================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//メッシュの廃棄
		if (g_aEnemy[nCntEnemy].pMesh != NULL)
		{
			g_aEnemy[nCntEnemy].pMesh->Release();
			g_aEnemy[nCntEnemy].pMesh = NULL;
		}

		//マテリアルの廃棄
		if (g_aEnemy[nCntEnemy].pBuffMat != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMat->Release();
			g_aEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}

}

//==========================================================
//モデルの更新処理
//==========================================================
void UpdateEnemy(void)
{
	Camera *pCamera = GetCamera();

	//プレイヤーとの当たり判定
	CollisonEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている場合
			//影の位置設定
			SetPositionShadow(g_aEnemy[nCntEnemy].nShadow, g_aEnemy[nCntEnemy].pos);

			//状態設定
			SetStateEnemy(nCntEnemy);
		}
	}
}

//==========================================================
//モデルの描画処理
//==========================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aEnemy[nCntEnemy].apTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==========================================================
//モデルとプレイヤーの判定
//==========================================================
void CollisonEnemy(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報のポインタ
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている場合
			if (pPlayer->pos.z + pPlayer->vtxMax.z > g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z)
			{
				if (pPlayer->posOld.x + pPlayer->vtxMax.x <= g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x && pPlayer->pos.x + pPlayer->vtxMax.x > g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x)
				{//左からめり込んだ場合
					pPlayer->pos.x = g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x - pPlayer->vtxMax.x;
				}
				if (pPlayer->posOld.x + pPlayer->vtxMin.x >= g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x)
				{//右からめり込んだ場合
					pPlayer->pos.x = g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x - pPlayer->vtxMin.x;
				}
			}
			if (pPlayer->pos.x + pPlayer->vtxMax.x > g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x)
			{
				if (pPlayer->posOld.z + pPlayer->vtxMax.z <= g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z && pPlayer->pos.z + pPlayer->vtxMax.z > g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z)
				{//手前からめり込んだ場合
					pPlayer->pos.z = g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z - pPlayer->vtxMax.z;
				}
				if (pPlayer->posOld.z + pPlayer->vtxMin.z >= g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z)
				{//奥からめり込んだ場合
					pPlayer->pos.z = g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z - pPlayer->vtxMin.z;
				}
			}
		}
	}
}

//==========================================================
//敵の設定処理
//==========================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, int nType)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//デバイスへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されていない場合
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nType;
			g_aEnemy[nCntEnemy].nShadow = SetShadow();
			g_aEnemy[nCntEnemy].bUse = true;

			//Xファイルの読み込み
			D3DXLoadMeshFromX(c_apEnemynameEnemy[nType],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemy[nCntEnemy].pBuffMat,
				NULL,
				&g_aEnemy[nCntEnemy].dwNumMat,
				&g_aEnemy[nCntEnemy].pMesh);

			D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//テクスチャファイルが存在している
				 //テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aEnemy[nCntEnemy].apTexture[nCntMat]);
				}
			}

			//最大値と最小値を取得
			GetSizeEnemy(nCntEnemy);

			break;
		}
	}
}

//==========================================================
//敵の最大値と最小値の取得
//==========================================================
void GetSizeEnemy(int nCntEnemy)
{
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファのポインタ

	//頂点数を取得
	nNumVtx = g_aEnemy[nCntEnemy].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMesh->GetFVF());

	//頂点バッファをロック
	g_aEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		if (vtx.x < g_aEnemy[nCntEnemy].vtxMin.x)
		{//最小値よりも値が小さい場合
			g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;
		}
		else if (vtx.x > g_aEnemy[nCntEnemy].vtxMax.x)
		{//最大値よりも値が大きい場合
			g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;
		}

		if (vtx.z < g_aEnemy[nCntEnemy].vtxMin.z)
		{//最小値よりも値が小さい場合
			g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;
		}
		else if (vtx.z > g_aEnemy[nCntEnemy].vtxMax.z)
		{//最大値よりも値が大きい場合
			g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	g_aEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
}

//==========================================================
//敵の情報取得
//==========================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==========================================================
//敵のダメージ反映
//==========================================================
void HitEnemy(int nDamage, Enemy *pEnemy)
{
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタを取得
	pEnemy->nLife -= nDamage;

	if (pEnemy->nLife <= 0)
	{//体力がなくなった場合
		pEnemy->bUse = false;
		ResetShadow(pEnemy->nShadow);

		//爆発の設置
		SetExplosion(pEnemy->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50.0f);

		//パーティクルの設定
		SetParticle(pEnemy->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f);
	}
	else
	{
		D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pEnemy->pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)pEnemy->dwNumMat; nCntMat++)
		{
			pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		pEnemy->nStateCounter = 10;
		pEnemy->nState = ENEMYSTATE_DAMAGE;
	}
}

//==========================================================
//敵の状態設定
//==========================================================
void SetStateEnemy(int nCntEnemy)
{
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタを取得

	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	switch (g_aEnemy[nCntEnemy].nState)
	{
	case ENEMYSTATE_NORMAL:
		break;
	case ENEMYSTATE_DAMAGE:
		g_aEnemy[nCntEnemy].nStateCounter--;
		if (g_aEnemy[nCntEnemy].nStateCounter <= 0)
		{
			g_aEnemy[nCntEnemy].nStateCounter = 0;
			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_NORMAL;

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}

		break;
	case ENEMYSTATE_DEATH:
		break;
	}
}
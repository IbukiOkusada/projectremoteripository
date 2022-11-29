//==========================================================
//
//モデル描画処理
//Author 奥定伊吹
//
//==========================================================
#include "main.h"
#include "Model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "bullet.h"
#include "particle.h"
#include "effect.h"

//==========================================================
//マクロ定義
//==========================================================
#define MODELFILE_DATA	"data\\MODEL\\rock000.x"		//モデルデータ

//==========================================================
//プロトタイプ宣言
//==========================================================
void CollisonModel(void);
void CollisionModelBullet(void);

//==========================================================
//グローバル変数宣言
//==========================================================
LPD3DXMESH g_pMeshModel = NULL;			//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;	//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_apTextureModel[20] = {};	//テクスチャのポインタ
DWORD g_dwNumMatModel = 0;				//マテリアルの数
D3DXVECTOR3 g_posModel;					//位置
D3DXVECTOR3 g_rotModel;					//向き
D3DXMATRIX g_mtxWorldModel;				//ワールドマトリックス
D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;	//モデルの最小値と最大値
int g_nShadow = -1;

//==========================================================
//モデルの初期化処理
//==========================================================
void InitModel(void)
{
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファのポインタ

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//デバイスへのポインタ

	//各種変数の初期化
	g_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_vtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	//Xファイルの読み込み
	D3DXLoadMeshFromX(MODELFILE_DATA,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel);

	//頂点数を取得
	nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファをロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		if (vtx.x < g_vtxMinModel.x)
		{//最小値よりも値が小さい場合
			g_vtxMinModel.x = vtx.x;
		}
		else if (vtx.x > g_vtxMaxModel.x)
		{//最大値よりも値が大きい場合
			g_vtxMaxModel.x = vtx.x;
		}

		if (vtx.z < g_vtxMinModel.z)
		{//最小値よりも値が小さい場合
			g_vtxMinModel.z = vtx.z;
		}
		else if (vtx.z > g_vtxMaxModel.z)
		{//最大値よりも値が大きい場合
			g_vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	g_pMeshModel->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在している
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureModel[nCntMat]);
		}
	}

	//影の設定
	g_nShadow = SetShadow();
}

//==========================================================
//モデルの終了処理
//==========================================================
void UninitModel(void)
{
	//メッシュの廃棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの廃棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}

}

//==========================================================
//モデルの更新処理
//==========================================================
void UpdateModel(void)
{
	Camera *pCamera = GetCamera();

	//プレイヤーの更新
	/*if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_posModel.x -= cosf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z += sinf(pCamera->rot.y) * pCamera->moveV.z;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_posModel.x += cosf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z -= sinf(pCamera->rot.y) * pCamera->moveV.z;
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{
		g_posModel.x += sinf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z += cosf(pCamera->rot.y) * pCamera->moveV.z;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_posModel.x -= sinf(pCamera->rot.y) * pCamera->moveV.x;
		g_posModel.z -= cosf(pCamera->rot.y) * pCamera->moveV.z;
	}*/

	//影の位置設定
	//SetPositionShadow(g_nShadow, g_posModel);

	//プレイヤーとの当たり判定
	CollisonModel();

	//モデルと弾のあたり判定
	CollisionModelBullet();

}

//==========================================================
//モデルの描画処理
//==========================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModel);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================
//モデルとプレイヤーの判定
//==========================================================
void CollisonModel(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報のポインタ

	if (pPlayer->pos.z + pPlayer->vtxMax.z > g_posModel.z + g_vtxMinModel.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_posModel.z + g_vtxMaxModel.z)
	{
		if (pPlayer->posOld.x + pPlayer->vtxMax.x <= g_posModel.x + g_vtxMinModel.x && pPlayer->pos.x + pPlayer->vtxMax.x > g_posModel.x + g_vtxMinModel.x)
		{//左からめり込んだ場合
			pPlayer->pos.x = g_posModel.x + g_vtxMinModel.x - pPlayer->vtxMax.x;
		}
		if (pPlayer->posOld.x + pPlayer->vtxMin.x >= g_posModel.x + g_vtxMaxModel.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_posModel.x + g_vtxMaxModel.x)
		{//右からめり込んだ場合
			pPlayer->pos.x = g_posModel.x + g_vtxMaxModel.x - pPlayer->vtxMin.x;
		}
	}
	if (pPlayer->pos.x + pPlayer->vtxMax.x > g_posModel.x + g_vtxMinModel.x && pPlayer->pos.x + pPlayer->vtxMin.x < g_posModel.x + g_vtxMaxModel.x)
	{
		if (pPlayer->posOld.z + pPlayer->vtxMax.z <= g_posModel.z + g_vtxMinModel.z && pPlayer->pos.z + pPlayer->vtxMax.z > g_posModel.z + g_vtxMinModel.z)
		{//手前からめり込んだ場合
			pPlayer->pos.z = g_posModel.z + g_vtxMinModel.z - pPlayer->vtxMax.z;
		}
		if (pPlayer->posOld.z + pPlayer->vtxMin.z >= g_posModel.z + g_vtxMaxModel.z && pPlayer->pos.z + pPlayer->vtxMin.z < g_posModel.z + g_vtxMaxModel.z)
		{//奥からめり込んだ場合
			pPlayer->pos.z = g_posModel.z + g_vtxMaxModel.z - pPlayer->vtxMin.z;
		}
	}
}

//==========================================================
//モデルと弾の当たり判定
//==========================================================
void CollisionModelBullet(void)
{
	Bullet *pBullet = GetBullet();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{//敵が使用されている場合
			if (pBullet->pos.x + pBullet->fWidth >= g_posModel.x + g_vtxMinModel.x &&
				pBullet->pos.x - pBullet->fWidth <= g_posModel.x + g_vtxMaxModel.x &&
				pBullet->pos.z + pBullet->fWidth >= g_posModel.z + g_vtxMinModel.z &&
				pBullet->pos.z - pBullet->fWidth <= g_posModel.z + g_vtxMaxModel.z)
			{//敵の範囲内に入った場合
				pBullet->bUse = false;
				ResetShadow(pBullet->nShadow);	//使用していた影を使用されていない状態にする
				SetParticle(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_MOVE, 5.0f);
			}
		}
	}
}
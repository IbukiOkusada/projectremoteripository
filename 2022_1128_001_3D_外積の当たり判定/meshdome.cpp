//==========================================================
//
//メッシュドーム描画処理
//Author 奥定伊吹
//
//==========================================================
#include "main.h"
#include "meshdome.h"

//==========================================================
//マクロ定義
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\sky000.jpg"		//テクスチャデータ
#define MAX_FIELD		(28)			//フィールドの頂点数
#define FIELD_WIDTH		(16)			//フィールドの幅
#define FIELD_HEIGHT	(8)				//フィールドの高さ
#define FIELD_TYOUTEN	((FIELD_WIDTH + 1) * (FIELD_HEIGHT + 1))			//頂点数
#define FIELD_WIDTHWIDTH	(4000.0f)	//幅距離
#define FIELD_HEIGHTHEIGHT	(50.0f)	//奥行距離
#define MAX_WALLINDEX	(300)			//壁のインデックス最大数
#define MAX_WALLVERTEX	(300)			//壁の頂点最大数

//==========================================================
//グローバル変数宣言
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshDome = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshDome = NULL;		//インデックスバッファへのポインタ
D3DXMATRIX g_mtxWorldMeshDome;							//ワールドマトリックス
MeshDome g_MeshDome;	//メッシュシリンダーの情報

//==========================================================
//ポリゴンの初期化処理
//==========================================================
void InitMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXVECTOR3 vecDir;				//設定変更用ベクトル

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureMeshDome);

	//各種変数の初期化
	g_MeshDome.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshDome.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshDome.fLength = FIELD_WIDTHWIDTH;
	g_MeshDome.fRot = (D3DX_PI * 2) / FIELD_WIDTH;
	g_MeshDome.fHeightRot = (D3DX_PI * 0.5f) / (FIELD_HEIGHT + 2);
	g_MeshDome.nVertex = FIELD_TYOUTEN;
	g_MeshDome.nIndex = (FIELD_WIDTH + 1) * (FIELD_HEIGHT) * 2 + (FIELD_HEIGHT) * 2;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_WALLVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshDome,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshDome->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntpVtx = 0; nCntpVtx < FIELD_TYOUTEN; nCntpVtx++)
	{
		float fRot = g_MeshDome.fRot * (nCntpVtx % (FIELD_WIDTH + 1) );

		if (fRot > D3DX_PI || fRot < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (fRot > D3DX_PI)
			{
				fRot += (-D3DX_PI * 2);
			}
			else if (fRot < -D3DX_PI)
			{
				fRot += (D3DX_PI * 2);
			}
		}

		float fHeightRot = g_MeshDome.fHeightRot * (nCntpVtx / (FIELD_WIDTH + 1));

		if (fHeightRot > D3DX_PI || fHeightRot < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (fHeightRot > D3DX_PI)
			{
				fHeightRot += (-D3DX_PI * 2);
			}
			else if (fHeightRot < -D3DX_PI)
			{
				fHeightRot += (D3DX_PI * 2);
			}
		}

		//頂点座標
		pVtx[nCntpVtx].pos.x = (cosf(fHeightRot) * cosf(fRot)) * g_MeshDome.fLength;
		pVtx[nCntpVtx].pos.y = sinf(fHeightRot) * g_MeshDome.fLength;
		pVtx[nCntpVtx].pos.z = (cosf(fHeightRot) * sinf(fRot)) * g_MeshDome.fLength;

		vecDir = pVtx[nCntpVtx].pos - g_MeshDome.pos;

		//ベクトルを正規化する
		D3DXVec3Normalize(&vecDir, &vecDir);

		//法線
		pVtx[nCntpVtx].nor = vecDir;

		//色
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2((1.0f / FIELD_WIDTH) * (nCntpVtx % (FIELD_WIDTH + 1)), (1.0f / FIELD_HEIGHT) * (nCntpVtx / (FIELD_WIDTH + 1)));
	}

	/*pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.375f, 0.0f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.625f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.0f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.875f, 0.0f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 1.0f);*/

	//頂点バッファをアンロックする
	g_pVtxBuffMeshDome->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_WALLINDEX,	//必要なインデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshDome,
		NULL);

	WORD *pIdx;	//インデックス情報のポインタ

	//インデックスバッファをロックし頂点番号データへのポインタを取得
	g_pIdexBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < g_MeshDome.nIndex * 0.5f; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + FIELD_WIDTH + 1) - (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 2)) - (nCntIdx / (FIELD_WIDTH + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx + (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 1)) - (nCntIdx / (FIELD_WIDTH + 2));
	}

	//インデックスバッファをアンロックする
	g_pIdexBuffMeshDome->Unlock();

}

//==========================================================
//ポリゴンの終了処理
//==========================================================
void UninitMeshDome(void)
{
	//テクスチャの廃棄
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	//インデックスバッファの廃棄
	if (g_pIdexBuffMeshDome != NULL)
	{
		g_pIdexBuffMeshDome->Release();
		g_pIdexBuffMeshDome = NULL;
	}

}

//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpdateMeshDome(void)
{
	
}

//==========================================================
//ポリゴンの描画処理
//==========================================================
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	int nCntIndex = 0;

	//ライティングをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshDome.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshDome.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMeshDome,
		0,
		sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdexBuffMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_MeshDome.nVertex,		//用意した頂点数
		nCntIndex,
		g_MeshDome.nIndex - 2	//描画するプリミティブ数
	);

	//ライティングをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================
//メッシュウォールの設定
//==========================================================
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth)
{
	
}

//==========================================================
//メッシュウォールのインデックス番号設定
//==========================================================
void SetIndexMeshDome(int nCntWall)
{

}

//==========================================================
//メッシュウォールのpVtx設定
//==========================================================
void SetpVtxMeshDome(int nCntWall)
{
	
}
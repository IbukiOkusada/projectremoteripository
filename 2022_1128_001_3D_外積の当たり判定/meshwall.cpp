//==========================================================
//
//メッシュウォール描画処理
//Author 奥定伊吹
//
//==========================================================
#include "meshwall.h"
#include "input.h"

//==========================================================
//マクロ定義
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\field000.jpg"		//テクスチャデータ
#define MAX_FIELD		(28)			//フィールドの頂点数
#define FIELD_WIDTH		(3)				//フィールドの幅
#define FIELD_HEIGHT	(3)				//フィールドの高さ
#define FIELD_TYOUTEN	(4 * 4)			//超点数
#define FIELD_WIDTHWIDTH	(100.0f)	//幅距離
#define FIELD_HEIGHTHEIGHT	(100.0f)	//奥行距離
#define MAX_WALLINDEX	(2048)			//壁のインデックス最大数
#define MAX_WALLVERTEX	(256)			//壁の頂点最大数

//==========================================================
//グローバル変数宣言
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshWall = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshWall;								//位置
D3DXVECTOR3 g_rotMeshWall;								//向き
D3DXMATRIX g_mtxWorldMeshWall;							//ワールドマトリックス
int g_nVertex; //頂点の使用数確認
int g_nIndex;	//インデックスの仕様数確認
MeshWall g_aMeshWall[MAX_MESHWALL];	//メッシュウォールの情報

//==========================================================
//ポリゴンの初期化処理
//==========================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureMeshWall);

	//各種変数の初期化
	g_posMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		g_aMeshWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].fHeight = 0.0f;
		g_aMeshWall[nCntWall].fWidth =  0.0f;
		g_aMeshWall[nCntWall].nHeightMesh = 0;
		g_aMeshWall[nCntWall].nWidthMesh = 0;
		g_aMeshWall[nCntWall].nVertex = 0;
		g_aMeshWall[nCntWall].bUse = false;
	}

	g_nVertex = 0;
	g_nIndex = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_WALLVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	//VERTEX_3D *pVtx;

	////頂点バッファをロックし頂点情報へのポインタを取得
	//g_pVtxBuffMeshWall->Lock(
	//	0,
	//	0,
	//	(void**)&pVtx,
	//	0);

	////頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	//for (int nCntpVtx = 0; nCntpVtx < FIELD_TYOUTEN; nCntpVtx++)
	//{
	//	//頂点座標
	//	pVtx[nCntpVtx].pos.x = -(FIELD_WIDTHWIDTH * FIELD_WIDTH) + (nCntpVtx % (FIELD_WIDTH + 1) * (FIELD_WIDTHWIDTH * 2));
	//	pVtx[nCntpVtx].pos.y = ((FIELD_HEIGHTHEIGHT * 2) * FIELD_HEIGHT) + ((nCntpVtx / (FIELD_HEIGHT + 1) * (-FIELD_HEIGHTHEIGHT * 2)));
	//	pVtx[nCntpVtx].pos.z = 0.0f;

	//	//法線
	//	pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//	//色
	//	pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//	pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (FIELD_WIDTH + 1)), 1.0f * (nCntpVtx / (FIELD_HEIGHT + 1)));
	//}

	////頂点バッファをアンロックする
	//g_pVtxBuffMeshWall->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_WALLINDEX,	//必要なインデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshWall,
		NULL);

	//WORD *pIdx;	//インデックス情報のポインタ

	////インデックスバッファをロックし頂点番号データへのポインタを取得
	//g_pIdexBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	//for (int nCntIdx = 0; nCntIdx < MAX_FIELD / 2; nCntIdx++)
	//{
	//	pIdx[nCntIdx * 2] = (nCntIdx + FIELD_WIDTH + 1) - (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 2)) - (nCntIdx / (FIELD_WIDTH + 2));
	//	pIdx[nCntIdx * 2 + 1] = nCntIdx + (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 1)) - (nCntIdx / (FIELD_WIDTH + 2));
	//}

	////インデックスバッファをアンロックする
	//g_pIdexBuffMeshWall->Unlock();

	
	//壁の設定
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -1800.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f), 1200.0f, 1800.0f, 1, 1);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, 1500.0f), D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f), 200.0f, 200.0f, 3, 3);
	SetMeshWall(D3DXVECTOR3(-1800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f), 100.0f, 300.0f,2, 6);
	SetMeshWall(D3DXVECTOR3(1800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f), 100.0f, 600.0f, 4, 3);

}

//==========================================================
//ポリゴンの終了処理
//==========================================================
void UninitMeshWall(void)
{
	//テクスチャの廃棄
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//インデックスバッファの廃棄
	if (g_pIdexBuffMeshWall != NULL)
	{
		g_pIdexBuffMeshWall->Release();
		g_pIdexBuffMeshWall = NULL;
	}

}

//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpdateMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1キーが押されたとき
		
	}
}

//==========================================================
//ポリゴンの描画処理
//==========================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	int nCntIndex = 0;

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//使用されているとき
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshWall[nCntWall].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntWall].rot.y, g_aMeshWall[nCntWall].rot.x, g_aMeshWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntWall].pos.x, g_aMeshWall[nCntWall].pos.y, g_aMeshWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntWall].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffMeshWall,
				0,
				sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdexBuffMeshWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshWall[nCntWall].nVertex,		//用意した頂点数
				nCntIndex,
				g_aMeshWall[nCntWall].nIndex - 2	//描画するプリミティブ数
			);

			nCntIndex += g_aMeshWall[nCntWall].nIndex;
		}
	}
}

//==========================================================
//メッシュウォールの設定
//==========================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth)
{
	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == false)
		{//使用されていない場合
			g_aMeshWall[nCntWall].pos = pos;
			g_aMeshWall[nCntWall].rot = rot;
			g_aMeshWall[nCntWall].fHeight = fHeight;
			g_aMeshWall[nCntWall].fWidth = fWidth;
			g_aMeshWall[nCntWall].nHeightMesh = nHeight;
			g_aMeshWall[nCntWall].nWidthMesh = nWidth;
			g_aMeshWall[nCntWall].nVertex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh + 1);
			g_aMeshWall[nCntWall].bUse = true;
			g_aMeshWall[nCntWall].nIndex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh) * 2 + (g_aMeshWall[nCntWall].nHeightMesh - 1) * 2;

			//インデックスの設定
			SetIndexMeshWall(nCntWall);

			//頂点座標の設定
			SetpVtxMeshWall(nCntWall);

			break;
		}
	}
}

//==========================================================
//メッシュウォールのインデックス番号設定
//==========================================================
void SetIndexMeshWall(int nCntWall)
{
	WORD *pIdx;	//インデックス情報のポインタ

	//インデックスバッファをロックし頂点番号データへのポインタを取得
	g_pIdexBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	pIdx += g_nIndex;

	for (int nCntIdx = 0; nCntIdx < g_aMeshWall[nCntWall].nIndex / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + g_aMeshWall[nCntWall].nWidthMesh + 1)
			- (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 2)) 
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx
			+ (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 1))
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));
		pIdx[nCntIdx * 2] += g_nVertex;
		pIdx[nCntIdx * 2 + 1] += g_nVertex;
	}

	//インデックスバッファをアンロックする
	g_pIdexBuffMeshWall->Unlock();

	g_nIndex += g_aMeshWall[nCntWall].nIndex;
}

//==========================================================
//メッシュウォールのpVtx設定
//==========================================================
void SetpVtxMeshWall(int nCntWall)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshWall->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += g_nVertex;

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntpVtx = 0; nCntpVtx < g_aMeshWall[nCntWall].nVertex; nCntpVtx++)
	{
		//頂点座標
		pVtx[nCntpVtx].pos.x = -(g_aMeshWall[nCntWall].fWidth * g_aMeshWall[nCntWall].nWidthMesh) + (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].fWidth * 2));
		pVtx[nCntpVtx].pos.y = ((g_aMeshWall[nCntWall].fHeight * 2) * g_aMeshWall[nCntWall].nHeightMesh) + ((nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (-g_aMeshWall[nCntWall].fHeight * 2)));
		pVtx[nCntpVtx].pos.z = 0.0f;

		//法線
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//色
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1)), 1.0f * (nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1)));
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshWall->Unlock();

	g_nVertex += g_aMeshWall[nCntWall].nVertex;
}

//==========================================================
//メッシュウォールの情報取得
//==========================================================
MeshWall *GetMeshWall(void)
{
	return &g_aMeshWall[0];
}
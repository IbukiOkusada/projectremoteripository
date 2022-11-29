//==========================================================
//
//メッシュフィールド描画処理
//Author 奥定伊吹
//
//==========================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"

//==========================================================
//マクロ定義
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\field000.jpg"		//テクスチャデータ
#define MAX_FIELD		(28)	//フィールドの頂点数
#define FIELD_WIDTH		(3)		//フィールドの幅
#define FIELD_HEIGHT	(3)		//フィールドの高さ
#define FIELD_TYOUTEN	(4 * 4)	//超点数
#define FIELD_WIDTHWIDTH	(600.0f)	//幅距離
#define FIELD_HEIGHTHEIGHT	(600.0f)	//奥行距離

//==========================================================
//グローバル変数宣言
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshField;								//位置
D3DXVECTOR3 g_rotMeshField;								//向き
D3DXMATRIX g_mtxWorldMeshField;							//ワールドマトリックス

//==========================================================
//ポリゴンの初期化処理
//==========================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureMeshField);

	//各種変数の初期化
	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * FIELD_TYOUTEN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;
	VERTEX_3D aVtx[FIELD_TYOUTEN];

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntpVtx = 0; nCntpVtx < FIELD_TYOUTEN; nCntpVtx++)
	{
		//頂点座標
		pVtx[nCntpVtx].pos.x = -(FIELD_WIDTHWIDTH * FIELD_WIDTH) + (nCntpVtx % (FIELD_WIDTH + 1) * (FIELD_WIDTHWIDTH * 2));
		pVtx[nCntpVtx].pos.y = 0.0f;
		pVtx[nCntpVtx].pos.z = (FIELD_HEIGHTHEIGHT * FIELD_HEIGHT) + ((nCntpVtx / (FIELD_HEIGHT + 1) * (-FIELD_HEIGHTHEIGHT * 2)));

		//法線
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//色
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (FIELD_WIDTH + 1)), 1.0f * (nCntpVtx / (FIELD_HEIGHT + 1)));
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_FIELD,	//必要なインデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshField,
		NULL);

	WORD *pIdx;	//インデックス情報のポインタ

	//インデックスバッファをロックし頂点番号データへのポインタを取得
	g_pIdexBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MAX_FIELD / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + FIELD_WIDTH + 1) - (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 2)) - (nCntIdx / (FIELD_WIDTH + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx + (nCntIdx % (FIELD_WIDTH + 2) / (FIELD_WIDTH + 1) * (FIELD_WIDTH + 1)) - (nCntIdx / (FIELD_WIDTH + 2));
	}

	//インデックスバッファをアンロックする
	g_pIdexBuffMeshField->Unlock();
}

//==========================================================
//ポリゴンの終了処理
//==========================================================
void UninitMeshField(void)
{
	//テクスチャの廃棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの廃棄
	if (g_pIdexBuffMeshField != NULL)
	{
		g_pIdexBuffMeshField->Release();
		g_pIdexBuffMeshField = NULL;
	}

}

//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpdateMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1キーが押されたとき
		
	}
}

//==========================================================
//ポリゴンの描画処理
//==========================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMeshField,
		0,
		sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdexBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);


	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,		//用意した超点数
		0,
		26	//描画するプリミティブ数
	);
}
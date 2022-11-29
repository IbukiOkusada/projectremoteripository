//==========================================================
//
//壁描画処理
//Author 奥定伊吹
//
//==========================================================
#include "main.h"
#include "wall.h"
#include "input.h"

//==========================================================
//マクロ定義
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\block000.jpg"		//テクスチャデータ

//==========================================================
//プロトタイプ宣言
//==========================================================
void pVtxWall(int nCntWall);

//==========================================================
//グローバル変数宣言
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posWall;								//位置
D3DXVECTOR3 g_rotWall;								//向き
D3DXMATRIX g_mtxWorldWall;							//ワールドマトリックス
Wall g_aWall[MAX_WALL];								//壁の情報
int nSelectWall = 0;

//==========================================================
//壁の初期化処理
//==========================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureWall);

	//各種変数の初期化
	g_posWall = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	g_rotWall = D3DXVECTOR3(0.0f, 1.57f, 0.0f);

	//構造体の各情報初期化
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_aWall[nCntWall].fWidth = 0.0f;						//幅
		g_aWall[nCntWall].fHeight = 0.0f;						//高さ
		g_aWall[nCntWall].bUse = false;							//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//壁の設定
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.3f * D3DX_PI, 0.0f), 150.0f, 100.0f);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f), 150.0f, 100.0f);
	SetWall(D3DXVECTOR3(-650.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.5f * -D3DX_PI, 0.0f), 150.0f, 100.0f);
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f), 150.0f, 100.0f);

	//pVtxの設定
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		pVtxWall(nCntWall);
	}
}

//==========================================================
//壁の終了処理
//==========================================================
void UninitWall(void)
{
	//テクスチャの廃棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//==========================================================
//壁の更新処理
//==========================================================
void UpdateWall(void)
{
	/*if (GetKeyboardPress(DIK_0))
	{
		g_aWall[nSelectWall].rot.y += 0.01f;

		if (g_aWall[nSelectWall].rot.y > D3DX_PI)
		{
			g_aWall[nSelectWall].rot.y *= -1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_9))
	{
		g_aWall[nSelectWall].rot.y -= 0.01f;

		if (g_aWall[nSelectWall].rot.y < -D3DX_PI)
		{
			g_aWall[nSelectWall].rot.y *= -1.0f;
		}
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		nSelectWall = (nSelectWall + 1) % MAX_WALL;
	}*/
}

//==========================================================
//壁の描画処理
//==========================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{//使用されている場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffWall,
				0,
				sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntWall * 4,
				2	//頂点情報構造体のサイズ
			);
		}
	}
}

//==========================================================
//壁の設定処理
//==========================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{//使用されていない場合
			g_aWall[nCntWall].pos = pos;			//位置
			g_aWall[nCntWall].rot = rot;			//向き
			g_aWall[nCntWall].fWidth = fWidth;		//幅
			g_aWall[nCntWall].fHeight = fHeight;	//高さ
			g_aWall[nCntWall].bUse = true;			//使用している状態にする

			//pVtxの設定
			pVtxWall(nCntWall);

			break;
		}
	}
}

//==========================================================
//pVtx設定
//==========================================================
void pVtxWall(int nCntWall)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += nCntWall * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, 0.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	if (nCntWall == 4)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//壁の情報を取得
Wall *GetWall(void)
{
	return &g_aWall[0];
}
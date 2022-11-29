//==========================================================
//
//ビルボード描画処理
//Author 奥定伊吹
//
//==========================================================
#include "billboard.h"
#include "input.h"
#include "camera.h"

//==========================================================
//マクロ定義
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\billboard.png"		//テクスチャデータ
#define MAX_BILLBOARD	(4)
#define BILLBOARD_MOVE	(1.0f)

//==========================================================
//プロトタイプ宣言
//==========================================================
void pVtxBillboard(int nCntBillboard);
void MoveBillboard(void);

//==========================================================
//グローバル変数宣言
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//頂点バッファへのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];					//ビルボードの情報

//==========================================================
//ビルボードの初期化処理
//==========================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE_DATA, &g_pTextureBillboard);

	//構造体の各情報初期化
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aBillboard[nCntBillboard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aBillboard[nCntBillboard].fWidth = 0.0f;						//幅
		g_aBillboard[nCntBillboard].fHeight = 0.0f;						//高さ
		g_aBillboard[nCntBillboard].bUse = false;						//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//ビルボードの設定
	SetBillboard(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f, 50.0f);
	SetBillboard(D3DXVECTOR3(50.0f, 50.0f, 0.0f), 50.0f, 50.0f);

	//pVtxの設定
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		pVtxBillboard(nCntBillboard);
	}
}

//==========================================================
//ビルボードの終了処理
//==========================================================
void UninitBillboard(void)
{
	//テクスチャの廃棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//==========================================================
//ビルボードの更新処理
//==========================================================
void UpdateBillboard(void)
{
	//移動
	MoveBillboard();
}

//==========================================================
//ビルボードの移動処理
//==========================================================
void MoveBillboard(void)
{
	Camera *pCamera = GetCamera();

	if (GetKeyboardPress(DIK_J) == true)
	{//Jキーが押されたとき
		if (GetKeyboardPress(DIK_I) == true)
		{//Iキーが押されたとき
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (-D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (-D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//Kキーが押されたとき
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (-D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (-D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
		}
		else
		{
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (-D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (-D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{
		if (GetKeyboardPress(DIK_I) == true)
		{//Iキーが押されたとき
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (D3DX_PI * 0.75f)) * BILLBOARD_MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//Kキーが押されたとき
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (D3DX_PI * 0.25f)) * BILLBOARD_MOVE;
		}
		else
		{
			g_aBillboard[0].move.x += cosf(pCamera->rot.x + (D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
			g_aBillboard[0].move.z += sinf(pCamera->rot.x + (D3DX_PI * 0.5f)) * BILLBOARD_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_I) == true)
	{//Iキーが押されたとき
		g_aBillboard[0].pos.x += -cosf(pCamera->rot.x) * BILLBOARD_MOVE;
		g_aBillboard[0].pos.z += -sinf(pCamera->rot.x) * BILLBOARD_MOVE;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//Kキーが押されたとき
		g_aBillboard[0].pos.x += cosf(pCamera->rot.x) * BILLBOARD_MOVE;
		g_aBillboard[0].pos.z += sinf(pCamera->rot.x) * BILLBOARD_MOVE;
	}

	g_aBillboard[0].pos.x += g_aBillboard[0].move.x;
	g_aBillboard[0].pos.z += g_aBillboard[0].move.z;

	g_aBillboard[0].move.x = 0.0f;
	g_aBillboard[0].move.z = 0.0f;
}

//==========================================================
//ビルボードの描画処理
//==========================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);


	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{//使用されている場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBillboard[nCntBillboard].mtxWorld, NULL, &mtxView);
			g_aBillboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._43 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._21 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._23 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._24 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffBillboard,
				0,
				sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntBillboard * 4,
				2	//頂点情報構造体のサイズ
			);
		}
	}


	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//==========================================================
//ビルボードの設定処理
//==========================================================
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{//使用されていない場合
			g_aBillboard[nCntBillboard].pos = pos;			//位置
			g_aBillboard[nCntBillboard].fWidth = fWidth;	//幅
			g_aBillboard[nCntBillboard].fHeight = fHeight;	//高さ
			g_aBillboard[nCntBillboard].bUse = true;		//使用している状態にする

			//pVtxの設定
			pVtxBillboard(nCntBillboard);

			break;
		}
	}
}

//==========================================================
//pVtx設定
//==========================================================
void pVtxBillboard(int nCntBillboard)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += nCntBillboard * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth, +g_aBillboard[nCntBillboard].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_aBillboard[nCntBillboard].fWidth, +g_aBillboard[nCntBillboard].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth, -g_aBillboard[nCntBillboard].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_aBillboard[nCntBillboard].fWidth, -g_aBillboard[nCntBillboard].fHeight, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}
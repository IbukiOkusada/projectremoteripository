//==========================================================
//
//メイン処理
//Author 奥定伊吹
//
//==========================================================
#include "main.h"
#include "input.h"
#include "stdio.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "debugproc.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "model.h"
#include "enemy.h"


//マクロ定義
#define WINDOW_NAME	"3D"			//ウインドウの名前
#define PCTIME		(1000/60)		//パソコンの時間
#define MAX_STRING	(512)			//文字の最大数
#define CLASS_NAME	"WindowClass"	//ウインドウクラスの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);
void DrawFPS(LPD3DXFONT font);
void DrawCamera(LPD3DXFONT font);
void DrawManual(LPD3DXFONT font);
void DrawLight(LPD3DXFONT font);
void DrawBullet(LPD3DXFONT font);

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;	//Direct3Dオブジェクトのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3Dデバイスへのポインタ
int g_nCountFPS = 0;		//FPSカウンタ
MODE g_mode = MODE_TITLE;	//モード
int g_bWire = 0;		//ワイヤーフレーム表示のON/OFF
int g_nZtest = 0;		//ZテストのON/OFF
int g_nAlphatest = 1;		//アルファテストのON/OFF

//================================================
//メイン関数
//================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						//ウインドウのスタイル
		WindowProc,						//ウインドウプロシージャ
		0,								//0にする(通常は使用しない)
		0,								//0にする(通常は使用しない)
		hInstance,						//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		//クライアント領域の背景色
		NULL,							//メニューバー
		CLASS_NAME,						//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	//ファイルのアイコン
	};

	HWND hWnd;	//ウインドウハンドル
	MSG msg;	//メッセージを格納する変数

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント形成を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx
	(
		0,								//拡張ウインドウスタイル
		CLASS_NAME,						//ウインドウクラスの名前
		WINDOW_NAME,					//ウインドウの名前
		WS_OVERLAPPEDWINDOW,			//ウインドウスタイル
		CW_USEDEFAULT,					//ウインドウの左上x座標
		CW_USEDEFAULT,					//ウインドウの左上y座標
		(rect.right - rect.left),		//ウインドウの幅
		(rect.bottom - rect.top),		//ウインドウの高さ
		NULL,							//親ウインドウのハンドル
		NULL,							//メニューハンドルまたは子ウインドウID
		hInstance,						//インスタンスハンドル
		NULL							//ウインドウ作成データ
	);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;	//初期化
	dwExecLastTime = timeGetTime();	//現在時刻を取得(保存)
	dwFrameCount = 0;	//初期化
	dwFPSLastTime = timeGetTime();	//現在時刻を取得(保存)

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();	//現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	//FPSを測定した時刻を保存
				dwFrameCount = 0;	//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻[現在時刻]を保存

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;	//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//================================================
//ウインドウプロシージャ
//================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;					//返り値を格納


	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//ウインドウの領域(形)

	switch (uMsg)
	{
	case WM_DESTROY:		//ウィンドウ廃棄のメッセージ

							//WM_QUETメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:		//キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE:

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウインドウを廃棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_COMMAND:	//コマンド発行メッセージ
		break;
	case WM_CLOSE:		//閉じるボタン押下
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{//はいを選択したとき
		 //ウインドウを廃棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{//いいえを選択したとき
			return 0;	//0を返さないと終了してしまう
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//既定の処理を返す
}

//================================================
//初期化処理
//================================================
LRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//DirectX3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						//バックバッファーの形式
	d3dpp.BackBufferCount = 1;									//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;						//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの作成
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	
	//テクスチャステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//マウスの初期化
	if (FAILED(InitMouse(hWnd)))
	{
		return E_FAIL;
	}

	//ゲームパッドの初期化処理
	InitGamepad();

	//カメラの初期化
	InitCamera();

	//影の初期化処理
	InitShadow();

	//ライトの初期化
	InitLight();

	//ポリゴンの初期化処理
	//InitPolygon();

	//メッシュフィールドの初期化処理
	InitMeshField();

	//壁の初期化処理
	InitWall();

	//メッシュウォールの初期化処理
	InitMeshWall();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//メッシュドームの初期化処理
	InitMeshDome();

	//ビルボードの初期化
	InitBillboard();

	//弾の初期化
	InitBullet();

	//プレイヤーの初期化処理
	InitPlayer();

	//モデルの初期化処理
	InitModel();

	//敵の初期化処理
	InitEnemy();

	//爆発の初期化
	InitExplosion();

	//エフェクトの初期化
	InitEffect();

	//パーティクルの初期化
	InitParticle();

	//デバッグ表示の初期化
	InitDebugProc();

	return S_OK;
}

//================================================
//終了処理
//================================================
void Uninit(void)
{

	//キーボードの終了処理
	UninitKeyboard();

	//マウスの終了処理
	UninitMouse();
	
	//ゲームパッドの終了処理
	UninitGamepad();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//ポリゴンの終了処理
	//UninitPolygon();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//壁の終了処理
	UninitWall();

	//メッシュウォールの終了処理
	UninitMeshWall();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュドームの終了処理
	UninitMeshDome();

	//ビルボードの終了処理
	UninitBillboard();

	//弾の終了処理
	UninitBullet();

	//プレイヤーの終了処理
	UninitPlayer();

	//モデルの終了処理
	UninitModel();

	//敵の終了処理
	UninitEnemy();

	//影の終了処理
	UninitShadow();

	//爆発の終了処理
	UninitExplosion();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//デバッグ表示の終了処理
	UninitDebugProc();

	//Direct3Dデバイスの廃棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの廃棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

//================================================
//更新処理
//================================================
void Update(void)
{
	//デバッグ表示の更新処理
	UpdateDebugProc();

	//キーボードの更新処理
	UpdateKeyboard();

	//マウスの更新処理
	UpdateMouse();

	//ゲームパッドの更新処理
	UpdateGamepad();

	//カメラの更新
	UpdateCamera();

	//ライトの更新
	UpdateLight();

	//ポリゴンの更新処理
	//UpdatePolygon();

	//メッシュフィールドの更新処理
	UpdateMeshField();

	//壁の更新処理
	UpdateWall();

	//メッシュウォールの更新処理
	UpdateMeshWall();

	//メッシュシリンダーの更新処理
	UpdateMeshCylinder();

	//メッシュドームの更新処理
	UpdateMeshDome();

	//ビルボードの更新処理
	UpdateBillboard();

	//プレイヤーの更新処理
	UpdatePlayer();

	//モデルの更新処理
	UpdateModel();

	//敵の更新処理
	UpdateEnemy();

	//弾の更新処理
	UpdateBullet();

	//影の更新処理
	UpdateShadow();

	//爆発の更新処理
	UpdateExplosion();

	//エフェクトの更新処理
	UpdateEffect();

	//パーティクルの更新処理
	UpdateParticle();

	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8キー入力されたとき
		g_bWire = g_bWire ^ 1;

		//描画モードの変更
		switch (g_bWire)
		{
		case 1:
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
		case 0:
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{
		g_nAlphatest ^= 1;		//弾のアルファテストの切り替え
	}
	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		g_nZtest ^= 1;			//爆発のZテストの切り替え
	}

}

//================================================
//描画処理
//================================================
void Draw(void)
{
	//画面クリア
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画が成功した場合

		//カメラの設定
		SetCamera();

		//ポリゴンの描画
		//DrawPolygon();

		//メッシュフィールドの描画処理
		DrawMeshField();

		//ビルボードの描画処理
		DrawBillboard();

		//プレイヤーの描画処理
		DrawPlayer();

		//モデルの描画処理
		DrawModel();

		//敵の描画処理
		DrawEnemy();

		//弾の描画処理
		DrawBullet();

		//影の描画処理
		DrawShadow();

		//壁の描画
		DrawWall();

		//メッシュウォールの描画
		DrawMeshWall();

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュドームの描画処理
		DrawMeshDome();

		//爆発の描画
		DrawExplosion();

		//エフェクトの描画
		DrawEffect();

		//デバッグ表示の描画
		DrawDebugProc();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//================================================
//デバイスの取得
//================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//================================================
//FPSの取得
//================================================
int GetFPS(void)
{
	return g_nCountFPS;
}

//================================================
//モードの設定
//================================================
void SetMode(MODE mode)
{
	

}

//================================================
//モードの取得
//================================================
MODE GetMode(void)
{
	return g_mode;
}

//================================================
//Zテスト状態の取得
//================================================
int GetZtest(void)
{
	return g_nZtest;
}

//================================================
//αテスト状態の取得
//================================================
int GetAlphaTest(void)
{
	return g_nAlphatest;
}

//================================================
//描画状態の取得
//================================================
int GetWireframe(void)
{
	return g_bWire;
}
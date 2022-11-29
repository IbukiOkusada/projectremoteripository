//==========================================================
//
//ライト処理
//Author 奥定伊吹
//
//==========================================================
#include "light.h"
#include "input.h"

//==========================================================
//グローバル変数
//==========================================================
void ChangeColLight(void);
void MoveLight(void);

//==========================================================
//グローバル変数
//==========================================================
D3DLIGHT9 g_alight[NUM_LIGHT];		//ライトの情報
int g_nSelectLight;	//選択中ライト

//==========================================================
//ライトの初期化処理
//==========================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得
	D3DXVECTOR3 vecDir;		//設定変更用ベクトル

	g_nSelectLight = 0;

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		switch (nCntLight)
		{
		case 0:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			break;
		case 2:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
			break;
		}

		//ライトの方向を設定
		switch (nCntLight)
		{
		case 0:
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;
		case 1:
			vecDir = D3DXVECTOR3(0.8f, 0.23f, -0.55f);
			break;
		case 2:
			vecDir = D3DXVECTOR3(-0.89f, -0.11f, 0.44f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトルを正規化する
		g_alight[nCntLight].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		//ライトを有効化する
		pDevice->LightEnable(nCntLight, TRUE);


	}
}

//==========================================================
//ライトの終了処理
//==========================================================
void UninitLight(void)
{

}

//==========================================================
//ライトの更新処理
//==========================================================
void UpdateLight(void)
{

	//番号変更
	if (GetKeyboardTrigger(DIK_L) == true)
	{//Lキー押されたとき
		//番号変更
		g_nSelectLight = (g_nSelectLight + 1) % NUM_LIGHT;
	}

	//色の変更
	ChangeColLight();

	//移動
	MoveLight();


}

//==========================================================
//ライトの色変更
//==========================================================
void ChangeColLight(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	//赤の変更
	if (GetKeyboardTrigger(DIK_1) == true || GetKeyboardRepeat(DIK_1) == true)
	{//1キーが押されたとき
		g_alight[g_nSelectLight].Diffuse.r -= 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.r < 0.0f)
		{//色が限界を超えたとき
			g_alight[g_nSelectLight].Diffuse.r = 1.0f;
		}
	}
	else if(GetKeyboardTrigger(DIK_2) == true || GetKeyboardRepeat(DIK_2) == true)
	{//2キーが押されたとき
		g_alight[g_nSelectLight].Diffuse.r += 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.r > 1.0f)
		{//色が限界を超えたとき
			g_alight[g_nSelectLight].Diffuse.r = 0.0f;
		}
	}

	//緑の変更
	if (GetKeyboardTrigger(DIK_3) == true || GetKeyboardRepeat(DIK_3) == true)
	{//3キーが押されたとき
		g_alight[g_nSelectLight].Diffuse.g -= 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.g < 0.0f)
		{//色が限界を超えたとき
			g_alight[g_nSelectLight].Diffuse.g = 1.0f;
		}
	}
	else if (GetKeyboardTrigger(DIK_4) == true || GetKeyboardRepeat(DIK_4) == true)
	{//4キーが押されたとき
		g_alight[g_nSelectLight].Diffuse.g += 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.g > 1.0f)
		{//色が限界を超えたとき
			g_alight[g_nSelectLight].Diffuse.g = 0.0f;
		}
	}

	//青の変更
	if (GetKeyboardTrigger(DIK_5) == true || GetKeyboardRepeat(DIK_5) == true)
	{//5キーが押されたとき
		g_alight[g_nSelectLight].Diffuse.b -= 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.b < 0.0f)
		{//色が限界を超えたとき
			g_alight[g_nSelectLight].Diffuse.b = 1.0f;
		}
	}
	else if (GetKeyboardTrigger(DIK_6) == true || GetKeyboardRepeat(DIK_6) == true)
	{//6キーが押されたとき
		g_alight[g_nSelectLight].Diffuse.b += 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.b > 1.0f)
		{//色が限界を超えたとき
			g_alight[g_nSelectLight].Diffuse.b = 0.0f;
		}
	}

	//ライトを設定する
	pDevice->SetLight(g_nSelectLight, &g_alight[g_nSelectLight]);
}

//==========================================================
//ライトの移動処理
//==========================================================
void MoveLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得
	D3DXVECTOR3 vecDir;		//設定変更用ベクトル

	//ベクトルを確保
	vecDir = g_alight[g_nSelectLight].Direction;

	////ベクトルの変更
	//if (GetKeyboardTrigger(DIK_I) == true || GetKeyboardRepeat(DIK_I) == true)
	//{//1キーが押されたとき
	//	vecDir.x -= 0.01f;
	//}
	//else if (GetKeyboardTrigger(DIK_J) == true || GetKeyboardRepeat(DIK_J) == true)
	//{//2キーが押されたとき
	//	vecDir.x += 0.01f;
	//}
	//else if (GetKeyboardTrigger(DIK_T) == true || GetKeyboardRepeat(DIK_T) == true)
	//{//2キーが押されたとき
	//	vecDir.y += 0.01f;
	//}
	//else if (GetKeyboardTrigger(DIK_L) == true || GetKeyboardRepeat(DIK_L) == true)
	//{//2キーが押されたとき
	//	vecDir.y -= 0.01f;
	//}

	D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトルを正規化する
	g_alight[g_nSelectLight].Direction = vecDir;
}

//==========================================================
//ライトの取得
//==========================================================
D3DLIGHT9 *GetLight(void)
{
	return &g_alight[0];
}

//==========================================================
//選択中ライト番号取得
//==========================================================
int GetSelectLight(void)
{
	return g_nSelectLight;
}

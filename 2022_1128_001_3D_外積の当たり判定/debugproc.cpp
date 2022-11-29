//==========================================================
//
//デバッグ表示処理
//Author 奥定伊吹
//
//==========================================================
#include "main.h"
#include "debugproc.h"
#include <string.h>
#include <stdio.h>
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_DEBUGSTRING	(2048)			//デバッグ表示の最大文字数
#define MAX_FLOATNUM	(2)				//小数点以下の最大表示数

//==========================================================
//グローバル変数
//==========================================================
LPD3DXFONT g_pFont = NULL;	//フォントへのポインタ
char g_aStrDebug[MAX_DEBUGSTRING];	//デバッグ表示用の文字列
bool g_bDispDebug = true;	//デバッグ表示のON/OFF

//==========================================================
//プロトタイプ宣言
//==========================================================
void SetManual(void);

//==========================================================
//デバッグONOFF表示メッセージ
//==========================================================
const char *c_apOnOff[DEBUGTYPE_MAX] =
{
	"OFF",
	"ON",
};

//==========================================================
//デバッグ表示の初期化処理
//==========================================================
void InitDebugProc(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	//デバッグ表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//デバッグ表示情報のクリア
	memset(&g_aStrDebug[0], NULL, sizeof(g_aStrDebug));
}

//==========================================================
//デバッグ表示の終了処理
//==========================================================
void UninitDebugProc(void)
{
	//デバッグ表示用フォントの廃棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//==========================================================
//デバッグ表示の更新処理
//==========================================================
void UpdateDebugProc(void)
{
	//操作説明設定
	SetManual();

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1キーが押されたとき
		g_bDispDebug = g_bDispDebug ? false : true;
	}
}

//==========================================================
//デバッグ表示の描画処理
//==========================================================
void DrawDebugProc(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (g_bDispDebug == true)
	{//デバックモードがオンの時
		//テキストの描画
		g_pFont->DrawText(NULL, &g_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	//デバッグ表示情報のクリア
	memset(&g_aStrDebug[0], NULL, sizeof(g_aStrDebug));
}

//==========================================================
//デバッグ表示の設定処理
//==========================================================
void PrintDebugProc(const char *fmt, ...)
{
	va_list args;
	char aString[MAX_DEBUGSTRING];
	char aSaveString[MAX_DEBUGSTRING];
	int nLength;
	int nStopLength;

	//文字列の代入
	strcpy(&aString[0], fmt);

	va_start(args, fmt);

	for (char *p = &aString[0]; *p != '\0'; p++)
	{
		if (*p == '%')
		{//確yy認文字がある場合
			p++;	//確認文字まで進める

			switch (*p)
			{
			case 'd':	//数字
				sprintf(&aSaveString[0], "%d", va_arg(args, int));

				//文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);
				break;
			case 'f':	//小数
				sprintf(&aSaveString[0], "%f", va_arg(args, double));

				//文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);

				//小数点以下の文字目まで確認
				for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
				{
					if (aSaveString[nCntlength] == '.')
					{//小数点があった場合

						//小数点以下の桁数を求める
						int nMin = (int)strlen(&aSaveString[nCntlength + 1]);
						
						//小数点より先の文字数と指定した値までの差分を求める
						nMin -= MAX_FLOATNUM;

						//文字列の長さを小数点以下第二位までに補正
						nLength -= nMin;
					}
				}

				break;
			case 'c':	//文字
				sprintf(&aSaveString[0], "%c", va_arg(args, char));

				//文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);
				break;
			case 's':	//文字列
				sprintf(&aSaveString[0], "%s", va_arg(args, const char*));

				//文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);
				break;
			}

			nStopLength = (int)strlen(p) + 1;
			//メモリ内の確認文字より後ろの文字列をずらす
			memmove(p + nLength - 2, p, nStopLength);

			p--;	//ポインタを%まで戻す

			//可変引数を指定された場所に挿入
			memcpy(p, &aSaveString[0], nLength);
		}
	}

	va_end(args);

	//文字列を連結する
	strcat(&g_aStrDebug[0], &aString[0]);
	
}

//==========================================================
//表示するテキスト設定
//==========================================================
void SetManual(void)
{
	D3DLIGHT9 *pLight = GetLight();
	//デバッグ変更とFPS
	PrintDebugProc("【デバッグ表示】【F1】【FPS】 %d\n", GetFPS());

	//操作説明と切り替え確認
	PrintDebugProc("【 操作方法 】----------------------------------------------\n");
	PrintDebugProc(" 全移動【W】 【A】 【S】 【D】	(追従していないときのみ)\n");
	PrintDebugProc(" 視点  【Y】 【N】 【Z】 【C】	(追従していないときのみ)\n");
	PrintDebugProc(" 注視点【T】 【B】 【Q】 【E】	(追従していないときのみ)\n");
	PrintDebugProc(" 距離  【U】 【M】\n");
	PrintDebugProc(" モデル【↑】【↓】【←】【→】\n");
	PrintDebugProc(" ライト変更【L】 色変更(赤 【1,2】青【3,4】緑【5,6】)\n");
	PrintDebugProc(" ワイヤーフレーム 【F8】【%s】\n", c_apOnOff[GetWireframe()]);
	PrintDebugProc(" 爆発のZテスト【F2】【%s】 弾のアルファテスト【F3】【%s】\n\n", c_apOnOff[GetZtest()], c_apOnOff[GetAlphaTest()]);

	//カメラ
	PrintDebugProc("【 カメラ 】------------------------------------------------\n");
	PrintDebugProc(" [視点:   (%f, %f, %f)]\n", GetCamera()->posV.x, GetCamera()->posV.y, GetCamera()->posV.z);
	PrintDebugProc(" [注視点: (%f, %f, %f)]\n", GetCamera()->posR.x, GetCamera()->posR.y, GetCamera()->posR.z);
	PrintDebugProc(" [角度: 　(%f, %f, %f)]\n", GetCamera()->rot.x, GetCamera()->rot.y, GetCamera()->rot.z);
	PrintDebugProc(" [距離: 　%f]\n", GetCamera()->fLength);
	PrintDebugProc(" [追従切替 : F5]\n\n");

	//ライト
	PrintDebugProc("【 ライト 】[選択中ライト : %d]------------------------------\n", GetSelectLight());

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++, pLight++)
	{
		PrintDebugProc(" < ライト- %d > ", nCntLight);
		PrintDebugProc(" (%f : %f : %f)", pLight->Direction.x, pLight->Direction.y, pLight->Direction.z);
		PrintDebugProc(" (%f : %f : %f)\n", pLight->Diffuse.r, pLight->Diffuse.g, pLight->Diffuse.b);
	}
	PrintDebugProc("\n");

	//プレイヤー
	PrintDebugProc("【 プレイヤー 】--------------------------------------------\n");
	PrintDebugProc(" [座標:   (%f, %f, %f)]\n", GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);
	PrintDebugProc(" [移動量: (%f, %f, %f)]\n", GetPlayer()->move.x, GetPlayer()->move.y, GetPlayer()->move.z);
	PrintDebugProc(" [角度: 　(%f),  目標の角度:   (%f)]\n", GetPlayer()->rot.y, GetPlayer()->fRotDest);
	PrintDebugProc(" [現在のキー] %d\n", GetPlayer()->MotionData.PatternKey);
	PrintDebugProc(" [現在のフレーム数] %d\n", GetPlayer()->MotionData.nNowFrame);
	PrintDebugProc(" [現在のキーのフレーム数] %d\n", GetPlayer()->MotionData.aNumFrame[GetPlayer()->MotionData.nMotionNum][GetPlayer()->MotionData.PatternKey]);

}

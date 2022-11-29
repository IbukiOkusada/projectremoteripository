//==========================================================
//
//デバッグ表示処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _DEBUGPROC_H_		//このマクロが定義されていない場合
#define _DEBUGPROC_H_

//==========================================================
//スイッチ列挙型の定義
//==========================================================
typedef enum
{
	DEBUGTYPE_OFF = 0,
	DEBUGTYPE_ON,
	DEBUGTYPE_MAX
}DEBUGTYPE;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void PrintDebugProc(const char *fmt, ...);

#endif
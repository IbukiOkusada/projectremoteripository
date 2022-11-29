//===================================================
//
//パーティクル処理
//Author 奥定伊吹
//
//===================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"		//main.hで定義しているものが必要なためinclude

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, float fRange);
void ResetParticle(void);

#endif

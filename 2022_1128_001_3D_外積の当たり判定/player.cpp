//==========================================================
//
//モデル描画処理
//Author 奥定伊吹
//
//==========================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include <stdio.h>

//==========================================================
//マクロ定義
//==========================================================
#define PLAYERFILE_DATA	"data\\MODEL\\car004.x"		//モデルデータ
#define PLAYER_MOVESPEED	(3.0f)	//移動量
#define ATR_SPEED			(0.3f)	//移動速度減衰
#define PLAYER_BULLETSPEED	(5.0f)	//弾の速度
#define YES_NUMPLAYERPARTS	"NUM_PARTS"	//パーツ数読み込み確認文字
#define YES_SETPLAYERPARTS	"SET_PLAYERSET"	//配置情報読み込み確認文字
#define END_SETPLAYERPARTS	"END_PLAYERSET"	//配置情報読み込み確認文字

#define YES_NUMPLAYERKEY	"NUM_KEY"		//数読み込み確認文字
#define YES_NUMPLAYERLOOP	"LOOP"			//パーツ数読み込み確認文字
#define YES_SETPLAYERMOTION	"MOTIONSET"		//モーション情報読み込み確認文字
#define END_SETPLAYERMOTION	"END_MOTIONSET"	//モーション情報読み込み確認文字

#define YES_SETPLAYERKEYSET	"KEYSET"		//キー情報読み込み確認文字
#define END_SETPLAYERKEYSET	"END_KEYSET"	//キー情報読み込み確認文字

#define YES_SETPLAYERKEY	"KEY"		//キー読み込み確認文字
#define END_SETPLAYERKEY	"END_KEY"	//キー読み込み確認文字

#define SET_PLAYERPARTS	(17)			//配置パーツ数

#define MIN_MOVEMOTION	(0.4f)			//移動から待機に遷移するまでの最低移動量
#define MAX_WALL		(1600.0f)

//==========================================================
//プロトタイプ宣言
//==========================================================
void MovePlayer(void);
void CollisionPlayer(void);
void LoadPlayerData(void);
void LoadPlayerMotionData(void);
void MotionSetData(void);
void UpdateMotionPlayer(void);
void MotionSet(PLAYERMOTION motion);
void SetPlayerPartsPos(void);
void SetMoveMotion(void);
void SetNeutralMotion(void);

//==========================================================
//モデルファイル名
//==========================================================
const char *c_apModelnamePlayer[SET_PLAYERPARTS] =
{
	"data\\MODEL\\sample\\waist.x",
	"data\\MODEL\\sample\\body.x",
	"data\\MODEL\\sample\\head.x",
	"data\\MODEL\\sample\\rightshoulder.x",
	"data\\MODEL\\sample\\rightupperarm.x",
	"data\\MODEL\\sample\\rightforearm.x",
	"data\\MODEL\\sample\\righthand.x",
	"data\\MODEL\\sample\\leftshoulder.x",
	"data\\MODEL\\sample\\leftupperarm.x",
	"data\\MODEL\\sample\\leftforearm.x",
	"data\\MODEL\\sample\\lefthand.x",
	"data\\MODEL\\sample\\righthigh.x",
	"data\\MODEL\\sample\\rightcalf.x",
	"data\\MODEL\\sample\\rightleg.x",
	"data\\MODEL\\sample\\lefthigh.x",
	"data\\MODEL\\sample\\leftcalf.x",
	"data\\MODEL\\sample\\leftleg.x",
};

//==========================================================
//グローバル変数宣言
//==========================================================
Player g_Player;	//プレイヤー情報
PlayerParts g_aPlayerParts[MAX_PLAYERPARTS];	//プレイヤーパーツの情報
int g_nPlayerPartsCount;
int g_nPlayerCameraCount;

//==========================================================
//モデルの初期化処理
//==========================================================
void InitPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファのポインタ

	//各種変数の初期化
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	g_Player.posOld = g_Player.pos;
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.movespeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.fJumpPower = 10.0f;
	g_Player.nShadow = -1;
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nPlayerPartsCount = 0; 
	g_Player.MotionData.nNowFrame = 0;
	g_Player.MotionData.PatternKey = 0;
	g_Player.MotionData.nMotionNum = 0;
	g_Player.bMove = false;		//移動していない状態にする
	g_Player.fRotMove = g_Player.rot.y;
	g_Player.fRotDiff = g_Player.rot.y;
	g_Player.fRotDest = g_Player.rot.y;

	for (int nCntPlayerMotion = 0; nCntPlayerMotion < MAX_PLAYERMOTION; nCntPlayerMotion++)
	{
		g_Player.MotionData.aNumKey[nCntPlayerMotion] = 0;
		g_Player.MotionData.aLoop[nCntPlayerMotion] = 0;

			for (int nCntPlayerKey = 0; nCntPlayerKey < MAX_PLAYERKEY; nCntPlayerKey++)
			{
				g_Player.MotionData.aNumFrame[nCntPlayerMotion][nCntPlayerKey] = 0;
			}
	}


	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayerParts[nCntPlayerParts].nParent = -1;
		g_aPlayerParts[nCntPlayerParts].nPartNum = 0;
		g_aPlayerParts[nCntPlayerParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntPlayerKey = 0; nCntPlayerKey < MAX_PLAYERKEY; nCntPlayerKey++)
		{
			for (int nCntPlayerMotion = 0; nCntPlayerMotion < MAX_PLAYERMOTION; nCntPlayerMotion++)
			{
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].aKeyData[nCntPlayerMotion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].aKeyData[nCntPlayerMotion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].rotDiff[nCntPlayerMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayerParts[nCntPlayerParts].aKey[nCntPlayerKey].posDiff[nCntPlayerMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	//ファイルの読み込み
	LoadPlayerData();

	//プレイヤーのモーション情報読み込み
	LoadPlayerMotionData();

	//モーションの情報設定
	MotionSetData();

	//モーションの初期設定
	MotionSet(PLAYERMOTION_NEUTRAL);

	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].Setpos = g_aPlayerParts[nCntPlayerParts].pos;
	}

	//パーツ位置セット
	SetPlayerPartsPos();

	for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(c_apModelnamePlayer[nCntPlayerParts],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aPlayerParts[nCntPlayerParts].pBuffMat,
			NULL,
			&g_aPlayerParts[nCntPlayerParts].dwNumMat,
			&g_aPlayerParts[nCntPlayerParts].pMesh);

		D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aPlayerParts[nCntPlayerParts].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayerParts[nCntPlayerParts].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在している
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aPlayerParts[nCntPlayerParts].apTexture[nCntMat]);
			}
		}

	}

	//頂点数を取得
	nNumVtx = g_aPlayerParts[0].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_aPlayerParts[0].pMesh->GetFVF());

	//頂点バッファをロック
	g_aPlayerParts[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		if (vtx.x < g_Player.vtxMin.x)
		{//最小値よりも値が小さい場合
			g_Player.vtxMin.x = vtx.x;
		}
		else if (vtx.x > g_Player.vtxMax.x)
		{//最大値よりも値が大きい場合
			g_Player.vtxMax.x = vtx.x;
		}

		if (vtx.z < g_Player.vtxMin.z)
		{//最小値よりも値が小さい場合
			g_Player.vtxMin.z = vtx.z;
		}
		else if (vtx.z > g_Player.vtxMax.z)
		{//最大値よりも値が大きい場合
			g_Player.vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	g_aPlayerParts[0].pMesh->UnlockVertexBuffer();

	g_nPlayerCameraCount = 60;

	//影の設定
	g_Player.nShadow = SetShadow();
}

//==========================================================
//モデルの終了処理
//==========================================================
void UninitPlayer(void)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		//メッシュの廃棄
		if (g_aPlayerParts[nCntPlayerParts].pMesh != NULL)
		{
			g_aPlayerParts[nCntPlayerParts].pMesh->Release();
			g_aPlayerParts[nCntPlayerParts].pMesh = NULL;
		}

		//マテリアルの廃棄
		if (g_aPlayerParts[nCntPlayerParts].pBuffMat != NULL)
		{
			g_aPlayerParts[nCntPlayerParts].pBuffMat->Release();
			g_aPlayerParts[nCntPlayerParts].pBuffMat = NULL;
		}
	}

}

//==========================================================
//モデルの更新処理
//==========================================================
void UpdatePlayer(void)
{
	//前回の座標を取得
	g_Player.posOld = g_Player.pos;

	//移動処理
	MovePlayer();

	//モーション更新
	UpdateMotionPlayer();
}

//==========================================================
//モデルのモーション更新処理
//==========================================================
void UpdateMotionPlayer(void)
{
	g_Player.MotionData.nNowFrame++;

	if (g_Player.MotionData.nNowFrame >= g_Player.MotionData.aNumFrame[g_Player.MotionData.nMotionNum][g_Player.MotionData.PatternKey])
	{//フレーム数が現在のキーのフレーム数を超えた場合
		g_Player.MotionData.nNowFrame = 0;	//フレームカウントリセット
		g_Player.MotionData.PatternKey = (g_Player.MotionData.PatternKey + 1) % g_Player.MotionData.aNumKey[g_Player.MotionData.nMotionNum];	//キーを次に変更
	}
	else
	{//継続時
		for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
		{
			g_aPlayerParts[nCntPlayerParts].rot += g_aPlayerParts[nCntPlayerParts].aKey[g_Player.MotionData.nMotionNum].rotDiff[g_Player.MotionData.PatternKey];
		}
	}
}

//==========================================================
//モデルの描画処理
//==========================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	for (int nCntPlayerParts = -1; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{

		switch (nCntPlayerParts)
		{
		case -1:	//仮想本体

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Player.mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
			D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
			D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);
			break;
		default:	//パーツ

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayerParts[nCntPlayerParts].mtxWorld);

			if (g_aPlayerParts[nCntPlayerParts].nParent == -1)
			{

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aPlayerParts[nCntPlayerParts].pos.x, g_aPlayerParts[nCntPlayerParts].pos.y, g_aPlayerParts[nCntPlayerParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayerParts[nCntPlayerParts].mtxWorld, &g_Player.mtxWorld, &mtxTrans);
			}
			else
			{

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aPlayerParts[nCntPlayerParts].pos.x, g_aPlayerParts[nCntPlayerParts].pos.y, g_aPlayerParts[nCntPlayerParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayerParts[nCntPlayerParts].mtxWorld, &mtxTrans, &g_aPlayerParts[g_aPlayerParts[nCntPlayerParts].nParent].mtxWorld);

			}

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerParts[nCntPlayerParts].rot.y, g_aPlayerParts[nCntPlayerParts].rot.x, g_aPlayerParts[nCntPlayerParts].rot.z);
			D3DXMatrixMultiply(&g_aPlayerParts[nCntPlayerParts].mtxWorld, &mtxRot, &g_aPlayerParts[nCntPlayerParts].mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerParts[nCntPlayerParts].mtxWorld);
			break;
		}

		if (nCntPlayerParts != -1)
		{
			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayerParts[nCntPlayerParts].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aPlayerParts[nCntPlayerParts].dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aPlayerParts[nCntPlayerParts].apTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_aPlayerParts[nCntPlayerParts].pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==========================================================
//プレイヤーの移動処理
//==========================================================
void MovePlayer(void)
{
	Camera *pCamera = GetCamera();

	g_Player.fRotMove = g_Player.rot.y;	//現在の向きを取得

	//プレイヤーの更新
	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		if (GetKeyboardPress(DIK_UP) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = (-pCamera->rot.y + -D3DX_PI * 0.25f);
		}
		else
		{
			g_Player.move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = -pCamera->rot.y;
		}

		//移動設定
		SetMoveMotion();
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{

		if (GetKeyboardPress(DIK_UP) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * PLAYER_MOVESPEED;
			
			g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{
			g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * PLAYER_MOVESPEED;
			
			g_Player.fRotDest = (-pCamera->rot.y + -D3DX_PI * 0.75f);
		}
		else
		{
			g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * PLAYER_MOVESPEED;
			g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 1.0f);
		}

		//移動設定
		SetMoveMotion();
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{
		g_Player.move.x += -cosf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.move.z += -sinf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.fRotDest = (-pCamera->rot.y + D3DX_PI * 0.5f);

		//移動設定
		SetMoveMotion();

	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_Player.move.x += cosf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.move.z += sinf(pCamera->rot.y) * PLAYER_MOVESPEED;
		g_Player.fRotDest = (-pCamera->rot.y + -D3DX_PI * 0.5f);

		//移動設定
		SetMoveMotion();
	}
	else
	{//移動していないとき
		//待機設定
		SetNeutralMotion();
	}

	//弾の発射
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//スペースキーが押されたとき
		SetBullet(D3DXVECTOR3(g_Player.pos.x + cosf(-g_Player.rot.y + -D3DX_PI * 0.5f) * 20.0f, g_Player.pos.y + 30.0f, g_Player.pos.z + sinf(-g_Player.rot.y + -D3DX_PI * 0.5f) * 20.0f),
			D3DXVECTOR3(cosf(-g_Player.rot.y + -D3DX_PI * 0.5f) * PLAYER_BULLETSPEED, 0.0f, sinf(-g_Player.rot.y + -D3DX_PI * 0.5f) * PLAYER_BULLETSPEED),
			10.0f, 10.0f);
	}

	//減衰
	g_Player.move.x += (0.0f - g_Player.move.x) * ATR_SPEED;	//x座標
	g_Player.move.z += (0.0f - g_Player.move.z) * ATR_SPEED;	//y座標

	//座標の変更
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.z += g_Player.move.z;

	if (g_Player.fRotDest > D3DX_PI || g_Player.fRotDest < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (g_Player.fRotDest > D3DX_PI)
		{
			g_Player.fRotDest += (-D3DX_PI * 2);
		}
		else if (g_Player.fRotDest < -D3DX_PI)
		{
			g_Player.fRotDest += (D3DX_PI * 2);
		}
	}

	g_Player.fRotDiff = g_Player.fRotDest - g_Player.fRotMove;	//目標までの移動方向の差分

	if (g_Player.fRotDiff > D3DX_PI || g_Player.fRotDiff < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (g_Player.fRotDiff > D3DX_PI)
		{
			g_Player.fRotDiff += (-D3DX_PI * 2);
		}
		else if (g_Player.fRotDiff < -D3DX_PI)
		{
			g_Player.fRotDiff += (D3DX_PI * 2);
		}
	}

	g_Player.rot.y += g_Player.fRotDiff * 0.08f;

	if (g_Player.rot.y > D3DX_PI || g_Player.rot.y < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y += (-D3DX_PI * 2);
		}
		else if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += (D3DX_PI * 2);
		}
	}

	//壁との判定
	CollisionPlayer();


	if (g_Player.move.x <= 0.1f && g_Player.move.x >= 0.0f ||
		g_Player.move.x >= -0.1f && g_Player.move.x <= 0.0f)
	{//x座標の移動量がない場合
		if (g_Player.move.z <= 0.1f && g_Player.move.z >= 0.0f ||
			g_Player.move.z >= -0.1f && g_Player.move.z <= 0.0f)
		{//z座標の移動量がない場合
			if (g_nPlayerCameraCount <= 0)
			{
				SetRotCamera();
			}
			else
			{
				g_nPlayerCameraCount--;
			}
		}
		else
		{
			g_nPlayerCameraCount = 60;
		}
	}
	else
	{
		g_nPlayerCameraCount = 60;
	}

	//影の位置設定
	SetPositionShadow(g_Player.nShadow, g_Player.pos);


	
}

//==========================================================
//プレイヤーの壁との判定
//==========================================================
void CollisionPlayer(void)
{
	if (g_Player.pos.x > MAX_WALL)
	{
		g_Player.pos.x = MAX_WALL;
		g_Player.move.x = 0.0f;
	}
	if (g_Player.pos.x < -MAX_WALL)
	{
		g_Player.pos.x = -MAX_WALL;
		g_Player.move.x = 0.0f;
	}

	if (g_Player.pos.z > MAX_WALL)
	{
		g_Player.pos.z = MAX_WALL;
		g_Player.move.z = 0.0f;
	}
	if (g_Player.pos.z < -MAX_WALL)
	{
		g_Player.pos.z = -MAX_WALL;
		g_Player.move.z = 0.0f;
	}
}

//==========================================================
//プレイヤーの情報取得
//==========================================================
Player *GetPlayer(void)
{
	return &g_Player;
}

//==========================================================
//プレイヤーのデータ読み込み処理
//==========================================================
void LoadPlayerData(void)
{
	FILE *pFile;

	pFile = fopen("data\\TXT\\model.txt", "r");

	if (pFile != NULL)
	{//開けた場合

		char aNumStr[32] = {};	//配置数文字格納用

		//配置数読み込み
		fscanf(pFile, "%s", &aNumStr[0]);		//コメント(NUM_BLOCK)

		if (strcmp(&aNumStr[0], YES_NUMPLAYERPARTS) == 0)
		{//配置数確認の文字があった場合
			fscanf(pFile, "%s", &aNumStr[0]);		//コメント(=)
			fscanf(pFile, "%d", &g_nPlayerPartsCount);	//配置数取得

													//配置数分読み込む
			for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
			{
				char aStartStr[40] = {};	//開始の文字

				fscanf(pFile, "%s", &aStartStr[0]);
				if (strcmp(&aStartStr[0], YES_SETPLAYERPARTS) == 0)
				{//開始文字があった場合
					char aString[128] = {};
					char aEndStr[40] = {};	//終了の文字

					//fscanf(pFile, "%s", &aString[0]);		//コメント(何個目か)

					//パーツ番号を取得
					fscanf(pFile, "%s", &aString[0]);		//コメント(INDEX)
					fscanf(pFile, "%s", &aString[0]);		//コメント(=)
					fscanf(pFile, "%d", &g_aPlayerParts[nCntPlayerParts].nPartNum);		//パーツ番号

					//親番号を取得
					fscanf(pFile, "%s", &aString[0]);		//コメント(PARENT)
					fscanf(pFile, "%s", &aString[0]);		//コメント(=)
					fscanf(pFile, "%d", &g_aPlayerParts[nCntPlayerParts].nParent);		//親

					//座標を取得
					fscanf(pFile, "%s", &aString[0]);		//コメント(POS)
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].pos.x);		//x座標
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].pos.y);		//y座標
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].pos.z);		//z座標

					//向きを取得
					fscanf(pFile, "%s", &aString[0]);		//コメント(ROT)
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].rot.x);		//x座標
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].rot.y);		//y座標
					fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].rot.z);		//z座標

					//終了文字取得
					fscanf(pFile, "%s", &aEndStr[0]);

					if (strcmp(&aEndStr[0], END_SETPLAYERPARTS) != 0)
					{//終了文字がない場合
						break;	//繰り返しを抜ける
					}
				}
				else
				{//なかった場合
					break;
				}
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//開けなかった場合

	}
}

//==========================================================
//プレイヤーのモーションデータ読み込み
//==========================================================
void LoadPlayerMotionData(void)
{
	FILE *pFile;

	pFile = fopen("data\\TXT\\motion.txt", "r");

	if (pFile != NULL)
	{//開けた場合

		char aNumStr[128] = {};	//配置数文字格納用
		fscanf(pFile, "%s", &aNumStr[0]);		//コメント(線)
		fscanf(pFile, "%s", &aNumStr[0]);		//コメント(#)
		fscanf(pFile, "%s", &aNumStr[0]);		//コメント(モーション情報)
		fscanf(pFile, "%s", &aNumStr[0]);		//コメント(線)

		for (int nCntMotion = 0; nCntMotion < PLAYERMOTION_ACTION; nCntMotion++)
		{
			fscanf(pFile, "%s", &aNumStr[0]);		//コメント(線)
			fscanf(pFile, "%s", &aNumStr[0]);		//コメント(#)
			fscanf(pFile, "%s", &aNumStr[0]);		//コメント(...モーション)
			fscanf(pFile, "%s", &aNumStr[0]);		//コメント(線)
			fscanf(pFile, "%s", &aNumStr[0]);		//コメント(確認文字)

			if (strcmp(&aNumStr[0], YES_SETPLAYERMOTION) == 0)
			{//確認の文字があった場合
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(LOOP)
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(=)
				fscanf(pFile, "%d", &g_Player.MotionData.aLoop[nCntMotion]);	//ループ取得
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(#)
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント()
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(/)
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント()

				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(KEY)
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(=)
				fscanf(pFile, "%d", &g_Player.MotionData.aNumKey[nCntMotion]);	//ループ取得
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(#)
				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(文字)

				for (int nCntPlayerKey = 0; nCntPlayerKey < g_Player.MotionData.aNumKey[nCntMotion]; nCntPlayerKey++)
				{
					fscanf(pFile, "%s", &aNumStr[0]);		//コメント(確認文字)

					if (strcmp(&aNumStr[0], YES_SETPLAYERKEYSET) == 0)
					{//開始文字があった場合
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(#)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(線)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(<<)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(KEY)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(:)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(数字)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(/)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(数字)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(>>)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(線)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(FRAME)
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(=)
						fscanf(pFile, "%d", &g_Player.MotionData.aNumFrame[nCntMotion][nCntPlayerKey]);		//コメント(=)


						for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
						{
							fscanf(pFile, "%s", &aNumStr[0]);		//コメント(確認文字)

							if (strcmp(&aNumStr[0], YES_SETPLAYERKEY) == 0)
							{//開始文字があった場合
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(#)
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(空白)
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(線)
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(空白)
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント([)
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(数)

								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(POS)
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(=)
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].pos.x);		//x座標
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].pos.y);		//y座標
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].pos.z);		//z座標

																									//向きを取得
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(ROT)
								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(=)
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].rot.x);		//x座標
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].rot.y);		//y座標
								fscanf(pFile, "%f", &g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntPlayerKey].rot.z);		//z座標

								fscanf(pFile, "%s", &aNumStr[0]);		//コメント(確認文字)
							}
						}
						fscanf(pFile, "%s", &aNumStr[0]);		//コメント(確認文字)
					}
				}

				fscanf(pFile, "%s", &aNumStr[0]);		//コメント(確認文字)
			}
			else
			{//なかった場合

			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//開けなかった場合

	}
}

//==========================================================
//プレイヤーのモーション設定
//==========================================================
void MotionSetData(void)
{
	for (int nCntMotion = 0; nCntMotion < PLAYERMOTION_ACTION; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < g_Player.MotionData.aNumKey[nCntMotion]; nCntKey++)
		{
			//一フレームあたりの倍率を求める
			float fFrame = 1.0f / (float)g_Player.MotionData.aNumFrame[nCntMotion][nCntKey];

			for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
			{
				//向きの移動量の差分を求める
				g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey] =
					(g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[(nCntKey + 1) % g_Player.MotionData.aNumKey[nCntMotion]].rot
						- g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntKey].rot);

				//角度の確認
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
				{//-3.14～3.14の範囲外の場合
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
				{//-3.14～3.14の範囲外の場合
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
				{//-3.14～3.14の範囲外の場合
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (D3DX_PI * 2);
					}
				}

				//補正をかける
				g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey] *= fFrame;

				//角度の確認
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
				{//-3.14～3.14の範囲外の場合
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].x += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
				{//-3.14～3.14の範囲外の場合
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].y += (D3DX_PI * 2);
					}
				}
				if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI || g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
				{//-3.14～3.14の範囲外の場合
					if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z > D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (-D3DX_PI * 2);
					}
					else if (g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z < -D3DX_PI)
					{
						g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].rotDiff[nCntKey].z += (D3DX_PI * 2);
					}
				}

				//座標の移動量の差分を求める
				g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].posDiff[nCntKey] =
					(g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[(nCntKey + 1) % g_Player.MotionData.aNumKey[nCntMotion]].pos
						- g_aPlayerParts[nCntPlayerParts].aKey[nCntMotion].aKeyData[nCntKey].pos)
					* fFrame;
			}
		}
	}
}

//==========================================================
//初期モーション角度設定
//==========================================================
void MotionSet(PLAYERMOTION motion)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].rot = g_aPlayerParts[nCntPlayerParts].aKey[motion].aKeyData[0].rot;
	}
}

//==========================================================
//初期パーツ座標設定
//==========================================================
void SetPlayerPartsPos(void)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < g_nPlayerPartsCount; nCntPlayerParts++)
	{
		g_aPlayerParts[nCntPlayerParts].pos = g_aPlayerParts[nCntPlayerParts].Setpos;
	}
}

//==========================================================
//移動モーション設定
//==========================================================
void SetMoveMotion(void)
{
	if (g_Player.bMove == false)
	{//移動状態ではない場合
		g_Player.bMove = true;	//移動状態にする
		g_Player.MotionData.nMotionNum = PLAYERMOTION_WALK;	//移動状態にする
		g_Player.MotionData.nNowFrame = 0;					//フレーム数をリセット
		g_Player.MotionData.PatternKey = 0;					//現在のキーを初期化

		//座標セット
		SetPlayerPartsPos();

		//モーション角度設定
		MotionSet(PLAYERMOTION_WALK);

	}
}

//==========================================================
//待機モーション設定
//==========================================================
void SetNeutralMotion(void)
{
	if (g_Player.move.x <= MIN_MOVEMOTION && g_Player.move.x >= 0.0f ||
		g_Player.move.x >= -MIN_MOVEMOTION && g_Player.move.x <= 0.0f)
	{//x座標の移動量がない場合

		if (g_Player.move.z <= MIN_MOVEMOTION && g_Player.move.z >= 0.0f ||
			g_Player.move.z >= -MIN_MOVEMOTION && g_Player.move.z <= 0.0f)
		{//z座標の移動量がない場合

			if (g_Player.bMove == true)
			{//プレイヤーが移動状態の時
				g_Player.bMove = false;	//移動していない状態にする
				g_Player.MotionData.nMotionNum = PLAYERMOTION_NEUTRAL;	//待機状態にする
				g_Player.MotionData.nNowFrame = 0;					//フレーム数をリセット
				g_Player.MotionData.PatternKey = 0;					//現在のキーを初期化

				//座標セット
				SetPlayerPartsPos();

				//モーション角度設定
				MotionSet(PLAYERMOTION_NEUTRAL);
			}
		}
	}
}
//==========================================================
//
//���C�g����
//Author ����ɐ�
//
//==========================================================
#include "light.h"
#include "input.h"

//==========================================================
//�O���[�o���ϐ�
//==========================================================
void ChangeColLight(void);
void MoveLight(void);

//==========================================================
//�O���[�o���ϐ�
//==========================================================
D3DLIGHT9 g_alight[NUM_LIGHT];		//���C�g�̏��
int g_nSelectLight;	//�I�𒆃��C�g

//==========================================================
//���C�g�̏���������
//==========================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾
	D3DXVECTOR3 vecDir;		//�ݒ�ύX�p�x�N�g��

	g_nSelectLight = 0;

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
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

		//���C�g�̕�����ݒ�
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

		D3DXVec3Normalize(&vecDir, &vecDir);	//�x�N�g���𐳋K������
		g_alight[nCntLight].Direction = vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		//���C�g��L��������
		pDevice->LightEnable(nCntLight, TRUE);


	}
}

//==========================================================
//���C�g�̏I������
//==========================================================
void UninitLight(void)
{

}

//==========================================================
//���C�g�̍X�V����
//==========================================================
void UpdateLight(void)
{

	//�ԍ��ύX
	if (GetKeyboardTrigger(DIK_L) == true)
	{//L�L�[�����ꂽ�Ƃ�
		//�ԍ��ύX
		g_nSelectLight = (g_nSelectLight + 1) % NUM_LIGHT;
	}

	//�F�̕ύX
	ChangeColLight();

	//�ړ�
	MoveLight();


}

//==========================================================
//���C�g�̐F�ύX
//==========================================================
void ChangeColLight(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	//�Ԃ̕ύX
	if (GetKeyboardTrigger(DIK_1) == true || GetKeyboardRepeat(DIK_1) == true)
	{//1�L�[�������ꂽ�Ƃ�
		g_alight[g_nSelectLight].Diffuse.r -= 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.r < 0.0f)
		{//�F�����E�𒴂����Ƃ�
			g_alight[g_nSelectLight].Diffuse.r = 1.0f;
		}
	}
	else if(GetKeyboardTrigger(DIK_2) == true || GetKeyboardRepeat(DIK_2) == true)
	{//2�L�[�������ꂽ�Ƃ�
		g_alight[g_nSelectLight].Diffuse.r += 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.r > 1.0f)
		{//�F�����E�𒴂����Ƃ�
			g_alight[g_nSelectLight].Diffuse.r = 0.0f;
		}
	}

	//�΂̕ύX
	if (GetKeyboardTrigger(DIK_3) == true || GetKeyboardRepeat(DIK_3) == true)
	{//3�L�[�������ꂽ�Ƃ�
		g_alight[g_nSelectLight].Diffuse.g -= 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.g < 0.0f)
		{//�F�����E�𒴂����Ƃ�
			g_alight[g_nSelectLight].Diffuse.g = 1.0f;
		}
	}
	else if (GetKeyboardTrigger(DIK_4) == true || GetKeyboardRepeat(DIK_4) == true)
	{//4�L�[�������ꂽ�Ƃ�
		g_alight[g_nSelectLight].Diffuse.g += 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.g > 1.0f)
		{//�F�����E�𒴂����Ƃ�
			g_alight[g_nSelectLight].Diffuse.g = 0.0f;
		}
	}

	//�̕ύX
	if (GetKeyboardTrigger(DIK_5) == true || GetKeyboardRepeat(DIK_5) == true)
	{//5�L�[�������ꂽ�Ƃ�
		g_alight[g_nSelectLight].Diffuse.b -= 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.b < 0.0f)
		{//�F�����E�𒴂����Ƃ�
			g_alight[g_nSelectLight].Diffuse.b = 1.0f;
		}
	}
	else if (GetKeyboardTrigger(DIK_6) == true || GetKeyboardRepeat(DIK_6) == true)
	{//6�L�[�������ꂽ�Ƃ�
		g_alight[g_nSelectLight].Diffuse.b += 0.01f;

		if (g_alight[g_nSelectLight].Diffuse.b > 1.0f)
		{//�F�����E�𒴂����Ƃ�
			g_alight[g_nSelectLight].Diffuse.b = 0.0f;
		}
	}

	//���C�g��ݒ肷��
	pDevice->SetLight(g_nSelectLight, &g_alight[g_nSelectLight]);
}

//==========================================================
//���C�g�̈ړ�����
//==========================================================
void MoveLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾
	D3DXVECTOR3 vecDir;		//�ݒ�ύX�p�x�N�g��

	//�x�N�g�����m��
	vecDir = g_alight[g_nSelectLight].Direction;

	////�x�N�g���̕ύX
	//if (GetKeyboardTrigger(DIK_I) == true || GetKeyboardRepeat(DIK_I) == true)
	//{//1�L�[�������ꂽ�Ƃ�
	//	vecDir.x -= 0.01f;
	//}
	//else if (GetKeyboardTrigger(DIK_J) == true || GetKeyboardRepeat(DIK_J) == true)
	//{//2�L�[�������ꂽ�Ƃ�
	//	vecDir.x += 0.01f;
	//}
	//else if (GetKeyboardTrigger(DIK_T) == true || GetKeyboardRepeat(DIK_T) == true)
	//{//2�L�[�������ꂽ�Ƃ�
	//	vecDir.y += 0.01f;
	//}
	//else if (GetKeyboardTrigger(DIK_L) == true || GetKeyboardRepeat(DIK_L) == true)
	//{//2�L�[�������ꂽ�Ƃ�
	//	vecDir.y -= 0.01f;
	//}

	D3DXVec3Normalize(&vecDir, &vecDir);	//�x�N�g���𐳋K������
	g_alight[g_nSelectLight].Direction = vecDir;
}

//==========================================================
//���C�g�̎擾
//==========================================================
D3DLIGHT9 *GetLight(void)
{
	return &g_alight[0];
}

//==========================================================
//�I�𒆃��C�g�ԍ��擾
//==========================================================
int GetSelectLight(void)
{
	return g_nSelectLight;
}

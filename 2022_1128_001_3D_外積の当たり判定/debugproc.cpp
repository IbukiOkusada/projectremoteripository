//==========================================================
//
//�f�o�b�O�\������
//Author ����ɐ�
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
//�}�N����`
//==========================================================
#define MAX_DEBUGSTRING	(2048)			//�f�o�b�O�\���̍ő啶����
#define MAX_FLOATNUM	(2)				//�����_�ȉ��̍ő�\����

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPD3DXFONT g_pFont = NULL;	//�t�H���g�ւ̃|�C���^
char g_aStrDebug[MAX_DEBUGSTRING];	//�f�o�b�O�\���p�̕�����
bool g_bDispDebug = true;	//�f�o�b�O�\����ON/OFF

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void SetManual(void);

//==========================================================
//�f�o�b�OONOFF�\�����b�Z�[�W
//==========================================================
const char *c_apOnOff[DEBUGTYPE_MAX] =
{
	"OFF",
	"ON",
};

//==========================================================
//�f�o�b�O�\���̏���������
//==========================================================
void InitDebugProc(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//�f�o�b�O�\�����̃N���A
	memset(&g_aStrDebug[0], NULL, sizeof(g_aStrDebug));
}

//==========================================================
//�f�o�b�O�\���̏I������
//==========================================================
void UninitDebugProc(void)
{
	//�f�o�b�O�\���p�t�H���g�̔p��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//==========================================================
//�f�o�b�O�\���̍X�V����
//==========================================================
void UpdateDebugProc(void)
{
	//��������ݒ�
	SetManual();

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1�L�[�������ꂽ�Ƃ�
		g_bDispDebug = g_bDispDebug ? false : true;
	}
}

//==========================================================
//�f�o�b�O�\���̕`�揈��
//==========================================================
void DrawDebugProc(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (g_bDispDebug == true)
	{//�f�o�b�N���[�h���I���̎�
		//�e�L�X�g�̕`��
		g_pFont->DrawText(NULL, &g_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	//�f�o�b�O�\�����̃N���A
	memset(&g_aStrDebug[0], NULL, sizeof(g_aStrDebug));
}

//==========================================================
//�f�o�b�O�\���̐ݒ菈��
//==========================================================
void PrintDebugProc(const char *fmt, ...)
{
	va_list args;
	char aString[MAX_DEBUGSTRING];
	char aSaveString[MAX_DEBUGSTRING];
	int nLength;
	int nStopLength;

	//������̑��
	strcpy(&aString[0], fmt);

	va_start(args, fmt);

	for (char *p = &aString[0]; *p != '\0'; p++)
	{
		if (*p == '%')
		{//�myy�F����������ꍇ
			p++;	//�m�F�����܂Ői�߂�

			switch (*p)
			{
			case 'd':	//����
				sprintf(&aSaveString[0], "%d", va_arg(args, int));

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);
				break;
			case 'f':	//����
				sprintf(&aSaveString[0], "%f", va_arg(args, double));

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);

				//�����_�ȉ��̕����ڂ܂Ŋm�F
				for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
				{
					if (aSaveString[nCntlength] == '.')
					{//�����_���������ꍇ

						//�����_�ȉ��̌��������߂�
						int nMin = (int)strlen(&aSaveString[nCntlength + 1]);
						
						//�����_����̕������Ǝw�肵���l�܂ł̍��������߂�
						nMin -= MAX_FLOATNUM;

						//������̒����������_�ȉ����ʂ܂łɕ␳
						nLength -= nMin;
					}
				}

				break;
			case 'c':	//����
				sprintf(&aSaveString[0], "%c", va_arg(args, char));

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);
				break;
			case 's':	//������
				sprintf(&aSaveString[0], "%s", va_arg(args, const char*));

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);
				break;
			}

			nStopLength = (int)strlen(p) + 1;
			//���������̊m�F���������̕���������炷
			memmove(p + nLength - 2, p, nStopLength);

			p--;	//�|�C���^��%�܂Ŗ߂�

			//�ψ������w�肳�ꂽ�ꏊ�ɑ}��
			memcpy(p, &aSaveString[0], nLength);
		}
	}

	va_end(args);

	//�������A������
	strcat(&g_aStrDebug[0], &aString[0]);
	
}

//==========================================================
//�\������e�L�X�g�ݒ�
//==========================================================
void SetManual(void)
{
	D3DLIGHT9 *pLight = GetLight();
	//�f�o�b�O�ύX��FPS
	PrintDebugProc("�y�f�o�b�O�\���z�yF1�z�yFPS�z %d\n", GetFPS());

	//��������Ɛ؂�ւ��m�F
	PrintDebugProc("�y ������@ �z----------------------------------------------\n");
	PrintDebugProc(" �S�ړ��yW�z �yA�z �yS�z �yD�z	(�Ǐ]���Ă��Ȃ��Ƃ��̂�)\n");
	PrintDebugProc(" ���_  �yY�z �yN�z �yZ�z �yC�z	(�Ǐ]���Ă��Ȃ��Ƃ��̂�)\n");
	PrintDebugProc(" �����_�yT�z �yB�z �yQ�z �yE�z	(�Ǐ]���Ă��Ȃ��Ƃ��̂�)\n");
	PrintDebugProc(" ����  �yU�z �yM�z\n");
	PrintDebugProc(" ���f���y���z�y���z�y���z�y���z\n");
	PrintDebugProc(" ���C�g�ύX�yL�z �F�ύX(�� �y1,2�z�y3,4�z�΁y5,6�z)\n");
	PrintDebugProc(" ���C���[�t���[�� �yF8�z�y%s�z\n", c_apOnOff[GetWireframe()]);
	PrintDebugProc(" ������Z�e�X�g�yF2�z�y%s�z �e�̃A���t�@�e�X�g�yF3�z�y%s�z\n\n", c_apOnOff[GetZtest()], c_apOnOff[GetAlphaTest()]);

	//�J����
	PrintDebugProc("�y �J���� �z------------------------------------------------\n");
	PrintDebugProc(" [���_:   (%f, %f, %f)]\n", GetCamera()->posV.x, GetCamera()->posV.y, GetCamera()->posV.z);
	PrintDebugProc(" [�����_: (%f, %f, %f)]\n", GetCamera()->posR.x, GetCamera()->posR.y, GetCamera()->posR.z);
	PrintDebugProc(" [�p�x: �@(%f, %f, %f)]\n", GetCamera()->rot.x, GetCamera()->rot.y, GetCamera()->rot.z);
	PrintDebugProc(" [����: �@%f]\n", GetCamera()->fLength);
	PrintDebugProc(" [�Ǐ]�ؑ� : F5]\n\n");

	//���C�g
	PrintDebugProc("�y ���C�g �z[�I�𒆃��C�g : %d]------------------------------\n", GetSelectLight());

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++, pLight++)
	{
		PrintDebugProc(" < ���C�g- %d > ", nCntLight);
		PrintDebugProc(" (%f : %f : %f)", pLight->Direction.x, pLight->Direction.y, pLight->Direction.z);
		PrintDebugProc(" (%f : %f : %f)\n", pLight->Diffuse.r, pLight->Diffuse.g, pLight->Diffuse.b);
	}
	PrintDebugProc("\n");

	//�v���C���[
	PrintDebugProc("�y �v���C���[ �z--------------------------------------------\n");
	PrintDebugProc(" [���W:   (%f, %f, %f)]\n", GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);
	PrintDebugProc(" [�ړ���: (%f, %f, %f)]\n", GetPlayer()->move.x, GetPlayer()->move.y, GetPlayer()->move.z);
	PrintDebugProc(" [�p�x: �@(%f),  �ڕW�̊p�x:   (%f)]\n", GetPlayer()->rot.y, GetPlayer()->fRotDest);
	PrintDebugProc(" [���݂̃L�[] %d\n", GetPlayer()->MotionData.PatternKey);
	PrintDebugProc(" [���݂̃t���[����] %d\n", GetPlayer()->MotionData.nNowFrame);
	PrintDebugProc(" [���݂̃L�[�̃t���[����] %d\n", GetPlayer()->MotionData.aNumFrame[GetPlayer()->MotionData.nMotionNum][GetPlayer()->MotionData.PatternKey]);

}

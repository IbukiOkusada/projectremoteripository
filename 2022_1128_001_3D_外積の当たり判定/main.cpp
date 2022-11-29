//==========================================================
//
//���C������
//Author ����ɐ�
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


//�}�N����`
#define WINDOW_NAME	"3D"			//�E�C���h�E�̖��O
#define PCTIME		(1000/60)		//�p�\�R���̎���
#define MAX_STRING	(512)			//�����̍ő吔
#define CLASS_NAME	"WindowClass"	//�E�C���h�E�N���X�̖��O

//�v���g�^�C�v�錾
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

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;	//Direct3D�I�u�W�F�N�g�̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3D�f�o�C�X�ւ̃|�C���^
int g_nCountFPS = 0;		//FPS�J�E���^
MODE g_mode = MODE_TITLE;	//���[�h
int g_bWire = 0;		//���C���[�t���[���\����ON/OFF
int g_nZtest = 0;		//Z�e�X�g��ON/OFF
int g_nAlphatest = 1;		//�A���t�@�e�X�g��ON/OFF

//================================================
//���C���֐�
//================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						//�E�C���h�E�̃X�^�C��
		WindowProc,						//�E�C���h�E�v���V�[�W��
		0,								//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,								//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,						//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		//�N���C�A���g�̈�̔w�i�F
		NULL,							//���j���[�o�[
		CLASS_NAME,						//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	//�t�@�C���̃A�C�R��
	};

	HWND hWnd;	//�E�C���h�E�n���h��
	MSG msg;	//���b�Z�[�W���i�[����ϐ�

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�`�����w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx
	(
		0,								//�g���E�C���h�E�X�^�C��
		CLASS_NAME,						//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,					//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�C���h�E�̍���x���W
		CW_USEDEFAULT,					//�E�C���h�E�̍���y���W
		(rect.right - rect.left),		//�E�C���h�E�̕�
		(rect.bottom - rect.top),		//�E�C���h�E�̍���
		NULL,							//�e�E�C���h�E�̃n���h��
		NULL,							//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,						//�C���X�^���X�n���h��
		NULL							//�E�C���h�E�쐬�f�[�^
	);

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//�����������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;	//������
	dwExecLastTime = timeGetTime();	//���ݎ������擾(�ۑ�)
	dwFrameCount = 0;	//������
	dwFPSLastTime = timeGetTime();	//���ݎ������擾(�ۑ�)

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();	//���ݎ������擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				//FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;	//�t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;	//�����J�n�̎���[���ݎ���]��ۑ�

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;	//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//================================================
//�E�C���h�E�v���V�[�W��
//================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;					//�Ԃ�l���i�[


	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//�E�C���h�E�̗̈�(�`)

	switch (uMsg)
	{
	case WM_DESTROY:		//�E�B���h�E�p���̃��b�Z�[�W

							//WM_QUET���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:		//�L�[�����̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE:

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				//�E�C���h�E��p������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_COMMAND:	//�R�}���h���s���b�Z�[�W
		break;
	case WM_CLOSE:		//����{�^������
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{//�͂���I�������Ƃ�
		 //�E�C���h�E��p������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{//��������I�������Ƃ�
			return 0;	//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//����̏�����Ԃ�
}

//================================================
//����������
//================================================
LRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//DirectX3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						//�o�b�N�o�b�t�@�[�̌`��
	d3dpp.BackBufferCount = 1;									//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//�_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE;						//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̍쐬
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

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	
	//�e�N�X�`���X�e�[�W�X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�}�E�X�̏�����
	if (FAILED(InitMouse(hWnd)))
	{
		return E_FAIL;
	}

	//�Q�[���p�b�h�̏���������
	InitGamepad();

	//�J�����̏�����
	InitCamera();

	//�e�̏���������
	InitShadow();

	//���C�g�̏�����
	InitLight();

	//�|���S���̏���������
	//InitPolygon();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//�ǂ̏���������
	InitWall();

	//���b�V���E�H�[���̏���������
	InitMeshWall();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//�r���{�[�h�̏�����
	InitBillboard();

	//�e�̏�����
	InitBullet();

	//�v���C���[�̏���������
	InitPlayer();

	//���f���̏���������
	InitModel();

	//�G�̏���������
	InitEnemy();

	//�����̏�����
	InitExplosion();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//�f�o�b�O�\���̏�����
	InitDebugProc();

	return S_OK;
}

//================================================
//�I������
//================================================
void Uninit(void)
{

	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�}�E�X�̏I������
	UninitMouse();
	
	//�Q�[���p�b�h�̏I������
	UninitGamepad();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�|���S���̏I������
	//UninitPolygon();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//�ǂ̏I������
	UninitWall();

	//���b�V���E�H�[���̏I������
	UninitMeshWall();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���h�[���̏I������
	UninitMeshDome();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�e�̏I������
	UninitBullet();

	//�v���C���[�̏I������
	UninitPlayer();

	//���f���̏I������
	UninitModel();

	//�G�̏I������
	UninitEnemy();

	//�e�̏I������
	UninitShadow();

	//�����̏I������
	UninitExplosion();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�f�o�b�O�\���̏I������
	UninitDebugProc();

	//Direct3D�f�o�C�X�̔p��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔p��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

//================================================
//�X�V����
//================================================
void Update(void)
{
	//�f�o�b�O�\���̍X�V����
	UpdateDebugProc();

	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�}�E�X�̍X�V����
	UpdateMouse();

	//�Q�[���p�b�h�̍X�V����
	UpdateGamepad();

	//�J�����̍X�V
	UpdateCamera();

	//���C�g�̍X�V
	UpdateLight();

	//�|���S���̍X�V����
	//UpdatePolygon();

	//���b�V���t�B�[���h�̍X�V����
	UpdateMeshField();

	//�ǂ̍X�V����
	UpdateWall();

	//���b�V���E�H�[���̍X�V����
	UpdateMeshWall();

	//���b�V���V�����_�[�̍X�V����
	UpdateMeshCylinder();

	//���b�V���h�[���̍X�V����
	UpdateMeshDome();

	//�r���{�[�h�̍X�V����
	UpdateBillboard();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//���f���̍X�V����
	UpdateModel();

	//�G�̍X�V����
	UpdateEnemy();

	//�e�̍X�V����
	UpdateBullet();

	//�e�̍X�V����
	UpdateShadow();

	//�����̍X�V����
	UpdateExplosion();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8�L�[���͂��ꂽ�Ƃ�
		g_bWire = g_bWire ^ 1;

		//�`�惂�[�h�̕ύX
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
		g_nAlphatest ^= 1;		//�e�̃A���t�@�e�X�g�̐؂�ւ�
	}
	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		g_nZtest ^= 1;			//������Z�e�X�g�̐؂�ւ�
	}

}

//================================================
//�`�揈��
//================================================
void Draw(void)
{
	//��ʃN���A
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`�悪���������ꍇ

		//�J�����̐ݒ�
		SetCamera();

		//�|���S���̕`��
		//DrawPolygon();

		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshField();

		//�r���{�[�h�̕`�揈��
		DrawBillboard();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//���f���̕`�揈��
		DrawModel();

		//�G�̕`�揈��
		DrawEnemy();

		//�e�̕`�揈��
		DrawBullet();

		//�e�̕`�揈��
		DrawShadow();

		//�ǂ̕`��
		DrawWall();

		//���b�V���E�H�[���̕`��
		DrawMeshWall();

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���h�[���̕`�揈��
		DrawMeshDome();

		//�����̕`��
		DrawExplosion();

		//�G�t�F�N�g�̕`��
		DrawEffect();

		//�f�o�b�O�\���̕`��
		DrawDebugProc();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//================================================
//�f�o�C�X�̎擾
//================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//================================================
//FPS�̎擾
//================================================
int GetFPS(void)
{
	return g_nCountFPS;
}

//================================================
//���[�h�̐ݒ�
//================================================
void SetMode(MODE mode)
{
	

}

//================================================
//���[�h�̎擾
//================================================
MODE GetMode(void)
{
	return g_mode;
}

//================================================
//Z�e�X�g��Ԃ̎擾
//================================================
int GetZtest(void)
{
	return g_nZtest;
}

//================================================
//���e�X�g��Ԃ̎擾
//================================================
int GetAlphaTest(void)
{
	return g_nAlphatest;
}

//================================================
//�`���Ԃ̎擾
//================================================
int GetWireframe(void)
{
	return g_bWire;
}
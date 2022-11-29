//==========================================================
//
//�J��������
//Author ����ɐ�
//
//==========================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//==========================================================
//�}�N����`
//==========================================================
#define CAMERA_MOVESPEED	(1.0f)			//�ړ���
#define CAMERA_LENGTHMOVE	(1.0f)			//�J���������ړ���
#define ROTATE_SPEED	(0.008f)			//�J�����̉�]���x
#define CAMERA_MAXLENGTH	(400.0f)		//�J�����ő勗��
#define CAMERA_MINLENGTH	(100.0f)		//�J�����ŏ�����
#define MAX_CAMERA_ROTZ (D3DX_PI * 0.9f)	//�J�����ő�p�x
#define MIN_CAMERA_ROTZ (D3DX_PI * 0.1f)	//�J�����ő�p�x
#define MOUSE_MOVESPEED		(0.2f)			//�}�E�X�ړ����x
#define MOUSE_ROTATESPEED_X	(0.005f)		//�}�E�X��]���xx��
#define MOUSE_ROTATESPEED_Z	(0.01f)			//�}�E�X��]���xz��
#define MOUSE_WHEELSPEED	(0.1f)			//�}�E�X�z�C�[����]���x

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void MoveCameraV(void);
void MoveCameraVR(void);
void MoveCameraR(void);
void SetCameraV(void);
void SetCameraR(void);
void MouseCamera(void);
void CameraMove(void);


//==========================================================
//�O���[�o���ϐ�
//==========================================================
Camera g_camera;	//�J�����̏��
int g_nStoker = 1;	//�Ǐ]�̃I���I�t

//==========================================================
//�J�����̏���������
//==========================================================
void InitCamera(void)
{
	//�e�ϐ��̏�����
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posV = D3DXVECTOR3(-300.0f, 150.0f, 0.0f);
	g_camera.fLength = sqrtf((g_camera.posV.x - g_camera.posR.x) * (g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z));
	g_camera.rot = D3DXVECTOR3(0.0f, atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z), tanf(g_camera.posR.y - g_camera.posV.y));
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�ݒ�
	SetCameraV();
}

//==========================================================
//�J�����̏I������
//==========================================================
void UninitCamera(void)
{

}

//==========================================================
//�J�����̍X�V����
//==========================================================
void UpdateCamera(void)
{

	if (g_nStoker == 0)
	{
		//�}�E�X�ł̎��_�ړ�
		MouseCamera();

		//�S���_�̈ړ�
		MoveCameraVR();

		//�����_�̈ړ�
		MoveCameraR();

		//���_�̈ړ�
		MoveCameraV();
	}
	else
	{

		//�Ǐ]�ړ��̍X�V
		CameraMove();

		//���_�̈ړ�
		MoveCameraV();
	}

	//�Ǐ]��Ԃ̍X�V
	if (GetKeyboardTrigger(DIK_F5) == true)
	{
		g_nStoker ^= 1;
	}
}

//==========================================================
//�J�����̐ݒ菈��
//==========================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	//�v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̐���
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float) SCREEN_HEIGHT,
								10.0f,
								5000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//==========================================================
//�J�����̏��擾
//==========================================================
Camera *GetCamera(void)
{
	return &g_camera;
}

//==========================================================
//�J�����̑S�ړ�����
//==========================================================
void MoveCameraVR(void)
{
	Camera *pCamera = GetCamera();

	//���_�����_�̓����ړ�
	if (GetKeyboardPress(DIK_A) == true)
	{//A�L�[����

		//���_�̈ړ�
		if (GetKeyboardPress(DIK_W) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
		}
		else
		{
			pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[����

		//���_�̈ړ�
		if (GetKeyboardPress(DIK_W) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			pCamera->move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
		}
		else
		{
			pCamera->move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
			pCamera->move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
		}

	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//W�L�[����

		//���_�̈ړ�
		pCamera->move.x += -cosf(pCamera->rot.y) * CAMERA_MOVESPEED;
		pCamera->move.z += -sinf(pCamera->rot.y) * CAMERA_MOVESPEED;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//S�L�[����
		//���_�̈ړ�
		pCamera->move.x += cosf(pCamera->rot.y) * CAMERA_MOVESPEED;
		pCamera->move.z += sinf(pCamera->rot.y) * CAMERA_MOVESPEED;

	}

	//�ړ��ʂ����Z
	pCamera->posV.x += pCamera->move.x;
	pCamera->posV.z += pCamera->move.z;

	//�ړ��ʂ�������
	pCamera->move.x = 0.0f;
	pCamera->move.z = 0.0f;

	//�����_�ݒ�
	SetCameraR();
}

//==========================================================
//�J�����̎��_�ړ�����
//==========================================================
void MoveCameraV(void)
{
	Camera *pCamera = GetCamera();

	//x���̈ړ�
	if (GetKeyboardPress(DIK_Z) == true && GetKeyboardPress(DIK_C) != true)
	{//Q�L�[����
		pCamera->rot.y += -D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.y < -D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_C) == true && GetKeyboardPress(DIK_Z) != true)
	{//E�L�[����
		pCamera->rot.y += D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.y > D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}

	//z���̈ړ�
	if (GetKeyboardPress(DIK_Y) == true && GetKeyboardPress(DIK_N) != true)
	{//Y�L�[����
		//�p�x�̕ύX
		pCamera->rot.z += -D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
		
	}
	else if (GetKeyboardPress(DIK_N) == true && GetKeyboardPress(DIK_Y) != true)
	{//N�L�[����
		//�p�x�̕ύX
		pCamera->rot.z += D3DX_PI * ROTATE_SPEED;

		if (pCamera->rot.z > MAX_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}

	}

	//�����_����̋����̑���
	if (GetKeyboardPress(DIK_U) == true && GetKeyboardPress(DIK_M) != true)
	{//U�L�[����
		pCamera->fLength -= CAMERA_LENGTHMOVE;

		if (pCamera->fLength < CAMERA_MINLENGTH)
		{//�������ŏ��𒴂����ꍇ
			pCamera->fLength = CAMERA_MINLENGTH;
		}
		
	}
	else if (GetKeyboardPress(DIK_M) == true && GetKeyboardPress(DIK_U) != true)
	{//M�L�[����
		pCamera->fLength += CAMERA_LENGTHMOVE;

		if (pCamera->fLength > CAMERA_MAXLENGTH)
		{//�������ő�𒴂����ꍇ
			pCamera->fLength = CAMERA_MAXLENGTH;
		}
	}

	//���_�ݒ�
	SetCameraV();
}

//==========================================================
//�J�����̒����_�ړ�����
//==========================================================
void MoveCameraR(void)
{
	Camera *pCamera = GetCamera();

	//x���̈ړ�
	if (GetKeyboardPress(DIK_Q) == true && GetKeyboardPress(DIK_E) != true)
	{//Q�L�[����

		//�p�x�̕ύX
		pCamera->rot.y += D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.y > D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_E) == true && GetKeyboardPress(DIK_Q) != true)
	{//E�L�[����

		//�p�x�̕ύX
		pCamera->rot.y += -D3DX_PI * ROTATE_SPEED;

		if (pCamera->rot.y < -D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
	}

	//z���̈ړ�
	if (GetKeyboardPress(DIK_T) == true && GetKeyboardPress(DIK_B) != true)
	{//T�L�[����

		//�p�x�̕ύX
		pCamera->rot.z += D3DX_PI * ROTATE_SPEED;

		if (pCamera->rot.z >  MAX_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}
	}
	else if (GetKeyboardPress(DIK_B) == true && GetKeyboardPress(DIK_T) != true)
	{//B�L�[����
		//�p�x�̕ύX
		pCamera->rot.z += -D3DX_PI * ROTATE_SPEED;
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
	}

	//�����_�ݒ�
	SetCameraR();
}

//==========================================================
//�J�������_�ݒ菈��
//==========================================================
void SetCameraV(void)
{
	Camera *pCamera = GetCamera();

	pCamera->posV.y = pCamera->posR.y + cosf(pCamera->rot.z) * pCamera->fLength;
	pCamera->posV.x = pCamera->posR.x + (sinf(pCamera->rot.z) * cosf(pCamera->rot.y)) * pCamera->fLength;
	pCamera->posV.z = pCamera->posR.z + (sinf(pCamera->rot.z) * sinf(pCamera->rot.y)) * pCamera->fLength;
}

//==========================================================
//�J���������_�ݒ菈��
//==========================================================
void SetCameraR(void)
{
	Camera *pCamera = GetCamera();

	pCamera->posR.y = pCamera->posV.y - cosf(pCamera->rot.z) * pCamera->fLength;
	pCamera->posR.x = pCamera->posV.x - (sinf(pCamera->rot.z) * cosf(pCamera->rot.y)) * pCamera->fLength;
	pCamera->posR.z = pCamera->posV.z - (sinf(pCamera->rot.z) * sinf(pCamera->rot.y)) * pCamera->fLength;
}

//==========================================================
//�}�E�X�ł̃J�����ړ�����
//==========================================================
void MouseCamera(void)
{
	Camera *pCamera = GetCamera();

	if (GetMousePress(MOUSE_LBUTTON) == true && GetMousePress(MOUSE_RBUTTON) == true)
	{//���E�����ɉ�����Ă���Ƃ�
		pCamera->move.x += cosf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * (GetMouseCousorMove().x * MOUSE_MOVESPEED) + -cosf(pCamera->rot.y) * (GetMouseCousorMove().y * MOUSE_MOVESPEED);
		pCamera->move.z += sinf(pCamera->rot.y + (-D3DX_PI * 0.5f)) * (GetMouseCousorMove().x * MOUSE_MOVESPEED) + -sinf(pCamera->rot.y) * (GetMouseCousorMove().y * MOUSE_MOVESPEED);

		//�ړ��ʂ����Z
		pCamera->posV.x += pCamera->move.x;
		pCamera->posV.z += pCamera->move.z;

		//�ړ��ʂ�������
		pCamera->move.x = 0.0f;
		pCamera->move.z = 0.0f;

		//�����_�ݒ�
		SetCameraR();
	}
	else if (GetMousePress(MOUSE_LBUTTON) == true)
	{//���L�[��������Ă���Ƃ�
		pCamera->rot.y -= GetMouseCousorMove().x * MOUSE_ROTATESPEED_X;
		pCamera->rot.z -= GetMouseCousorMove().y * 0.005f;

		if (pCamera->rot.y > D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		else if (pCamera->rot.y < -D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
		else if (pCamera->rot.z > MAX_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}
	}
	else if (GetMousePress(MOUSE_RBUTTON) == true)
	{//�E�L�[��������Ă���Ƃ�
		pCamera->rot.y -= GetMouseCousorMove().x * MOUSE_ROTATESPEED_X;
		pCamera->rot.z -= GetMouseCousorMove().y * MOUSE_ROTATESPEED_Z;

		if (pCamera->rot.y > D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		else if (pCamera->rot.y < -D3DX_PI)
		{//�p�x�����𒴂����ꍇ
			pCamera->rot.y = -D3DX_PI;
			pCamera->rot.y *= -1.0f;
		}
		if (pCamera->rot.z < MIN_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MIN_CAMERA_ROTZ;
		}
		else if (pCamera->rot.z > MAX_CAMERA_ROTZ)
		{//�p�x�����E�𒴂����ꍇ
			pCamera->rot.z = MAX_CAMERA_ROTZ;
		}

		//���_�ݒ�
		SetCameraR();
	}

	//�z�C�[���̎g�p�ʂŋ����̕ύX
	pCamera->fLength += GetMouseCousorMove().z * MOUSE_WHEELSPEED;

	if (pCamera->fLength > CAMERA_MAXLENGTH)
	{//�������ő�𒴂����ꍇ
		pCamera->fLength = CAMERA_MAXLENGTH;
	}
	else if (pCamera->fLength < CAMERA_MINLENGTH)
	{//�������ŏ��𒴂����ꍇ
		pCamera->fLength = CAMERA_MINLENGTH;
	}

	//���_�ݒ�
	SetCameraV();
}

//==========================================================
//�J�����̒Ǐ]�ݒ�
//==========================================================
void CameraMove(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏����擾
	Camera *pCamera = GetCamera();	//�J�����̏����擾

	//�ړI�̒����_�̍��W���擾
	pCamera->posRDest = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y) * 70.0f , pPlayer->pos.y, pPlayer->pos.z - cosf(pPlayer->rot.y) * 70.0f);

	//�ړI�̎��_�̍��W���l��
	pCamera->posVDest = D3DXVECTOR3(
		pCamera->posRDest.x + (sinf(pCamera->rot.z) * cosf(pCamera->rot.y)) * pCamera->fLength,
		pCamera->posRDest.y + cosf(pCamera->rot.z) * pCamera->fLength,
		pCamera->posRDest.z + (sinf(pCamera->rot.z) * sinf(pCamera->rot.y)) * pCamera->fLength);

	//�����_�̕␳
	D3DXVECTOR3 RDiff = D3DXVECTOR3(pCamera->posRDest.x - pCamera->posR.x, pCamera->posRDest.y - pCamera->posR.y, pCamera->posRDest.z - pCamera->posR.z);
	pCamera->posR += RDiff * 0.2f;

	//���_�̕␳
	D3DXVECTOR3 VDiff = D3DXVECTOR3(pCamera->posVDest.x - pCamera->posV.x, pCamera->posVDest.y - pCamera->posV.y, pCamera->posVDest.z - pCamera->posV.z);
	pCamera->posV += VDiff * 0.15f;
}

//==========================================================
//�J�����̌����̐ݒ�
//==========================================================
void SetRotCamera(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏����擾
	Camera *pCamera = GetCamera();	//�J�����̏����擾
	float fRotDiff;

	pCamera->rotDest.x = -pPlayer->rot.y + (D3DX_PI * 0.5f);	//�ړI�̌������擾

	fRotDiff = pCamera->rotDest.x - pCamera->rot.y;

	if (fRotDiff > D3DX_PI || fRotDiff < -D3DX_PI)
	{//-3.14�`3.14�͈̔͊O�̏ꍇ
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff += (-D3DX_PI * 2);
		}
		else if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += (D3DX_PI * 2);
		}
	}

	pCamera->rot.y += fRotDiff * 0.02f;

	if (pCamera->rot.y > D3DX_PI || pCamera->rot.y < -D3DX_PI)
	{//-3.14�`3.14�͈̔͊O�̏ꍇ
		if (pCamera->rot.y > D3DX_PI)
		{
			pCamera->rot.y += (-D3DX_PI * 2);
		}
		else if (pCamera->rot.y  < -D3DX_PI)
		{
			pCamera->rot.y += (D3DX_PI * 2);
		}
	}

	//���_�̐ݒ�
	SetCameraV();

}
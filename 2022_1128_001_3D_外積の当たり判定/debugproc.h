//==========================================================
//
//�f�o�b�O�\������
//Author ����ɐ�
//
//==========================================================
#ifndef _DEBUGPROC_H_		//���̃}�N������`����Ă��Ȃ��ꍇ
#define _DEBUGPROC_H_

//==========================================================
//�X�C�b�`�񋓌^�̒�`
//==========================================================
typedef enum
{
	DEBUGTYPE_OFF = 0,
	DEBUGTYPE_ON,
	DEBUGTYPE_MAX
}DEBUGTYPE;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void PrintDebugProc(const char *fmt, ...);

#endif
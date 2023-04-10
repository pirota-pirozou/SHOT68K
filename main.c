// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�T���v���v���O�����̋N������ main.c �ł���B
//

#include <stdlib.h>
#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"
#include "mylib.h"
#include "CMSprite.h"
#include "GamePadManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"

//#define PAD_TEST		// �R�����g���O���ƃQ�[���p�b�h�̒l�\���e�X�g

// �V�[���̓o�^�e�[�u��
static const SSceneWork sceneTable[] =
 {
	{ Title_Init, Title_Update, Title_Draw, Title_VSync, Title_Clear },		// �^�C�g���V�[��
	{ Game_Init, Game_Update, Game_Draw, Game_VSync, Game_Clear }			// �Q�[���V�[��
};

// �v���g�^�C�v�錾
BOOL load_data(void);

/* main */
int main(int argc, char *argv[])
{
	super_begin();			// �X�[�p�[�o�C�U�[���[�h��

	screen_init();			// ��ʏ�����

	gcls(0);				// �O���t�B�b�N��ʃN���A�O
	gcls(1);				// �O���t�B�b�N��ʃN���A�P

	BOOL bSuccess;
	bSuccess = load_data();		// �f�[�^�̓ǂݍ���

	// �f�[�^�̓ǂݍ��݂Ɏ��s������I��
	if (!bSuccess)
	{
		goto FORCE_QUIT;
	}

//	printf("��ʂ��؂�ւ��܂����B\n");

//	INKEY();				// �L�[���͑҂�

	CM_sprite_on();			// �X�v���C�g�\���Ǘ��n�m

	// �Q�[���p�b�h�}�l�[�W���[�̏�����
	GamePadManager_Init();

	// �V�[���}�l�[�W���[�̏�����
	SceneManager_Init();
	SceneManager_Regist(sceneTable);

	// �V�[���̐؂�ւ�
	SceneManager_ChangeScene(SCENE_ID_TITLE);

	// �Q�[���p�b�h�̃e�X�g
#ifdef PAD_TEST
	unsigned short pad = 0;
	unsigned char strtmp[128];
#endif

	// ���C�����[�v�e�X�g
	for (;;)
	{
		if (BITSNS(0) & 2) 	// ESC�L�[�Ŕ�����
		{
			break;
		}
		GamePadManager_Update();	// �Q�[���p�b�h�̍X�V

#ifdef PAD_TEST
		pad = GamePadManager_GetPad();
		sprintf(strtmp, "PAD=%02X", pad);
		CM_bg_puts(strtmp, 0, 8, 1);
#endif

		Scene_Update();		// �V�[���̍X�V
		Scene_Draw();		// �V�[���̕`��
		// VSync�̑҂�
//		vsync();
		CM_vsync();			// VSync�҂�
		Scene_VSync();		// �V�[����VSync��̏���
	}
	Scene_Clear();			// �Q�[�����[�v���甲����Ƃ��V�[���̃N���A

	CM_sprite_off();		// �X�v���C�g�\���Ǘ��n�e�e

	// �v���O�����I��
	// �v���O������Abort�A�h���X�������ɂ����ɐݒ肷��
FORCE_QUIT:
	asm volatile (".xdef _PRG_QUIT\n");
	asm volatile ("_PRG_QUIT:\n");
	super_end();					// ���[�U�[���[�h�֕��A

	printf("�v���O�����̎��s���I�����܂����B\n");

	return 0;
}

// �f�[�^�̓ǂݍ���
// �X�v���C�g�p�^�[���ƃp���b�g�f�[�^��ǂݍ��ݒ�`����
// ����: �Ȃ�
// Return: TRUE:���� FALSE:���s
BOOL load_data(void)
{
	FILE *fp = NULL;
	pPX2FILE px2buf = (pPX2FILE) -1;
	BOOL result = TRUE;

	// PX2�t�@�C���̓ǂݍ���
	do
	{
		px2buf = (pPX2FILE)MALLOC(sizeof(PX2FILE));
		if (px2buf < 0)
		{
			printf("px2buf: ���������m�ۂł��܂���B\n");
			result = FALSE;
			break;
		}
		// PX2�t�@�C���̓ǂݍ���
		fp = fopen("shoot.px2", "rb");
		if (fp == NULL)
		{
			printf("�X�v���C�g�t�@�C�����J���܂���B\n");
			result = FALSE;
			break;
		}
		int a = fread(px2buf, sizeof(PX2FILE), 1, fp);
		if (a != 1)
		{
			printf("�X�v���C�g�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n");
			result = FALSE;
			break;
		}
		// �X�v���C�g�p�^�[���ƃp���b�g�̒�`
		CM_def_px2(px2buf);
		break;
	} while (0);

	if (fp != NULL)
	{
		// �t�@�C���N���[�Y
		fclose(fp);
		fp = NULL;
	}
	if (px2buf != (pPX2FILE) -1)
	{
		// ���������
		MFREE((int)px2buf);
		px2buf = (pPX2FILE) -1;
	}

	return result;
}

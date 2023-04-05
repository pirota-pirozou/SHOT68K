// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�T���v���v���O�����̋N������ main.c �ł���B
//

#include <stdlib.h>
#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"
#include "mylib.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"

// �V�[���̓o�^�e�[�u��
static SSceneWork sceneTable[] =
 {
	{ Title_Init, Title_Update, Title_Draw, Title_VSync, Title_Clear },
	{ Game_Init, Game_Update, Game_Draw, Game_VSync, Game_Clear }
};

/* main */
int main(int argc, char *argv[])
{
	super_begin();			// �X�[�p�[�o�C�U�[���[�h��

	screen_init();			// ��ʏ�����

	gcls(0);				// �O���t�B�b�N��ʃN���A�O
	gcls(1);				// �O���t�B�b�N��ʃN���A�P

	printf("��ʂ��؂�ւ��܂����B\n");

	// �������m�ۂ̃e�X�g
	// DOS�R�[���̃������m�ۊ֐����g�p����B�W����malloc()�͓��삪�ςȋC������B
	// �Ȃ����߂�l��int�^�ɂȂ��Ă���̂ŁA�L���X�g���Ďg�p����
	char *buf1 = (char*)MALLOC(0xA000);
	if (buf1 < 0)
	{
		printf("buf1: ���������m�ۂł��܂���B\n");
	}

	char *buf2 = (char*)MALLOC(0x2000);
	if (buf2 < 0)
	{
		printf("buf2: ���������m�ۂł��܂���B\n");
	}
	char *buf3 = (char*)MALLOC(0x8000);
	if (buf3 < 0)
	{
		printf("buf3: ���������m�ۂł��܂���B\n");
	}

	// ����������̃e�X�g
	// �t���ɉ������
	// �Ȃ���������int�^�ɂȂ��Ă���̂ŁA�L���X�g���Ďg�p����
	MFREE((int)buf3);
	MFREE((int)buf2);
	MFREE((int)buf1);

//	INKEY();				// �L�[���͑҂�

	// �V�[���}�l�[�W���[�̏�����
	SceneManager_Init();
	SceneManager_Regist(sceneTable);

	// �V�[���̐؂�ւ�
	SceneManager_ChangeScene(SCENE_ID_TITLE);

	// �Q�[���p�b�h�̃e�X�g
	int pad = 0;
	// ���C�����[�v�e�X�g
	for (;;)
	{
		if (BITSNS(0) & 2) 	// ESC�L�[�Ŕ�����
		{
			break;
		}
		pad = gamepad();
		printf("pad = %02X\n", pad);

		Scene_Update();		// �V�[���̍X�V
		Scene_Draw();		// �V�[���̕`��
		// VSync�̑҂�
		vsync();
		Scene_VSync();		// �V�[���̐���������̏���
	}
	Scene_Clear();			// �Q�[�����[�v���甲����Ƃ��V�[���̃N���A

	// �v���O�����I��
	// �v���O������Abort�A�h���X�������ɂ����ɐݒ肷��
	asm volatile (".xdef _PRG_QUIT\n");
	asm volatile ("_PRG_QUIT:\n");
	super_end();					// ���[�U�[���[�h�֕��A

	printf("�v���O�����̎��s���I�����܂����B\n");

	return 0;
}

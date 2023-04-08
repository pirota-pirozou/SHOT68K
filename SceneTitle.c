// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�^�C�g���V�[���̃R�[�h�{�� �ł���B
// Path: SceneTitle.c
//

#include "SceneTitle.h"

extern int score;                  // �X�R�A
extern int hiscore;                // �n�C�X�R�A

// �^�C�g���V�[���@������
void Title_Init(void)
{
//    printf("Title_Init() ");
    BGTEXTCL(1, 0x0100);	// BGTEXT1 CLR
}

// �^�C�g���V�[���@�X�V
void Title_Update(void)
{
//    printf(" TITLE_Update() ");
    CM_bg_puts("TITLE_UPDATE()", 0, 1, 1);
    // �Q�[���p�b�h�̒l���擾
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if (pad_trg & PAD_A)
    {
        // A�{�^���ŃQ�[���V�[���֑J��
        SceneManager_ChangeScene(SCENE_ID_GAME);
    }
}

// �^�C�g���V�[���@�`��
void Title_Draw(void)
{
//    printf("Title_Draw()\n");
    CM_bg_puts("TITLE_DRAW()", 0, 2, 1);

    CM_sp_set(0, 50, 50, 0x0140, 3);
//					*���P���X�v���C�g�v���[���ԍ��i�O�`�P�Q�V�j
//					*���Q�������W
//					*���R�������W
//					*���S���p�^�[���R�[�h %HR_VR_X_X_COLR_SPATSPAT : ����8bit:�p�^�[�� / 8-12bit:�J���[
//					*���T���v���C�I���e�B %0000_0000_0000_0_0_PR : 00 �\�����Ȃ� / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

}

// �^�C�g���V�[���@VSync
void Title_VSync(void)
{
//    printf(" Title_VSync()\n");
    CM_bg_puts("TITLE_VSYNC()", 0, 3, 1);
}

// �^�C�g���V�[���@�N���A�i�I���j
void Title_Clear(void)
{
//    printf("**Title_Clear()\n");
}

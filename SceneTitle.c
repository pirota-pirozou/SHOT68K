// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�^�C�g���V�[���̃R�[�h�{�� �ł���B
// Path: SceneTitle.c
//

#include "SceneTitle.h"
#include "BMPLoad256.h"

extern pBMPFILE256 pBmpBackTitle;	// BMP�t�@�C���f�[�^�i�^�C�g���j

extern int score;                  // �X�R�A
extern int hiscore;                // �n�C�X�R�A

static int cou;                    // �J�E���^

/// @brief �^�C�g���V�[���@������
void Title_Init(void)
{
    char strtmp[128];

    CM_sprite_clr();            // �X�v���C�g�N���A
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR

    // �O���t�B�b�N��ʂO�̃N���A
    gcls(0);
    // �r�b�g�}�b�v�t�@�C���̓ǂݍ���
    PutBMPMemory256(pBmpBackTitle);

    sprintf(strtmp, "SCORE %08d", score);
    CM_bg_puts(strtmp, 0, 0, 1);
    sprintf(strtmp, "HI %08d", hiscore);
    CM_bg_puts(strtmp, 19, 0, 1);

    CM_bg_puts("<SAMPLE GAME:SHOT68K>", 4,  6, 1);
    CM_bg_puts("PROGRAMMED BY PIROTA.", 4, 29, 1);

    CM_bg_puts("VER 0.2", 24, 31, 1);

    zmsc_stop();                // �T�E���h��~
    cou = 0;
}

/// @brief �^�C�g���V�[���@�X�V
void Title_Update(void)
{
    // �Q�[���p�b�h�̒l���擾
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if ( (pad_trg & (PAD_A|PAD_B)) ||
         ((pad_trg & PAD_START) == PAD_START) )
    {
        // A,B,START�{�^���ŃQ�[���V�[���֑J��
        SceneManager_ChangeScene(SCENE_ID_GAME);
    }

    // �J�E���^���C���N�������g
    cou++;
}

/// @brief �^�C�g���V�[���@�`��
void Title_Draw(void)
{
//    CM_bg_puts("TITLE_DRAW()", 0, 2, 1);

//    CM_sp_set(0, 50, 50, 0x0140, 3);
//					*���P���X�v���C�g�v���[���ԍ��i�O�`�P�Q�V�j
//					*���Q�������W
//					*���R�������W
//					*���S���p�^�[���R�[�h %HR_VR_X_X_COLR_SPATSPAT : ����8bit:�p�^�[�� / 8-12bit:�J���[
//					*���T���v���C�I���e�B %0000_0000_0000_0_0_PR : 00 �\�����Ȃ� / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

}

/// @brief �^�C�g���V�[���@VSync
void Title_VSync(void)
{
    if ((cou & 32)==0)
    {
        CM_bg_puts("PRESS BUTTON TO START.", 4, 16, 1);
    }
    else
    {
        CM_bg_puts("                      ", 4, 16, 1);
    }

}

// @brief �^�C�g���V�[���@�N���A�i�I���j
void Title_Clear(void)
{
//    printf("**Title_Clear()\n");
}

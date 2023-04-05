// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���p�b�h�}�l�[�W���[�̃\�[�X�t�@�C�� �ł���B
// GamePadManager.c
//
// ���̃R�[�h�́A�Q�[���p�b�h�̓��͂��Ǘ�����B
//
#include "GamePadManager.h"

// �Q�[���p�b�h�̒l
static unsigned short pad;
static unsigned short pad_bak;
static unsigned short pad_trg;

// �Q�[���p�b�h�}�l�[�W���[�@������
void GamePadManager_Init(void)
{
    printf("GamePadManager_Init()\n");

    pad = pad_bak = pad_trg = 0;
}

// �Q�[���p�b�h�}�l�[�W���[�@�X�V
void GamePadManager_Update(void)
{
//    printf("GamePadManager_Update()\n");

    // �Q�[���p�b�h�̒l���擾
    pad = (unsigned short) gamepad();

    // �{�^���������ꂽ��u�̏�Ԃ��L�^
    int tmppad = pad;
    pad_trg = ~pad_bak & tmppad;
    pad_bak = tmppad;
}

// �Q�[���p�b�h�}�l�[�W���[�@�p�b�h��Ԏ擾
unsigned short GamePadManager_GetPad(void)
{
//    printf("GamePadManager_GetPad()\n");
    return pad;
}

// �Q�[���p�b�h�}�l�[�W���[�@pad�̃g���K�[��Ԃ��擾
unsigned short GamePadManager_GetTrigger(void)
{
//    printf("GamePadManager_GetTrigger()\n");
    return pad_trg;
}

// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���p�b�h�}�l�[�W���[�̃\�[�X�t�@�C�� �ł���B
// GamePadManager.c
//
// ���̃R�[�h�́A�Q�[���p�b�h�̓��͂��Ǘ�����B
//
#include "GamePadManager.h"

// �Q�[���p�b�h�̒l
static uint16 pad;
static uint16 pad_bak;
static uint16 pad_trg;

// �Q�[���p�b�h�}�l�[�W���[�@������
FORCE_INLINE void GamePadManager_Init(void)
{
    pad = pad_bak = pad_trg = 0;
}

// �Q�[���p�b�h�}�l�[�W���[�@�X�V
void GamePadManager_Update(void)
{
    // �Q�[���p�b�h�̒l���擾
    pad = (uint16) gamepad();

    // �{�^���������ꂽ��u�̏�Ԃ��L�^
    int tmppad = pad;
    pad_trg = ~pad_bak & tmppad;
    pad_bak = tmppad;
}

// �Q�[���p�b�h�}�l�[�W���[�@�p�b�h��Ԏ擾
FORCE_INLINE uint16 GamePadManager_GetPad(void)
{
    return pad;
}

// �Q�[���p�b�h�}�l�[�W���[�@pad�̃g���K�[��Ԃ��擾
FORCE_INLINE uint16 GamePadManager_GetTrigger(void)
{
    return pad_trg;
}

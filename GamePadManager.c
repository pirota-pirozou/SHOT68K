// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���p�b�h�}�l�[�W���[�̃\�[�X�t�@�C�� �ł���B
// GamePadManager.c
//
// ���̃R�[�h�́A�Q�[���p�b�h�̓��͂��Ǘ�����B
//

#include "GamePadManager.h"

// �Q�[���p�b�h�̒l
uint16 pad;
uint16 pad_trg;
static uint16 pad_bak;

// �Q�[���p�b�h�}�l�[�W���[�@������
void GamePadManager_Init(void)
{
    pad = pad_bak = pad_trg = 0;
}

// �Q�[���p�b�h�}�l�[�W���[�@�X�V
void GamePadManager_Update(void)
{
    // �Q�[���p�b�h�̒l���擾
    pad = (uint16) gamepad();

    // �{�^���������ꂽ��u�̏�Ԃ��L�^
    uint16 tmppad = pad;
    pad_trg = ~pad_bak & tmppad;
    pad_bak = tmppad;
}

// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���p�b�h�}�l�[�W���[�̃w�b�_�t�@�C�� �ł���B
// GamePadManager.h
#pragma once
#ifndef __GAMEPADMANAGER_H__
#define __GAMEPADMANAGER_H__

#include <stdio.h>
#include "CF_MACRO.h"
#include "types.h"
#include "mylib.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint16 pad;
extern uint16 pad_trg;

// �}�N����`��֐��v���g�^�C�v�錾�������ɋL�q����
void GamePadManager_Init(void);
void GamePadManager_Update(void);

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

#ifdef __cplusplus
}
#endif

#endif // __GAMEPADMANAGER_H__

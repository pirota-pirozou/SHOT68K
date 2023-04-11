// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���p�b�h�}�l�[�W���[�̃w�b�_�t�@�C�� �ł���B
// GamePadManager.h
#pragma once
#ifndef __GAMEPADMANAGER_H__
#define __GAMEPADMANAGER_H__

#include <stdio.h>
#include "types.h"
#include "mylib.h"

#ifdef __cplusplus
extern "C" {
#endif

// �}�N����`��֐��v���g�^�C�v�錾�������ɋL�q����
void GamePadManager_Init(void);
void GamePadManager_Update(void);
uint16 GamePadManager_GetPad(void);
uint16 GamePadManager_GetTrigger(void);


#ifdef __cplusplus
}
#endif

#endif // __GAMEPADMANAGER_H__

// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�n�a�i�}�l�[�W���[�̃w�b�_�t�@�C�� �ł���B
//
#pragma once
#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__

#include <stdlib.h>
#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"
#include "GamePadManager.h"
#include "CMSprite.h"


// �v���g�^�C�v�錾
void OBJManager_Init();
void ObjManager_Make();
void ObjManager_Update();
void ObjManager_Draw();
void ObjManager_End();

#endif // __OBJMANAGER_H__

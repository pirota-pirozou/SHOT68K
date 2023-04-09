// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�n�a�i�}�l�[�W���[�̃w�b�_�t�@�C�� �ł���B
//
#pragma once
#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"

#include "GamePadManager.h"
#include "SceneManager.h"
#include "CMSprite.h"

// �I�u�W�F�N�g�̍ő吔
#define OBJ_MAX 128

// �I�u�W�F�N�gID�̒�`
enum
{
    OBJ_ID_EMPTY = 0,
    OBJ_ID_PLAYER,
    OBJ_ID_PBULLET,
    OBJ_ID_EBULLET,
    OBJ_ID_ENEMY1,
    OBJ_ID_ENEMY2,
    OBJ_ID_ENEMY3,
    OBJ_ID_EEFFECT,
    OBJ_ID_PEFFECT,
    OBJ_ID_MAX
};

// �\���̂̒�`

/// @brief �I�u�W�F�N�g�̊�{�\����
/// @note 128�o�C�g�ɂȂ�悤�Ƀp�f�B���O�����Ă���

typedef struct _tObj SObj;
typedef void (*ObjFuncPtr)(SObj *);

struct _tObj
{
    uint16 id;              // �I�u�W�F�N�gID
    uint16 x;               // X���W
    uint16 y;               // Y���W
    uint16 pat;             // �p�^�[���ԍ�
    uint16 plane;           // �v���[���ԍ�
    uint16 pad0;
    ObjFuncPtr Update;      // �X�V�֐�
    ObjFuncPtr Draw;        // �`��֐�

    uint8  padding[128-20]; // 128�o�C�g�ɂȂ�悤�Ƀp�f�B���O
};
typedef SObj* pSObj;

/// @brief �v���C���[�̍\����
typedef struct
{
    uint16 id;              // �I�u�W�F�N�gID
    uint16 x;               // X���W
    uint16 y;               // Y���W
    uint16 pat;             // �p�^�[���ԍ�
    uint16 plane;           // �v���[���ԍ�
    uint16 pad0;

    ObjFuncPtr Update;      // �X�V�֐�
    ObjFuncPtr Draw;        // �`��֐�

    uint8  padding[128-20]; // 128�o�C�g�ɂȂ�悤�Ƀp�f�B���O
} SObjPlayer, *pSObjPlayer;

/// @brief �v���C���[�e�̍\����
typedef struct
{
    uint16 id;              // �I�u�W�F�N�gID
    uint16 x;               // X���W
    uint16 y;               // Y���W
    uint16 pat;             // �p�^�[���ԍ�
    uint16 plane;           // �v���[���ԍ�
    uint16 pad0;

    ObjFuncPtr Update;      // �X�V�֐�
    ObjFuncPtr Draw;        // �`��֐�

    uint8  padding[128-20]; // 128�o�C�g�ɂȂ�悤�Ƀp�f�B���O
} SObjPBullet, *pSObjPBullet;

// �v���g�^�C�v�錾
void OBJManager_Init();
pSObj ObjManager_Make(int id, int x, int y);
void ObjManager_Update();
void ObjManager_Draw();
void ObjManager_End();



void ObjManager_Destroy(pSObj pObj);


#endif // __OBJMANAGER_H__

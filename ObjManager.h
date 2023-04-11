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

// switch-case �œ��B���Ȃ��}�N��
#ifdef __GNUC__
#define __UNREACHABLE__ __builtin_unreachable()
#endif

#ifdef _MSC_VER
#define __UNREACHABLE__ __assume(0)
#endif

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

/// @brief �I�u�W�F�N�g�̊�{�\����
/// @note �S���̃I�u�W�F�N�g�ł��̍\���̂��g�p����
struct _tObj
{
    uint16 id;              // �I�u�W�F�N�gID
    int16 x;                // X���W
    int16 y;                // Y���W
    uint16 pat;             // �p�^�[���ԍ�
    uint16 plane;           // �v���[���ԍ�
    uint16 pad0;
    ObjFuncPtr Update;      // �X�V�֐�
    ObjFuncPtr Draw;        // �`��֐�
    uint16 anm_spd;         // �A�j���[�V�������x
    uint16 anm_num;         // �A�j���[�V��������
    uint16 anm_cou;         // �A�j���[�V���^�C�~���O�J�E���^
    uint16 anm_idx;         // �A�j���[�V�����C���f�b�N�X

    uint8  padding[128-28]; // 128�o�C�g�ɂȂ�悤�Ƀp�f�B���O
};
typedef SObj* pSObj;

extern pSObj g_pObj; // �I�u�W�F�N�g�̃|�C���^

/////////////////////////////////
// �n�a�i�}�l�[�W���[�F�擾����
/////////////////////////////////
/// @brief �I�u�W�F�N�g�̃|�C���^���擾����
/// @param i �I�u�W�F�N�g�̃C���f�b�N�X
/// @retval �I�u�W�F�N�g�̃|�C���^
FORCE_INLINE pSObj ObjManager_GetObj(int i)
{
    return &g_pObj[i];
}

// �v���g�^�C�v�錾
void OBJManager_Init();
pSObj ObjManager_Make(int id, int x, int y);
void ObjManager_Update();
void ObjManager_Draw();
void ObjManager_End();

void ObjManager_Destroy(pSObj pObj);

int ObjManager_FindEnemyIdx(void);
int ObjManager_FindEnemyNextIdx(int);

#endif // __OBJMANAGER_H__

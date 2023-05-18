// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�T�E���h�}�l�[�W���[�̃w�b�_�t�@�C�� �ł���B
//
#pragma once
#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include <stdint.h>
#include "CF_MACRO.h"
#include "types.h"
#include "zmsclib.h"

// BGM�̒�`
enum
{
    BGM_TITLE = 0,
//    BGM_GAME,
    BGM_MAX = 0
};

// SE�̒�`
enum
{
    SE_EBOMB = 0,   // �G�̔�����
    SE_PSHOT,       // ���@�̃V���b�g��
    SE_PBOMB,       // ���@�̔�����
    SE_MAX
};

// �T�E���hID�̒�`

// �\���̂̒�`
typedef struct {
    int size;
    void *data;
} ZMDdata, *pZMDdata;

#ifdef __cplusplus
extern "C" {
#endif

// �v���g�^�C�v�錾
void SoundManager_Init(void);
void SoundManager_Load(void);
void SoundManager_PlayBGM(int);
void SoundManager_PlaySE(int);
void SoundManager_End(void);

#ifdef __cplusplus
}
#endif

#endif // __SOUNDMANAGER_H__

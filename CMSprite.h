// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// X68000 �X�v���C�g���C�u���� C����w�b�_
// 2023/04/06 �쐬
// by Pirota Pirozou
//
// ���̃��C�u�����́AX68000�̃X�v���C�g���������߂̃��C�u�����ł��B

#pragma once
#ifndef __CMSprite_H__
#define __CMSprite_H__

#include "types.h"

// PX2�t�@�C���\����
typedef struct
{
	uint16 atr[256];	    // �c�[���p�A�g���r���[�g
	uint16 pal[256];	    // X68K�p���b�g
	uint8  sprpat[0x8000];	// X68K�X�v���C�g�p�^�[��
} PX2FILE, * pPX2FILE;

#ifdef __cplusplus
extern "C" {
#endif

// �v���g�^�C�v�錾
void CM_def_px2(pPX2FILE);
void CM_def_grp_palette(uint16*);



#ifdef __cplusplus
}
#endif

#endif // __CMSprite_H__

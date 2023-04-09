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
void CM_sprite_on(void);
void CM_sprite_off(void);
void CM_sprite_clr(void);
void CM_vsync(void);
void CM_sp_set32(int, int, int, int, int);
//					*���P���X�v���C�g�v���[���ԍ��i�O�`�P�Q�V�j
//					*���Q�������W
//					*���R�������W
//					*���S���p�^�[���R�[�h %HR_VR_X_X_COLR_SPATSPAT : ����8bit:�p�^�[�� / 8-12bit:�J���[
//					*���T���v���C�I���e�B %0000_0000_0000_0_0_PR : 00 �\�����Ȃ� / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1
void CM_sp_set(int, int, int, int, int);
//					*���P���X�v���C�g�v���[���ԍ��i�O�`�P�Q�V�j
//					*���Q�������W
//					*���R�������W
//					*���S���p�^�[���R�[�h %HR_VR_X_X_COLR_SPATSPAT : ����8bit:�p�^�[�� / 8-12bit:�J���[
//					*���T���v���C�I���e�B %0000_0000_0000_0_0_PR : 00 �\�����Ȃ� / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

void CM_parts_wrt(int, int, int, int);
//					*���P���a���y�[�W
//					*���Q�������W
//					*���R�������W
//					*���S���p�^�[���R�[�h(0~255)

void CM_parts_clr(int, int, int);
//* ���P���a���y�[�W
//* ���Q�������W
//* ���R�������W

void CM_dma32(int, int, char *, char *);
//* d1=#%0000_0101(��۸��ϰ�ޥ�ƭ�َQ��)*
//* d2=�J�E���^                         *
//* a1=�]�����i�\�[�X�E�������A�h���X�j *
//* a2=�]����i�ި�èȰ��ݥ�h�^�n�|�[�g)*

void CM_wait_dma_end(void);

void CM_def_px2(pPX2FILE);
void CM_def_grp_palette(uint16*);
void CM_bg_puts(const char *, int, int, int);



#ifdef __cplusplus
}
#endif

#endif // __CMSprite_H__

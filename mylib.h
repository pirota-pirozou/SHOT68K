// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// �I���I�����C�u���� For X68000
// By Pirota Pirozou, 2022-2023
//
// ���̃R�[�h�́A����ƕ֗��Ȋ֐����܂Ƃ߂����̂ł��B
// ���̃��C�u�����́A���R�ɉ��ς��Ă�����č\���܂���B
// �������A���ς������̂����J����ꍇ���A���̃��C�u�����̒��쌠�\���͕K�v����܂���B
// �܂��A���̃��C�u�������g�p�����v���O�����̒��쌠�́A
// ���̃v���O�����̒���҂ɂ���܂��B
//
// This code is a collection of functions that are useful to have.
// This library may be freely modified.
// However, even if you publish the modified version, you do not need to indicate the copyright of this library.
// The copyright of any program that uses this library belongs to the author of the program,
// the authors of the programs that use this library.

#pragma once
#ifndef __MYLIB_H__
#define __MYLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

// gamepad(); �p�̒萔
#define PAD_UP		0x01
#define PAD_DOWN	0x02
#define PAD_LEFT	0x04
#define PAD_RIGHT	0x08
#define PAD_A		0x20
#define PAD_B		0x40

#define PAD_SELECT	0x03            // XBOX360�R���g���[����MENU�{�^��
#define PAD_START	0x0C            // ���K�h���~�j�p�b�h��START�{�^��
#define PAD_X   	0x60            // XBOX360�R���g���[����X�{�^��
//#define PAD_Y   	XM6g�ł͑Ή�����

// �v���g�^�C�v�錾
void super_begin(void);
void super_end(void);
void screen_init(void);
void vsync(void);
void gcls(int page);
int gamepad(void);
void TX_CLS(void);

#ifdef __cplusplus
}
#endif

#endif // __MYLIB_H__

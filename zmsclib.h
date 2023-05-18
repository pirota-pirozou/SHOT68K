// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// X68000 Z-MUSIC�R���g���[�����C�u���� C����w�b�_
// 2023/05/17 �쐬
// by Pirota Pirozou
//
// ���̃��C�u�����́AZ-MUSIC�Ō��ʉ��≹�y��炷���߂̃��C�u�����ł��B

#pragma once
#ifndef __ZMSCLIB_H__
#define __ZMSCLIB_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// �v���g�^�C�v�錾
void zmsc_init(void);
void zmsc_play(void *);
void zmsc_stop(void);
void zmsc_cont(void);
void zmsc_fadeout(void);
void zmsc_seplay(void *);
int zmsc_keepchk(void);


#ifdef __cplusplus
}
#endif

#endif // __ZMSCLIB_H__

// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// Written by Pirota Pirozou, 2023/04
// CF_MACRO.h
//
// ���̃R�[�h�́AC����p�̃}�N����`�̃w�b�_�t�@�C�� �ł���B
// xdev68k���œ��삷��悤�ɍ���Ă���B
//
// ���R�ɉ��ς��Ď����̃v���O�����ɑg�ݍ��ނ��Ƃ��\�B
// ���쌠�\�L���K�v�Ȃ��B
// �������A���̃R�[�h���g�p�������Ƃɂ�邢���Ȃ鑹�Q�ɂ��Ă��A��҂͈�؂̐ӔC�𕉂�Ȃ��B

#pragma once
#ifndef ___CF_MACRO_H___
#define ___CF_MACRO_H___

// xdev68k����malloc()/free()���g���Ƃ��܂����삵�Ȃ��̂ŁA
// ����ɁADOS�R�[����MALLOC()/MFREE()���g���֐����g�p���܂��B
// ��allmem() �ɂ��ʏ��malloc()/free()���g����̂ŃR�����g�A�E�g
//#define dos_malloc(siz)    	MALLOC((siz))
//#define dos_free(ptr)   	MFREE((int)(ptr))

#define FORCE_INLINE __attribute__((__always_inline__)) inline

// switch-case �œ��B���Ȃ��}�N��
#ifdef __GNUC__
#define __UNREACHABLE__ __builtin_unreachable()
#endif

#ifdef _MSC_VER
#define __UNREACHABLE__ __assume(0)
#endif





#endif // ___CF_MACRO_H___

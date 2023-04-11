// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃w�b�_�[�t�@�C���́A��{�I�Ȍ^��`���s���܂��B
// �N�������Ă��A�����悤�Ȍ^��`������̂ŁA���̃��C�u�����̒��쌠�\���͕K�v����܂���B
// �܂��A���̃��C�u�������g�p�����v���O�����̒��쌠�́A
// ���̃v���O�����̒���҂ɂ���܂��B
//
// This header file provides the basic type definitions.
// No copyright notice is required for this library, since the type definitions will be the same no matter who writes it.
// Also, the copyright of the program using this library belongs to the author of that program,
// the author of that program.

#pragma once
#ifndef __TYPES_H__
#define __TYPES_H__

// ��{�^
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned long uint32;
typedef signed long int32;
typedef unsigned long long uint64;
typedef signed long long int64;

typedef unsigned char BOOL;
#define TRUE 1
#define FALSE 0

//
#define FORCE_INLINE __attribute__((__always_inline__)) inline

#endif // __TYPES_H__

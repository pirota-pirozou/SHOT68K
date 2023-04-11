// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このヘッダーファイルは、基本的な型定義を行います。
// 誰が書いても、同じような型定義をするので、このライブラリの著作権表示は必要ありません。
// また、このライブラリを使用したプログラムの著作権は、
// そのプログラムの著作者にあります。
//
// This header file provides the basic type definitions.
// No copyright notice is required for this library, since the type definitions will be the same no matter who writes it.
// Also, the copyright of the program using this library belongs to the author of that program,
// the author of that program.

#pragma once
#ifndef __TYPES_H__
#define __TYPES_H__

// 基本型
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

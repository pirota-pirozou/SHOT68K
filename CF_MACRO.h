// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// Written by Pirota Pirozou, 2023/04
// CF_MACRO.h
//
// このコードは、C言語用のマクロ定義のヘッダファイル である。
// xdev68k環境で動作するように作られている。
//
// 自由に改変して自分のプログラムに組み込むことが可能。
// 著作権表記も必要ない。
// ただし、このコードを使用したことによるいかなる損害についても、作者は一切の責任を負わない。

#pragma once
#ifndef ___CF_MACRO_H___
#define ___CF_MACRO_H___

// xdev68k環境でmalloc()/free()を使うとうまく動作しないので、
// 代わりに、DOSコールのMALLOC()/MFREE()を使う関数を使用します。
// ※allmem() により通常のmalloc()/free()が使えるのでコメントアウト
//#define dos_malloc(siz)    	MALLOC((siz))
//#define dos_free(ptr)   	MFREE((int)(ptr))

#define FORCE_INLINE __attribute__((__always_inline__)) inline

// switch-case で到達しないマクロ
#ifdef __GNUC__
#define __UNREACHABLE__ __builtin_unreachable()
#endif

#ifdef _MSC_VER
#define __UNREACHABLE__ __assume(0)
#endif





#endif // ___CF_MACRO_H___

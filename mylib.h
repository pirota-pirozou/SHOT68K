// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// オレオレライブラリ For X68000
// By Pirota Pirozou, 2022-2023
//
// このコードは、あると便利な関数をまとめたものです。
// このライブラリは、自由に改変してもらって構いません。
// ただし、改変したものを公開する場合も、このライブラリの著作権表示は必要ありません。
// また、このライブラリを使用したプログラムの著作権は、
// そのプログラムの著作者にあります。
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

// gamepad(); 用の定数
#define PAD_UP		0x01
#define PAD_DOWN	0x02
#define PAD_LEFT	0x04
#define PAD_RIGHT	0x08
#define PAD_A		0x20
#define PAD_B		0x40

#define PAD_SELECT	0x03            // XBOX360コントローラのMENUボタン
#define PAD_START	0x0C            // メガドラミニパッドのSTARTボタン
#define PAD_X   	0x60            // XBOX360コントローラのXボタン
//#define PAD_Y   	XM6gでは対応せず

// プロトタイプ宣言
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

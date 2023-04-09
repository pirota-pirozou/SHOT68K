// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// X68000 スプライトライブラリ C言語ヘッダ
// 2023/04/06 作成
// by Pirota Pirozou
//
// このライブラリは、X68000のスプライトを扱うためのライブラリです。

#pragma once
#ifndef __CMSprite_H__
#define __CMSprite_H__

#include "types.h"

// PX2ファイル構造体
typedef struct
{
	uint16 atr[256];	    // ツール用アトリビュート
	uint16 pal[256];	    // X68Kパレット
	uint8  sprpat[0x8000];	// X68Kスプライトパターン
} PX2FILE, * pPX2FILE;

#ifdef __cplusplus
extern "C" {
#endif

// プロトタイプ宣言
void CM_sprite_on(void);
void CM_sprite_off(void);
void CM_sprite_clr(void);
void CM_vsync(void);
void CM_sp_set32(int, int, int, int, int);
//					*ｄ１＝スプライトプレーン番号（０～１２７）
//					*ｄ２＝ｘ座標
//					*ｄ３＝ｙ座標
//					*ｄ４＝パターンコード %HR_VR_X_X_COLR_SPATSPAT : 下位8bit:パターン / 8-12bit:カラー
//					*ｄ５＝プライオリティ %0000_0000_0000_0_0_PR : 00 表示しない / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1
void CM_sp_set(int, int, int, int, int);
//					*ｄ１＝スプライトプレーン番号（０～１２７）
//					*ｄ２＝ｘ座標
//					*ｄ３＝ｙ座標
//					*ｄ４＝パターンコード %HR_VR_X_X_COLR_SPATSPAT : 下位8bit:パターン / 8-12bit:カラー
//					*ｄ５＝プライオリティ %0000_0000_0000_0_0_PR : 00 表示しない / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

void CM_parts_wrt(int, int, int, int);
//					*ｄ１＝Ｂｇページ
//					*ｄ２＝ｘ座標
//					*ｄ３＝ｙ座標
//					*ｄ４＝パターンコード(0~255)

void CM_parts_clr(int, int, int);
//* ｄ１＝Ｂｇページ
//* ｄ２＝ｘ座標
//* ｄ３＝ｙ座標

void CM_dma32(int, int, char *, char *);
//* d1=#%0000_0101(ﾌﾟﾛｸﾞﾗﾏｰｽﾞ･ﾏﾆｭｱﾙ参照)*
//* d2=カウンタ                         *
//* a1=転送元（ソース・メモリアドレス） *
//* a2=転送先（ﾃﾞｨｽﾃｨﾈｰｼｮﾝ･Ｉ／Ｏポート)*

void CM_wait_dma_end(void);

void CM_def_px2(pPX2FILE);
void CM_def_grp_palette(uint16*);
void CM_bg_puts(const char *, int, int, int);



#ifdef __cplusplus
}
#endif

#endif // __CMSprite_H__

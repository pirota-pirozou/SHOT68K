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
void CM_def_px2(pPX2FILE);
void CM_def_grp_palette(uint16*);



#ifdef __cplusplus
}
#endif

#endif // __CMSprite_H__

// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// X68000 Z-MUSICコントロールライブラリ C言語ヘッダ
// 2023/05/17 作成
// by Pirota Pirozou
//
// このライブラリは、Z-MUSICで効果音や音楽を鳴らすためのライブラリです。

#pragma once
#ifndef __ZMSCLIB_H__
#define __ZMSCLIB_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// プロトタイプ宣言
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

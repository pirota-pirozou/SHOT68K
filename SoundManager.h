// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、サウンドマネージャーのヘッダファイル である。
//
#pragma once
#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include <stdint.h>
#include "CF_MACRO.h"
#include "types.h"
#include "zmsclib.h"

// BGMの定義
enum
{
    BGM_TITLE = 0,
//    BGM_GAME,
    BGM_MAX = 0
};

// SEの定義
enum
{
    SE_EBOMB = 0,   // 敵の爆発音
    SE_PSHOT,       // 自機のショット音
    SE_PBOMB,       // 自機の爆発音
    SE_MAX
};

// サウンドIDの定義

// 構造体の定義
typedef struct {
    int size;
    void *data;
} ZMDdata, *pZMDdata;

#ifdef __cplusplus
extern "C" {
#endif

// プロトタイプ宣言
void SoundManager_Init(void);
void SoundManager_Load(void);
void SoundManager_PlayBGM(int);
void SoundManager_PlaySE(int);
void SoundManager_End(void);

#ifdef __cplusplus
}
#endif

#endif // __SOUNDMANAGER_H__

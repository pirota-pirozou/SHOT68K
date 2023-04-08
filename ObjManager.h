// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ＯＢＪマネージャーのヘッダファイル である。
//
#pragma once
#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__

#include <stdlib.h>
#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"
#include "GamePadManager.h"
#include "CMSprite.h"


// プロトタイプ宣言
void OBJManager_Init();
void ObjManager_Make();
void ObjManager_Update();
void ObjManager_Draw();
void ObjManager_End();

#endif // __OBJMANAGER_H__

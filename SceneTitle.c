// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、タイトルシーンのコード本体 である。
// Path: SceneTitle.c
//

#include "SceneTitle.h"

// タイトルシーン　初期化
void Title_Init(void)
{
    printf("Title_Init() ");
}

// タイトルシーン　更新
void Title_Update(void)
{
    printf(" Title_Update() ");
}

// タイトルシーン　描画
void Title_Draw(void)
{
    printf("Title_Draw()\n");
}

// タイトルシーン　VSync
void Title_VSync(void)
{
    printf(" Title_VSync()\n");
}

// タイトルシーン　クリア（終了）
void Title_Clear(void)
{
    printf("**Title_Clear()\n");
}

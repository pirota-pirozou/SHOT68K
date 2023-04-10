// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、サンプルプログラムの起動部分 main.c である。
//

#include <stdlib.h>
#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"
#include "mylib.h"
#include "CMSprite.h"
#include "GamePadManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"

//#define PAD_TEST		// コメントを外すとゲームパッドの値表示テスト

// シーンの登録テーブル
static const SSceneWork sceneTable[] =
 {
	{ Title_Init, Title_Update, Title_Draw, Title_VSync, Title_Clear },		// タイトルシーン
	{ Game_Init, Game_Update, Game_Draw, Game_VSync, Game_Clear }			// ゲームシーン
};

// プロトタイプ宣言
BOOL load_data(void);

/* main */
int main(int argc, char *argv[])
{
	super_begin();			// スーパーバイザーモードへ

	screen_init();			// 画面初期化

	gcls(0);				// グラフィック画面クリア０
	gcls(1);				// グラフィック画面クリア１

	BOOL bSuccess;
	bSuccess = load_data();		// データの読み込み

	// データの読み込みに失敗したら終了
	if (!bSuccess)
	{
		goto FORCE_QUIT;
	}

//	printf("画面が切り替わりました。\n");

//	INKEY();				// キー入力待ち

	CM_sprite_on();			// スプライト表示管理ＯＮ

	// ゲームパッドマネージャーの初期化
	GamePadManager_Init();

	// シーンマネージャーの初期化
	SceneManager_Init();
	SceneManager_Regist(sceneTable);

	// シーンの切り替え
	SceneManager_ChangeScene(SCENE_ID_TITLE);

	// ゲームパッドのテスト
#ifdef PAD_TEST
	unsigned short pad = 0;
	unsigned char strtmp[128];
#endif

	// メインループテスト
	for (;;)
	{
		if (BITSNS(0) & 2) 	// ESCキーで抜ける
		{
			break;
		}
		GamePadManager_Update();	// ゲームパッドの更新

#ifdef PAD_TEST
		pad = GamePadManager_GetPad();
		sprintf(strtmp, "PAD=%02X", pad);
		CM_bg_puts(strtmp, 0, 8, 1);
#endif

		Scene_Update();		// シーンの更新
		Scene_Draw();		// シーンの描画
		// VSyncの待ち
//		vsync();
		CM_vsync();			// VSync待ち
		Scene_VSync();		// シーンのVSync後の処理
	}
	Scene_Clear();			// ゲームループから抜けるときシーンのクリア

	CM_sprite_off();		// スプライト表示管理ＯＦＦ

	// プログラム終了
	// プログラムのAbortアドレスを強引にここに設定する
FORCE_QUIT:
	asm volatile (".xdef _PRG_QUIT\n");
	asm volatile ("_PRG_QUIT:\n");
	super_end();					// ユーザーモードへ復帰

	printf("プログラムの実行を終了しました。\n");

	return 0;
}

// データの読み込み
// スプライトパターンとパレットデータを読み込み定義する
// 引数: なし
// Return: TRUE:成功 FALSE:失敗
BOOL load_data(void)
{
	FILE *fp = NULL;
	pPX2FILE px2buf = (pPX2FILE) -1;
	BOOL result = TRUE;

	// PX2ファイルの読み込み
	do
	{
		px2buf = (pPX2FILE)MALLOC(sizeof(PX2FILE));
		if (px2buf < 0)
		{
			printf("px2buf: メモリが確保できません。\n");
			result = FALSE;
			break;
		}
		// PX2ファイルの読み込み
		fp = fopen("shoot.px2", "rb");
		if (fp == NULL)
		{
			printf("スプライトファイルが開けません。\n");
			result = FALSE;
			break;
		}
		int a = fread(px2buf, sizeof(PX2FILE), 1, fp);
		if (a != 1)
		{
			printf("スプライトファイルの読み込みに失敗しました。\n");
			result = FALSE;
			break;
		}
		// スプライトパターンとパレットの定義
		CM_def_px2(px2buf);
		break;
	} while (0);

	if (fp != NULL)
	{
		// ファイルクローズ
		fclose(fp);
		fp = NULL;
	}
	if (px2buf != (pPX2FILE) -1)
	{
		// メモリ解放
		MFREE((int)px2buf);
		px2buf = (pPX2FILE) -1;
	}

	return result;
}

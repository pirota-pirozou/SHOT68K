// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、サンプルプログラムの起動部分 main.c である。
//

#include <stdlib.h>
#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"
#include "mylib.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"

// シーンの登録テーブル
static SSceneWork sceneTable[] =
 {
	{ Title_Init, Title_Update, Title_Draw, Title_VSync, Title_Clear },
	{ Game_Init, Game_Update, Game_Draw, Game_VSync, Game_Clear }
};

/* main */
int main(int argc, char *argv[])
{
	super_begin();			// スーパーバイザーモードへ

	screen_init();			// 画面初期化

	gcls(0);				// グラフィック画面クリア０
	gcls(1);				// グラフィック画面クリア１

	printf("画面が切り替わりました。\n");

	// メモリ確保のテスト
	// DOSコールのメモリ確保関数を使用する。標準のmalloc()は動作が変な気がする。
	// なぜか戻り値がint型になっているので、キャストして使用する
	char *buf1 = (char*)MALLOC(0xA000);
	if (buf1 < 0)
	{
		printf("buf1: メモリが確保できません。\n");
	}

	char *buf2 = (char*)MALLOC(0x2000);
	if (buf2 < 0)
	{
		printf("buf2: メモリが確保できません。\n");
	}
	char *buf3 = (char*)MALLOC(0x8000);
	if (buf3 < 0)
	{
		printf("buf3: メモリが確保できません。\n");
	}

	// メモリ解放のテスト
	// 逆順に解放する
	// なぜか引数がint型になっているので、キャストして使用する
	MFREE((int)buf3);
	MFREE((int)buf2);
	MFREE((int)buf1);

//	INKEY();				// キー入力待ち

	// シーンマネージャーの初期化
	SceneManager_Init();
	SceneManager_Regist(sceneTable);

	// シーンの切り替え
	SceneManager_ChangeScene(SCENE_ID_TITLE);

	// ゲームパッドのテスト
	int pad = 0;
	// メインループテスト
	for (;;)
	{
		if (BITSNS(0) & 2) 	// ESCキーで抜ける
		{
			break;
		}
		pad = gamepad();
		printf("pad = %02X\n", pad);

		Scene_Update();		// シーンの更新
		Scene_Draw();		// シーンの描画
		// VSyncの待ち
		vsync();
		Scene_VSync();		// シーンの垂直同期後の処理
	}
	Scene_Clear();			// ゲームループから抜けるときシーンのクリア

	// プログラム終了
	// プログラムのAbortアドレスを強引にここに設定する
	asm volatile (".xdef _PRG_QUIT\n");
	asm volatile ("_PRG_QUIT:\n");
	super_end();					// ユーザーモードへ復帰

	printf("プログラムの実行を終了しました。\n");

	return 0;
}

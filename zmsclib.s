* -*-  tab-width : 8 ; mode : m68k ; encode : ShiftJIS -*-
* Z-MUSICコントロールライブラリ 680000アセンブラコード
* 2023/05/17 作成
* by Pirota Pirozou
*
* このライブラリは、Z-MUSICで音楽や効果音を鳴らすものです。

	include iocscall.mac
	include doscall.mac
	include	macro.h

	.xdef	_zmsc_init
	.xdef	_zmsc_play
	.xdef	_zmsc_stop
	.xdef	_zmsc_cont
	.xdef	_zmsc_fadeout
	.xdef	_zmsc_seplay
	.xdef	_zmsc_keepchk

	* Z-MUSIC API マクロ
Z_MUSIC	macro	func
	moveq.l	func,d1
	trap	#3
	endm

* 破壊してはいけないレジスタ
* reglist	reg	d3-d7/a3-a5

****************************************
*　初期値なしデータ部　　　　　　　　　*
****************************************
	.bss

keeped_flg:	.ds.w	1	* 常駐フラグ

**************************************
*　初期値ありデータ部　　　　　　　　*
**************************************
	.data

        .text

        * void zmsc_init(void);
_zmsc_init:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	init_ret				* 常駐していない場合は何もしない

	Z_MUSIC	#$00					* Z-MUSIC初期化
	Z_MUSIC	#$0D					* Z-MUSIC初期設定
init_ret:
	rts

        * void zmsc_play(void *ptr);
_zmsc_play:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	play_ret				* 常駐していない場合は何もしない

	movea.l	4(sp),a1				* 演奏データ格納アドレス
	moveq	#0,d2					* 高速応答
	Z_MUSIC	#$11					* ZMD再生

	moveq	#0, d0
play_ret:
	rts

	* void zmsc_stop(void);
_zmsc_stop:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	stop_ret				* 常駐していない場合は何もしない

reglist	reg	d3-d4
	movem.l reglist,-(sp)
	moveq	#0, d2
	moveq	#0, d3
	moveq	#0, d4
	Z_MUSIC	#$0A					* 演奏停止
	movem.l (sp)+,reglist
stop_ret:
	rts

	* void zmsc_cont(void);
_zmsc_cont:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	@f 					* 常駐していない場合は何もしない

reglist	reg	d3-d4
	movem.l reglist,-(sp)
	moveq	#0, d2
	moveq	#0, d3
	moveq	#0, d4
	Z_MUSIC	#$0B					* 演奏再開
	movem.l (sp)+,reglist
@@:
	rts

	* void zmsc_fadeout(void);
_zmsc_fadeout:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	@f					* 常駐していない場合は何もしない

	moveq	#64,d2					* フェードアウト速度
	Z_MUSIC	#$1A					* フェードアウト
@@:
	rts

	.if 0
--------------------------------------------------------------------------------
ファンクション$12       se_play

機能:   効果音演奏

引数:   d2.l＝何トラックから割り込ませるか（1～32）
	a1.l＝演奏データ格納アドレス(備考参照)

戻り値: なし

備考：  効果音の構造
Offset  ＋0     :$10(.b)               ←偶数アドレス}
        ＋1～＋6:'ZmuSiC'                            }メモリ上に無くても構わない
	＋7     :Version Number(.b)                  }
	＋8～   :共通コマンド                        }
	＋?     :Number of tracks(.w)  ←a1.lで指し示すべきアドレス(偶数アドレス)
	なお、内部フォーマットについての詳しい解説はMEASURE12参照
--------------------------------------------------------------------------------
	.endif
	* void zmsc_seplay(void *ptr);
_zmsc_seplay:
*	tst.w	keeped_flg				* 常駐フラグをチェック
*	beq	@f					* 常駐していない場合は何もしない

	moveq	#7, d2					* 7-8 トラックで再生
	movea.l	4(sp),a1				* 演奏データ格納アドレス
	Z_MUSIC	#$12					* ＳＥ再生

	moveq	#0, d0
@@:
	rts

	* int zmsc_keepchk(void);
_zmsc_keepchk:
	* > eq＝常駐確認
	* > mi＝常駐していない
	move.l  $8c.w,a0
	subq.w  #8,a0
	cmpi.l  #'ZmuS',(a0)+
	bne     chk_drv_err
	cmpi.w  #'iC',(a0)+
	bne     chk_drv_err
	* 常駐している
	moveq	#0,d0
	move.w	(a0)+,d0	* バージョン
	move.w  #1, keeped_flg
	rts

	* 常駐していない
chk_drv_err:
	clr.w 	keeped_flg
	moveq.l #-1,d0
	rts

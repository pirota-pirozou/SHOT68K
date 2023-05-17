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

	moveq	#0, d0
init_ret:
	rts

        * void zmsc_play(int);
_zmsc_play:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	play_ret				* 常駐していない場合は何もしない

	moveq	#0, d0
play_ret:
	rts

	* void zmsc_stop(void);
_zmsc_stop:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	stop_ret				* 常駐していない場合は何もしない

	moveq	#0, d0

stop_ret:
	rts

	* void zmsc_fadeout(void);
_zmsc_fadeout:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	fadeout_ret				* 常駐していない場合は何もしない

	moveq	#0, d0
fadeout_ret:
	rts

	* void zmsc_seplay(void);
_zmsc_seplay:
	tst.w	keeped_flg				* 常駐フラグをチェック
	beq	seplay_ret				* 常駐していない場合は何もしない

	moveq	#0, d0
seplay_ret:
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
	move.w  d0, keeped_flg
	rts

	* 常駐していない
chk_drv_err:
	clr.w 	keeped_flg
	moveq.l #-1,d0
	rts

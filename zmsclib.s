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

**************************************
*　初期値ありデータ部　　　　　　　　*
**************************************
	.data

        .text

        * void zmsc_init(void);
_zmsc_init:
	moveq	#0, d0
	rts

        * void zmsc_play(int);
_zmsc_play:
	moveq	#0, d0
	rts

	* void zmsc_stop(void);
_zmsc_stop:
	moveq	#0, d0
	rts

	* void zmsc_fadeout(void);
_zmsc_fadeout:
	moveq	#0, d0
	rts

	* void zmsc_seplay(void);
_zmsc_seplay:
	moveq	#0, d0
	rts

	* int zmsc_keepchk(void);
_zmsc_keepchk:
	moveq	#0, d0
	rts

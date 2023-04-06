* -*-  tab-width : 8 ; mode : m68k ; encode : ShiftJIS -*-
* X68000 スプライトライブラリ 680000アセンブラコード
* 2023/04/06 作成
* by Pirota Pirozou
*
* このライブラリは、X68000のスプライトを扱うためのライブラリです。

	include	IOCSCALL.MAC
	include	DOSCALL.MAC
	include	MACRO.H
	include	LABEL.H

        .xdef _CM_def_px2
        .xdef _CM_def_grp_palette

	************************
	**** スプライト定義 ****
	**** FOR PX2        ****
	************************
        * void CM_def_px2((unsigned char *)px2_buf);
        * px2_buf : PX2データのアドレス
        * この関数は、PX2データをスプライトに定義します。
        * ただし、キャラクタは先頭192キャラ、
        * （BG1面のデータを上書きしないため）
        * パレットは先頭1パレットをスキップして定義します。
        * （テキストパレットをいじらないため）
_CM_def_px2:
	link	a6,#0
	movea.l	8(a6),a0
	lea	$220(a0),a0
	lea	TEXTPAL+32,a1
	lea	PCG_AREA,a2

	move.l	#240-1,d0
@@:
	move.w	(a0)+,(a1)+
	dbra	d0,@b

	move.l	#12288-1,d0
@@:
	move.w	(a0)+,(a2)+
	dbra	d0, @b

	unlk	a6
	rts

	********************************
	**** グラフィックパレット定義 ****
	*******************************
	* void CM_def_grp_palette((unsigned short *) grppal_buf);
        * pe2pal_buf : PE2パレットデータのアドレス
_CM_def_grp_palette:
	movea.l	4(sp),a1
	lea	GRPPAL,a0

	moveq	#127,d0
@@:
	move.l	(a1)+,(a0)+
	dbra	d0,@b
	rts

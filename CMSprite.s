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
	include	DMAADR.H

        * 内部ワーク
	.xdef	sprtbl
	.xdef	paltbl

	.xdef	sp_ready
	.xdef	int_cou

        * 関数定義
	.xdef _CM_sprite_clr
        .xdef _CM_sprite_on
        .xdef _CM_sprite_off
        .xdef _CM_vsync
        .xdef _CM_sp_set32
        .xdef _CM_sp_set
        .xdef _CM_parts_wrt
        .xdef _CM_parts_clr

        .xdef _CM_dma32
        .xdef _CM_wait_dma_end

        .xdef _CM_def_px2
        .xdef _CM_def_grp_palette

	.xdef _CM_bg_puts



* 破壊してはいけないレジスタ
* reglist	reg	d3-d7/a3-a5

****************************************
*　初期値なしデータ部　　　　　　　　　*
****************************************
	.bss
	** ＳＰスクロール・レジスタ用ワーク **
sprtbl:
	ds.b	1024

	** ＳＰパレット・レジスタ用ワーク **
	* ※もしかしたら、使われていないかもしれない
paltbl:
	ds.b	512

	** 割り込みカウンタ（０～２５５） **
int_cou:
	ds.b	1
	** sprite_ready FLAG **
sp_ready:
	ds.b	1

	** ＤＭＡ－Ｃ割り込みベクタ **
DMAC_V:
	ds.l	1

**************************************
*　初期値ありデータ部　　　　　　　　*
**************************************
	.data
	** sp_set32用データ **
notable:
	* (0,1)(8,9)→(0,1)(2,3)
	dc.w	0,1,3,2		*ノーマル
	dc.w	1,0,2,3		*左右反転
	dc.w	2,3,1,0		*上下反転
	dc.w	3,2,0,1		*上下左右反転

	** parts_wrt用データ **
notable2:
	dc.w	0,2,1,3		*ノーマル
	dc.w	2,0,3,1		*左右反転
	dc.w	1,3,0,2		*上下反転
	dc.w	3,1,2,0		*上下左右反転

        .text

        ** スプライト表示モード　ＯＮ **
        * void CM_sprite_on(void);
_CM_sprite_on:
	clr.w	int_cou			* int_cou&sp_ready clr

	***** ＳＰスクロールＲＥＧ　クリア ****
	lea	$EB0006,a0
	moveq	#127,d0
@@:
	clr.w	(a0)
	addq.l	#8,a0
	dbra	d0,@b

	***** DMA INIT **********
	move.b	#$18,CCR2		* DMA Stop!!
	move.b	#$88,DCR2		* ＣＰＵ動作を優先、デバイスポート１６ビット
*	move.b	#$C8,DCR2		* ＣＰＵ動作を？？、デバイスポート１６ビット
	move.b	#$21,OCR2		* 転送単位：32bit=$21
	move.b	#FC_SUPER_DATA,MFC2
	move.b	#FC_SUPER_DATA,DFC2
	move.b	#5,SCR2			*
	clr.w	MTC2			* Counter Clear!

	ori.b	#$02,$EB0808
	ori.w	#$0040,$E82600

	move	sr,-(sp)		* 割り込み禁止

	ori	#%00000111_00000000,sr
	clr.b	$E88019			* MFP_TACR
	lea.l	sprite_int,A1
	move.w	#$00_01,d1
	iocs	_VDISPST
	move.b	#%00001000,$E88019

	move	(sp)+,sr		* 割り込み禁止状態の解除

	tst.l	d0
	beq	@f
	* Error
	moveq.l	#-1,d0
	rts
@@:	* OK
	moveq.l	#0,d0
	rts

	** スプライト表示モード　ＯＦＦ **
        * void CM_sprite_off(void);
_CM_sprite_off:
	push	d0-d1/a0-a1

	suba.l	a1,a1			* 垂直同期割り込みを禁止
	IOCS	_VDISPST

	pop	d0-d1/a0-a1
	rts

	** 割り込み処理ルーチン **
sprite_int:
*	move.w	sr,d0		*割り込みを許可する。割り込みプログラムでは
*	subi.w	#$0100,d0	*これを入れておくのが礼儀。これがないと曲が遅れたりなどの
*	move.w	d0,sr		*弊害（この処理中は他の処理が割り込めなくなるため）が出ます。
				*ただし、デバッグ中は邪魔なので完成してから入れましょう。だそうです。
*	andi.b	#$DF,$E88013

*	andi.b	#$FD,$EB0808			* SP_DISP OFF

	**********************	スプライトレジスタ　ＤＭＡ転送
	move.b	#$18,CCR2			* DMA Stop!!
	move.l	#sprtbl,MAR2			* SOURCE ADRESS
	move.l	#$EB0000,DAR2			* DIST.ADRESS
	move.w	#$100,MTC2			* SIZE COUNTER
	move.b	#$88,CCR2			* DMA Move start!

@@:	* DMA終了待ち
	tst.w	MTC2
	bne	@b

	move.b	#1,sp_ready
	rte


* 	*********************
* void CM_vsync();
_CM_vsync:
@@:	* 垂直同期割り込み待ち
	tst.b  sp_ready
	beq	@b
	move.b	#0,sp_ready
@@:
*	btst.b	#4,$E88001			* 垂直帰線期間中は待つ
*	beq	@b

*	ori.b	#$02,$EB0808			* SP_DISP ON
	rts

* スプライトクリア
* void CM_sprite_clr(void);
_CM_sprite_clr:
*	lea	$EB0006,a0
	lea.l	sprtbl+6,a0
	moveq	#127,d0
@@:
	clr.w	(a0)
	addq.l	#8,a0
	dbra	d0,@b

	rts

*************************************
**    ３２×３２スプライトＰＵＴ   **
**（仮想ＳＰスクロール・レジスタ用）*
**                                 **
** 破壊：ｄ１～ｄ６，ａ０，ａ１    **
**                                 **
**      Ｓ＿ｓｐ＿ｓｅｔ３２       **
*************************************
_CM_sp_set32:
					*ｄ１＝スプライトプレーン番号
					*ｄ２＝ｘ座標
					*ｄ３＝ｙ座標
					*ｄ４＝パターンコード
					*ｄ５＝プライオリティ
	link	a6,#0
reglist reg     d3-d6
	push    reglist
	movem.l	8(a6),d1-d5

	lea.l	sprtbl,a0
	lsl.w	#3,d1
	adda.w	d1,a0			*a0=ｓｐスクロールレジスタのａｄｒｅｓｓ

	lea.l	notable,a1
	btst	#14,d4
	beq	@f
	addq.l	#8,a1
@@:	btst.l	#15,d4
	beq	@f
	lea.l	16(a1),a1
@@:	move.l	d4,d6			*ｄ4の退避

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	move.l	d6,d4
	add.w	#16,d2

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	move.l	d6,d4
	add.w	#16,d3

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	move.l	d6,d4
	sub.w	#16,d2

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	pop	reglist
	unlk	a6

	rts

*************************************
**    １６×１６スプライトＰＵＴ   **
**（仮想ＳＰスクロール・レジスタ用）*
**                                 **
** 破壊：ａ０                      **
**                                 **
**   　　　Ｓ＿ｓｐ＿ｓｅｔ　　　　**
*************************************
_CM_sp_set:
	link	a6,#0
reglist reg     d3-d5
	push    reglist
	movem.l	8(a6),d1-d5
					*ｄ１＝スプライトプレーン番号
					*ｄ２＝ｘ座標
					*ｄ３＝ｙ座標
					*ｄ４＝パターンコード
					*ｄ５＝プライオリティ
	lea.l	sprtbl,a0
	move.l	d1,-(sp)
	lsl.w	#3,d1
	adda.w	d1,a0
	move.l	(sp)+,d1

	movem.w	d2-d5,(a0)

        pop	reglist
	unlk	a6
	rts

***********************************
**　ＢＧにパーツを置くルーチン　 **
**            （２×２）         **
**　破壊：ｄ２－ｄ５，ａ０－ａ１ **
**                               **
**      ｐａｒｔｓ＿ｗｒｔ       **
***********************************
_CM_parts_wrt:
	link	a6,#0
reglist reg     d3-d5
	push	reglist
	movem.l	8(a6),d1-d4
					*ｄ１＝Ｂｇページ
					*ｄ２＝ｘ座標
					*ｄ３＝ｙ座標
					*ｄ４＝パターンコード
	lea.l	$EBC000,a0
	tst.w	d1
	beq	@f

	lea.l	$2000(a0),a0
@@:
	move.w	d4,d5
	lsl.w	#2,d4			*d4=d4x4
	andi.w	#$00FF,d4
	andi.w	#$FF00,d5
	or.w	d5,d4

	add.w	d2,d2
	add.w	d2,d2
	adda.l	d2,a0			*a0=a0+(x*4)
	lsl.w	#8,d3
	adda.l	d3,a0			*a0=a0+(y*256)

	lea.l	notable2,a1
	btst.l	#14,d4
	beq	@f
	addq.l	#8,a1
@@:
	btst.l	#15,d4
	beq	@f
	lea.l	16(a1),a1
@@:
	move.w	d4,d5			*ｄ4の退避

	add.w	(a1)+,d4
	move.w	d4,(a0)+

	move.w	d5,d4
	add.w	(a1)+,d4
	move.w	d4,(a0)+

	adda.l	#124,a0			*a0=a0+((64-2)*2)

	move.w	d5,d4
	add.w	(a1)+,d4
	move.w	d4,(a0)+

	move.w	d5,d4
	add.w	(a1)+,d4
	move.w	d4,(a0)

	pop	reglist
	unlk	a6
	rts

***********************************
**　ＢＧのパーツを消すルーチン　 **
**         （２×２）　　　　　  **
**　破壊：ｄ２－ｄ３，ａ０　　　 **
**                               **
**      ｐａｒｔｓ＿ｃｌｒ       **
***********************************
* ｄ１＝Ｂｇページ
* ｄ２＝ｘ座標
* ｄ３＝ｙ座標
_CM_parts_clr:
	link	a6,#0
reglist	reg	d3
	move.l	reglist,-(sp)
	movem.l	8(a6),d1-d3

	lea.l	$EBC000,a0
	tst.w	d1
	beq	@f

	lea.l	$2000(a0),a0
@@:
	add.w	d2,d2
	add.w	d2,d2
	adda.l	d2,a0			*a0=a0+(x*4)
	lsl.w	#8,d3
	adda.l	d3,a0			*a0=a0+(y*256)

	clr.l	(a0)+
	lea.l	124(a0),a0
	clr.l	(a0)

        move.l	(sp)+,reglist
	unlk	a6
	rts

***************************************
*    ３２ビット単位 ＤＭＡ転送        *
*                                     *
* d1=#%0000_0101(ﾌﾟﾛｸﾞﾗﾏｰｽﾞ･ﾏﾆｭｱﾙ参照)*
* d2=カウンタ                         *
* a1=転送元（ソース・メモリアドレス） *
* a2=転送先（ﾃﾞｨｽﾃｨﾈｰｼｮﾝ･Ｉ／Ｏポート)*
***************************************
_CM_dma32:
@@:     * DMA転送終了待ち
	tst.w	MTC2
	bne	@b

	link	a6,#0
*reglist reg     d3-d6
*	push	reglist
	movem.l	8(a6),d1-d2/a1-a2

        * DMA転送開始
	move.b	#$18,CCR2		*
*	move.b	#$08,DCR2		* (全開バリバリの時は、コレ)
	move.b	#$88,DCR2		* ＣＰＵ動作を優先、デバイスポート１６ビット
	move.b	#$21,OCR2		* 32bit=$21
	move.b	#FC_SUPER_DATA,MFC2
	move.b	#FC_SUPER_DATA,DFC2
	move.b	d1,SCR2			*
	move.l	a1,MAR2			* SOURCE ADRESS
	move.l	a2,DAR2			* DIST.ADRESS
	move.w	d2,MTC2			* SIZE COUNTER
	move.b	#$88,CCR2		* Move start!

*	pop	reglist
	unlk	a6
	rts

        * DMA転送終了待ちルーチン
        * void CM_wait_dma_end(void);
_CM_wait_dma_end:
	tst.w	MTC2
	bne	_CM_wait_dma_end
	rts

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
        * スプライトパレット定義
	move.l	#240-1,d0
@@:
	move.w	(a0)+,(a1)+
	dbra	d0,@b

        * キャラクタ定義
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

        * グラフィックパレット定義
	moveq	#127,d0
@@:
	move.l	(a1)+,(a0)+
	dbra	d0,@b
	rts

* void CM_bg_puts(const char *str, int x, int y,int col);
_CM_bg_puts:
	link	a6,#0
reglist reg     d3
	push	reglist

	move.l	8(a6),a0		* a0=文字列アドレス
	move.l	12(a6),d1		* X座標
	move.l	16(a6),d2		* Y座標
	move.l	20(a6),d3		* 色コード 0-15
	* VRAMアドレス計算
	lea	BG_TXT1,a1		* a1=BG1 TEXTアドレス
	add.w	d1,d1
	adda.l	d1,a1
	lsl.w	#7,d2
	adda.l	d2,a1			* A1=BG RAM ADRESS

	* 色コード計算
	lsl.w	#8,d3
	andi.w	#$0F00,d3

@@:	move.w	d3,d0
	move.b	(a0)+,d0
	beq	@f			* 終端まできたら終了

	subi.b	#$20,d0			* 文字コードを補正
	move.w  d0,(a1)+		* 文字コードを書き込む
	bra.s	@b
@@:
	pop	reglist
	unlk	a6
	rts

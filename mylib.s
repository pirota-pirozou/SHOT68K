* -*-  tab-width : 8 ; mode : M658k-Assembly ; encode : ShiftJIS -*-
* オレオレライブラリ For X68000
* By Pirota Pirozou, 2022-2023
*
* このコードは、あると便利な関数をまとめたものです。
* このライブラリは、自由に改変してもらって構いません。
* ただし、改変したものを公開する場合も、このライブラリの著作権表示は必要ありません。
* また、このライブラリを使用したプログラムの著作権は、
* そのプログラムの著作者にあります。
*
* This code is a collection of functions that are useful to have.
* This library may be freely modified.
* However, even if you publish the modified version, you do not need to indicate the copyright of this library.
* The copyright of any program that uses this library belongs to the author of the program,
* the authors of the programs that use this library.

	.include IOCSCALL.MAC
	.include DOSCALL.MAC
	.include LABEL.H

	* ＰＵＳＨマクロ *		ex)push a3-d6
push:	macro	Param
	movem.l	Param,-(sp)
	endm

	* ＰＯＰマクロ *		ex)pop	d3-d7/a3
pop:	macro	Param
	movem.l	(sp)+,Param
	endm

COPYKILL	equ	1		* ＣＯＰＹキー処理　無効フラグ

	.xref	_PRG_QUIT

	.xdef	_super_begin
	.xdef	_super_end
	.xdef	_screen_init
	.xdef 	_gcls
	.xdef	_gamepad
	.xdef	_TX_CLS

	.xdef	_vsync

	.bss
	.even

nmibuf:
	ds.l	1			* 例外処理・元の飛び先
abort_vec:
	ds.l	1			* アボート・ベクタ　元の飛び先
uspbuf:
	ds.l	1			* ｕｓｐ退避用

	.text
	.even

	***************************
	* スーパーバイザー・モードへ
	***************************
	* void super_begin(void);
_super_begin:
regs	reg	d3-d7/a3-a5
	movem.l regs,-(sp)
*	-------------------
	lea.l	$80,a1			* ＴＲＡＰ＃０のバッファを調べる
	IOCS	_B_LPEEK

*	cmpi.l	#$00FF0000,d0
	bra	@f
*	bcs	@f		* 飛び先がＲＯＭでない

	*** ミュージックドライバが登録されてない ****
*	pea	mdrverr
*	DOS	_PRINT
	addq.l	#4,sp

	move.w	#1,-(sp)
	DOS	_EXIT2
*	===================
@@:
 	ifne	COPYKILL
	pea	dumint			* ＣＯＰＹ＝ＫＩＬＬ
	move.w	#$2C,-(sp)
	DOS	_INTVCS
*	-------------------
	pea	nmi			* ＩＮＴＥＲＲＵＰＴ＝ＫＩＬＬ
	move.w	#$2E,-(sp)		* （ＴＲＡＰ　＃１４）
	DOS	_INTVCS
	lea.l	12(sp),sp

	move.l	d0,nmibuf
	endif
*	===================
	pea	dumint			* ＢＲＥＡＫ＝ＫＩＬＬ
	move.w	#$2B,-(sp)		* 6
	DOS	_INTVCS
*	-------------------
	pea	dumint			* ＣＴＲＬ＋Ｃ＝ＫＩＬＬ
	move.w	#$2D,-(sp)
	DOS	_INTVCS			* 12
*	-------------------

	pea	_PRG_QUIT		* アボートベクタをセットする
	move.w	#$FFF2,-(sp)
	DOS	_INTVCS			* 18
	move.l	d0,abort_vec

	lea.l	18(sp),sp
*	-------------------

	clr.l	-(sp)
	DOS	_SUPER			* スーパーバイザ・モードへ
	addq.l	#4,sp
	lea.l	uspbuf,a1
	move.l	d0,(a1)

*	-------------------
	movem.l (sp)+,regs
	rts

	*******************
	**** 画面初期化 ****
	*******************
	* void screen_init(void);
_screen_init:
regs	reg	d3-d7/a3-a5
	movem.l regs,-(sp)

	**** システム関係初期化 ****
	moveq	#1,d1
	moveq	#2,d2
	IOCS	_TGUSEMD		* テキストをアプリの使用中にしてやる

	IOCS	_MS_INIT

	IOCS	_OS_CUROF		* カーソル消して
	IOCS	_MS_CUROF

	move.w	#2,-(sp)
	move.w	#14,-(sp)
	DOS	_CONCTRL		* ファンクション消去
	addq.l	#4,sp

	jbsr	_TX_CLS			* TEXT クリア

	moveq	#0,d1
	moveq	#2,d2			* Ｇ－ＲＡＭは、アプリで使用中
	IOCS	_TGUSEMD

	move.w	#$10A,d1
	IOCS	_CRTMOD			* 256x256 256色x2枚にして

	IOCS	_G_CLR_ON

	*** PCGパターン０ クリア
	lea.l	PCG_AREA,a0
	moveq	#0,d0
	move.l	#32*192-1,d1
@@:
	move.l	d0,(a0)+
	dbra	d1,@b

	moveq	#1,d1			* ＢＧＴＥＸＴ１　ＣＬＲ
	move.w	#$100,d2
	IOCS	_BGTEXTCL

	* スプライトパレットクリア
	lea.l	SP_PAL+32,a0
	moveq	#16*15/4-1,d0
	moveq	#0,d1
@@:
	move.l	d1,(a0)+
	dbra	d0,@b

	* グラフィックパレットクリア
	lea.l	GRPPAL,a0
	moveq	#127,d0
	moveq	#0,d1
@@:
	move.l	d1,(a0)+
	dbra	d0,@b

	*
	lea.l	SP_SCR+6,a0			* ＳＰスクロールレジスタ　初期化
	moveq	#127,d0
spcl_loop:
	clr.w	(a0)				* プライオリティ：0 （非表示）
	addq.l	#8,a0
	dbra	d0,spcl_loop

	clr.l	BG0_SCRX			* ＢＧ０スクロール 0,0
	clr.l	BG1_SCRX			* ＢＧ１スクロール 0,0

	* グラフィック優先順位
crtmode = %00000_001				* 256Colors
	move.b	#crtmode,$E80028		* ( R20 )
	move.b	#crtmode,$E82401		* 256colors
	* ビデオコントローラレジスタ
	move.w	#%00_10_00_01_1110_0100,$E82500	* R1>TEXT>SP>GR ／ GR=Sc0>Sc1
	move.b	#$6F,$E82601			* R2>グラフィック|スプライト表示許可

	move.b	#%1111,$E8002B			* グラフィックスクリーン１，２

	move.w	#%00000010_00_011_011,$EB0808	* ＢＧ０＝ＴＥＸＴ１／ＢＧ１＝ＴＥＸＴ１/DISP ON

	movem.l (sp)+,regs
	rts

	************************
	* ユーザー・モードへ復帰
	************************
	* void super_end(void);
_super_end:
regs	reg	d3-d7/a3-a5
	movem.l regs,-(sp)
	jbsr	_TX_CLS

	move.b	#$08,$E8E007		* ＨＲＬをクリア

	* B_SUPER の呼び出しにgcc最適化の都合で失敗する場合、CFLAGS に、-fno-defer-pop をつける
	* つけない場合は、コメントアウトでユーザ・モードへはOSに任せる
	lea.l	uspbuf,a1
	move.l	(a1),-(sp)
	DOS	_SUPER
	addq.l	#4,sp
*	-------------------

	**** 画面初期化 ****
	moveq	#1,d1
	moveq	#1,d2			* テキストは、システムで使っている
	IOCS	_TGUSEMD

	moveq	#0,d1
	moveq	#3,d2			* Ｇ－ＲＡＭは、アプリで使って壊れた
	IOCS	_TGUSEMD

	IOCS	_MS_INIT		* マウス初期化

	clr.w	-(sp)
	move.w	#16,-(sp)
	DOS	_CONCTRL
	addq.l	#2,sp
	move.w	#14,-(sp)
	DOS	_CONCTRL		* ファンクション表示
	addq.l	#4,sp

	IOCS	_OS_CURON

	**** メモリ開放 ****
	clr.l	-(sp)
	DOS	_MFREE
	addq.l	#4,sp

	move.w	#$FF,-(sp)
	DOS	_KFLUSH
	addq.l	#2,sp

	movem.l (sp)+,regs
	rts

	******************************
	**** グラフィック画面クリア ****
	******************************
	* void gcls(int page);
_gcls:
reglist	reg	d3-d7
	move.l	4(sp),d0		* 引数をd0に(ページ番号)

	push	reglist

	lsl.w	#3,d0			* x 8
	swap	d0
	lea	GRAM+$80000,a0
	adda.l	d0,a0
	move.l	#$800,d7
	moveq	#0,d0
	moveq	#0,d1
	moveq	#0,d2
	moveq	#0,d3
	moveq	#0,d4
	moveq	#0,d5
	moveq	#0,d6
	suba.l	a1,a1
@@:
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	dbra	d7,@b

	pop	reglist
	rts

	*****************************
	* ジョイパッド０＆キー入力    *
	*****************************
	* int gamepad(void);
	*	X12XRLDU
	*	････････
	*	RET=D0.L :JOYSTICK
	*	生のデータと異なりbitは1=ON, 0=OFF
_gamepad:
	moveq	#0,d1
	IOCS	_JOYGET			;ジョイスティック０　チェック
	eori.b	#$FF,d0			;反転
	bne	@f
	jbsr	kbchk0			;キーボード・チェック
@@:
	andi.l	#$FF,d0			;ジョイスティック０　ＦＬＡＧ
	rts

	*******************************
	* テンキー、ＺＸ　ＳＰ入力      *
	*******************************
	* キーボード入力とジョイパッド入力を合成
kbchk0:
	push	d1-d2

	moveq	#0,d2			;結果バッファ

	*** 方向　決定 ***
	move.w	#8,d1
	IOCS	_BITSNS			;
	btst.l	#7,d0			;チェック　”４”
	beq	st0_01
	ori.b	#4,d2			;ＬＥＦＴ　ＦＬＡＧ
st0_01:
	btst.l	#4,d0			;チェック  ”８”
	beq	st0_02
	ori.b	#1,d2			;ＵＰ　ＦＬＡＧ
st0_02:


st0_11:
	move.w	#9,d1
	IOCS	_BITSNS			;
	btst.l	#4,d0			;チェック　”２”
	beq	st0_12
	ori.b	#2,d2			;ＤＯＷＮ　ＦＬＡＧ
st0_12:
	btst.l	#1,d0			;チェック　”６”
	beq	st0_13
	ori.b	#8,d2
st0_13:
	*** ボタン　決定 ***
	btst.l	#6,d0			;チェック　”ENTER”
	bne	st0_trg1

st0_21:
	move.w	#5,d1
	IOCS	_BITSNS			;
	btst.l	#2,d0			;チェック　”Ｚ”
	beq	st0_22
	ori.b	#64,d2			;ＴＲＧ－２
st0_22:
	btst.l	#3,d0			;チェック　”Ｘ”
	beq	st0_23
st0_trg1:
	ori.b	#32,d2			;ＴＲＧ－１
	bra	st0_ret
	****************
st0_23:
	move.w	#6,d1
	IOCS	_BITSNS			;チェック　”ＳＰＣ”
	btst.l	#5,d0
	bne	st0_trg1

*	------------------
st0_ret:
	move.b	d2,d0

	pop	d1-d2
	rts

********************************************
*  テキストプレーン・オールクリアー        *
*                                          *
*         　ＴＸ＿ＣＬＳ                   *
********************************************
*	Ｉｎ：ナシ
*	破壊：ナシ
*
*	TX_CLS();
_TX_CLS:
	push	d0-d3/a0
	lea	TEXTRAM,a0

	moveq	#128-1,d0
KESU_loop:
	move.l	a0,-(sp)
	clr.l	(a0)
	adda.l	#$20000,a0
	clr.l	(a0)
	adda.l	#$20000,a0
	clr.l	(a0)
	adda.l	#$20000,a0
	clr.l	(a0)

	move.l	(sp)+,a0
	addq.l	#4,a0
	dbra	d0,KESU_loop

	move.w	#$00_01,d1
	move.w	#256-1,d2
	moveq	#$00_0F,d3
	IOCS	_TXRASCPY

	pop	d0-d3/a0
	rts

*	-------------------------	* ダミーの割り込み処理アドレス
*	割り込み処理アドレスを、
*	プログラム終了処理に変更する
*	-------------------------
nmi:
	cmpi.w	#$001F,d7		* ＮＭＩリセットが、かかったか？
	beq	nmi2
	cmpi.w	#$301F,d7		* ＮＭＩリセットが、かかったか？
	beq	nmi2

	pea	_PRG_QUIT		* アボートベクタをセットする
	move.w	#$FFF2,-(sp)
	DOS	_INTVCS
	addq.l	#6,sp

	move.l	nmibuf,-(sp)
	rts				* もとの処理アドレスへ飛ぶ

nmi2:
	move.b	#%1101,$E8E007		* ＮＭＩリセットが押されたら、これをするそうな。
*	move.b	#%1111,$E8E007		* (ＨＲＬがＯＮだったらコレ)
dumint:
	rte

	*******************************************************
	**** 確実に１／６０秒以内に終わる処理の時に使う同期取り ****
	*******************************************************
	* void vsync(void);
_vsync:
	btst.b	#4,$E88001		* 垂直帰線期間中は待つ
	beq	_vsync
@@:
	btst.b	#4,$E88001		* 垂直表示期間は待つ
	bne	@b
	rts

	.end

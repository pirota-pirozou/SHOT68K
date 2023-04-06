* -*-  tab-width : 4 ; mode : M68k-Assembly ; encode : ShiftJIS -*-
************************
** X68000 開発用ラベル **
**   By Pirota Pirozou **
************************
	.nlist
******** データ処理用
NULL		equ	$00		* ヌル
CR		equ	$0D		* キャリッジ・リターン
LF		equ	$0A		* ラインフィード
EOF		equ	$1D		* エンド・オブ・ファイル
ESC		equ	$1B		* ＥＳＣコード　始まり

STD		equ	$01		* ファイルハンドル・標準出力
STDERR		equ	$02		* ファイルハンドル・標準エラー出力

******** システム関係アドレス
GRAM		equ	$C00000		* グラフィックＶＲＡＭ
GRPPAL		equ	$E82000		* グラフィックパレット・レジスタ
GRP0_X		equ	$E80018		* スクリーン０　Ｘ
GRP0_Y		equ	$E8001A		* スクリーン０　Ｙ
GRP1_X		equ	$E8001C		* スクリーン１　Ｘ
GRP1_Y		equ	$E8001E		* スクリーン１　Ｙ
GRP2_X		equ	$E80020		* スクリーン２　Ｘ
GRP2_Y		equ	$E80022		* スクリーン２　Ｙ
GRP3_X		equ	$E80024		* スクリーン３　Ｘ
GRP3_Y		equ	$E80026		* スクリーン３　Ｙ

TEXTRAM		equ	$E00000		* テキストＶＲＡＭ
TEXTPAL		equ	$E82200		* テキストパレット・レジスタ
TEXT_X		equ	$E80014		* テキスト　Ｘ
TEXT_Y		equ	$E80016		* テキスト　Ｙ

SP_SCR		equ	$EB0000		* ＳＰスクロール・レジスタ
SP_PAL		equ	TEXTPAL		* ＳＰパレット・レジスタ

PCG_AREA	equ	$EB8000		* ＰＣＧエリア
BG_TXT0		equ	$EBC000		* ＢＧテキスト０
BG_TXT1		equ	$EBE000		* ＢＧテキスト１
BG0_SCRX	equ	$EB0800		* ＢＧスクロールレジスタ０　Ｘ
BG0_SCRY	equ	$EB0802		* ＢＧスクロールレジスタ０　Ｙ
BG1_SCRX	equ	$EB0804		* ＢＧスクロールレジスタ１　Ｘ
BG1_SCRY	equ	$EB0806		* ＢＧスクロールレジスタ１　Ｙ
BG_CNTRL	equ	$EB0808		* ＢＧコントロール

	.list

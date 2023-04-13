// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// Written by Pirota Pirozou, 2023/04
// BMPLoad256.h
//
// このコードは、２５６色ＢＭＰローダーのヘッダファイル である。
// xdev68k環境で動作するように作られている。
//
// BMPLoad256.c / BMPLoad256.h は、自由に改変して自分のプログラムに組み込むことが可能。
// 著作権表記も必要ない。
// ただし、このコードを使用したことによるいかなる損害についても、作者は一切の責任を負わない。

#pragma once
#ifndef ____BMPLOAD256_H____
#define ____BMPLOAD256_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short WORD_t;
typedef unsigned int DWORD_t;
typedef unsigned char BYTE_t;

/// @brief ビットマップファイルヘッダー
typedef struct {
    WORD_t bfType;
    DWORD_t bfSize;
    WORD_t bfReserved1;
    WORD_t bfReserved2;
    DWORD_t bfOffbits;
} BITMAPFILEHEADER;

/// @brief ビットマップ情報ヘッダー
typedef struct {
    DWORD_t biSize;
    DWORD_t biWidth;
    DWORD_t biHeight;
    WORD_t biPlanes;
    WORD_t biBitcount;
    DWORD_t biCompression;
    DWORD_t biSizeimage;
    DWORD_t biXPelsPerMeter;
    DWORD_t biYPelsPerMeter;
    DWORD_t biClrUsed;
    DWORD_t biClrImportant;
} BITMAPINFOHEADER;

/// @brief カラーパレット
typedef struct {
    BYTE_t rgbBlue;
    BYTE_t rgbGreen;
    BYTE_t rgbRed;
    BYTE_t rgbReserved;
} RGBQUAD;

/// @brief ２５６色ビットマップファイル
typedef struct {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD palette[256];
    BYTE_t imgdata[4];
} BMPFILE256, *pBMPFILE256;

#ifdef __cplusplus
extern "C" {
#endif

// プロトタイプ宣言
pBMPFILE256 LoadBMP256(const char *fname);
int PutBMPMemory256(pBMPFILE256 pBMP);
int PutBMPFile256(const char *fname);

#ifdef __cplusplus
}
#endif

#endif // __BMPLOAD256_H__

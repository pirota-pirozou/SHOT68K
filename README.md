# SHOT68K

## 概要

- このプログラムは、X68000のxdev68k環境向けに書かれた、シンプルなシューティングゲームのサンプル実装です。
つたない部分も多々あると思いますが、X68000ZでX68000に初めて触れた方でゲームプログラミングに興味がある方は、このコードを参考にしていただき、再利用、コピペなどで自らの新しいゲームを作ってみて下さい。

- 内容的にはスペースインベーダーの劣化コピー実装で、あまり凝ったことはしていません。スプライトダブラやスプライトキャッシュなどのテクニックも使用していない、いたってシンプルなものです。コードの理解を目的としているので、ゲームの内容はあえてシンプルにしています。

## ビルド方法

- このプログラムは、X68000のxdev68k環境でビルドすることを想定しています。実機ではビルドできません。xdev68kをインストールしてあるMINGW64環境（作者の場合Windows10/11）で、Makeして下さい。成功すると、SHOT68K.Xが生成されます。

- 動作にはFLOAT2.Xなどの浮動小数点演算ライブラリが必要です。FLOAT2.Xは、Human68K v2,v3のシステムディスク内の \SYS ディレクトリに同梱されているので、実行時には組み込まれているようにしてください。

## 遊び方

- ゲーム開始時に、自機が画面の下部に現れます。自機は、ジョイパッドの左右キーか、キーボードのテンキー[4][6]で左右に移動します。

- 自機の砲台は、ジョイパッドのA,Bボタンか、キーボード[Z][X]で弾を発射します。この弾を敵に当てて、全滅させて下さい。

- [ESC] キーでゲームを終了し、COMMAND.X に戻ります。

- X68000Z にメガドライブミニのパッドをつないでいる場合、STARTボタンでゲームを開始、ゲーム中にはポーズが可能です。

## ライセンス

- これらのソースコードは、自由に改変して自分のプログラムに組み込むことが可能です。著作権表記も必要ありません。ただし、このコードを使用したことによるいかなる損害についても、作者は一切の責任を負わないものとします。
**（画像データなどソースコード以外のデータは再利用しないで下さい）**

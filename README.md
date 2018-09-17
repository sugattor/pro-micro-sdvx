# Sketch of SDVX controller for Pro Micro

## 概要
Pro Micro(Arduino互換機)でボルテコンを作るためのスケッチファイルです。

コナステ版(旧e-amusement cloud)で遊ぶために作ったので、つまみの回転がキーボード出力になります。

マウス出力版はbranchに。

## 使い方
Arduino IDEに読み込ませて書き込むだけ。

キー設定・定数は#defineによって定義されています。適宜書き換えるかゲーム内設定を変更してください。

## 注意点
どのピンがどのキーに対応しているかはソースのコメントに書かれています。

ピンの配置等、Pro Microの仕様に関しては、以下をご覧ください。
- [(Arduino) Pro Micro](https://ht-deko.com/arduino/promicro.html)

3.3V用と5V用があります、恐らくどちらでも動くはずですが、書き込む際のボードの選択には気をつけましょう。

（ちなみに私はAliexpressで安い互換品を買いました）

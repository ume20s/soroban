# soroban
そろばんプログラム


「教養のソフトウェア」シリーズ


　　『 そろばん Ver1.20  [ soro.exe ] 』

　◎　機　能

　　そろばんみたいに計算します．


　◎　使い方

　　　soro [ -d | -m ]

　　-d : デンタク・モード
　　　　そろばんを電卓の操作感覚で使うことが出来ます．
　　　　使い方ウインドウには表示されませんが，使えるキーは全部でこんだけ
　　　　あります．
　　　　
　　　　　［０］～［９］ ： 数字の入力
　　　　　　　［－］　　 ： 引いては
　　　　　　　［＋］　　 ： 足しては
　　　　　 ［ Ｂ  Ｓ ］  ： １字後退
　　　　　 ［リターン］  ： ～円なり
　　　　　 ［スペース］  ： ～円では
　　　　　 ［Ｅ Ｓ Ｃ］  ： ご 破 算
　　　　　 ［ＳＴＯＰ］  ：  終  了 
　　　　
　　　　基本的には数字をパシパシ入力して［リターン］で加算していきます．
　　　　（つまり［＋］は，あまり使わないでしょう）
　　　　引き算したいときは［－］キーを押すと，次に入力する数が引算モード
　　　　になります．
　　　　あと，［リターン］と［スペース］はだいたい同じ動作をします．
　　　　この２つのキーで違うのは，表示が『～円なり』となるか『～円では』
　　　　となるかだけです．［スペース］を計算終了時に使うと，なんとなく心
　　　　が落ち着きます(笑)．
　　　　［ＳＴＯＰ］で終了です．お疲れさまでした．
　　
　　-m : マウス・モード
　　　　マウスでそろばんをはじきます．
　　　　左クリックで珠をはじきます．
　　　　右クリックはその桁をゼロクリアします．ご破算のときには右クリック
　　　　したままそろばん全体をなめるように動かすと楽です．
　　　　［ＳＴＯＰ］キーで終了です．
　　
　　オプション無し : デモンストレーション・モード
　　　　そろばんの簡単な使い方を説明してから，加算のデモンストレーション
　　　　を行ないます．
　　　　いやになったら［ＳＴＯＰ］キーを押してください．


　◎　その他・解説・雑感

　　幼い頃，よく姉とそろばんの競争をしました．　１＋２＋……と，１０まで
　　の総和を求めるまでの時間を計るのですが，どう頑張っても姉に勝つことは
　　出来ませんでした．
　　
　　まぁ，それはそれとして．
　　
　　そんなわけで，今回の“初めて”は『再帰』と『マウス』です．
　　再帰に関しては，実験的に「ハノイの塔」なんかで使ったことはあったので
　　すが，今回のように実用(笑)プログラムでまともに使ったのは初めてなので
　　した．
　　
　　マウス・モードは先にマウスドライバを組み込んでから実行してください．
　　いろんな種類のマウスドライバがあるそうですが，手元に資料のあるマウス
　　ドライバ（NEC の mouse.sys と Microsoft の mouse.com）にしか対応して
　　いません．
　　他のマウスドライバをお使いの方には，申し訳ないです．


　◎　とってもたくさんのありがとうをあなたに……

　　そろばんグラフィックに関しては，大石弘樹君（X68000ユーザー）の助言を
　　もらいました．　あと，オープニングのダジャレも，大石君にいくつか考え
　　てもらいました．　ども，ありがとネー．
　　
　　NIFTY-Serve FGALAMの蓬さんには，“謎のハングアップ事件”のデバッグに
　　つき合っていただきました．　蓬さんがいなかったら，きっとこの問題は解
　　決しなかったでしょう．　ども，ありがとネー．


　◎　転載について

　　このソフトはフリーウェアですが，このソフトおよびドキュメントの改変，
　　転載，その他もろもろはなんでもアリアリのアリです．
　　煮るなり焼くなり，好きにしてください(笑)．
　　ただし，ＲＢＢＳ（アマチュア無線を使ったパソコン通信です）への転載だ
　　けは，絶対にしないで下さい．


         CP NET        : UME
         NEMUKO NET    : UME
         ASCII NET PCS : pcs33097 (UME20)
         NIFTY Serve   : NBD00756 (UME20 | JF8ERQ)


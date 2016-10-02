# ProconPuzzle

input_processor.cppは入力段処理．使うときには`#include"input_processor.h"`忘れずに．  
Source.cppはそのデモ．

## 仕様

インスタンスを作った時にUSBカメラを使う準備を自動的に行う．

`find_pieces()`はUSBカメラの入力を元に二値化されたピース`bin_img`，コーナーの座標のvector`corners`を含む`piece_info`構造体のvectorを返す．  
引数に画像ファイルのパスを使う事も出来る．

### 操作
* Tキー *T*ake a picture ピースを追加
<!--* Fキ-->
* Eキー *E*xit ピース確定
* Canny_arg_1 輪郭抽出の為の良く分からないパラメータ1
* Canny_arg_2 輪郭抽出の為の良く分からないパラメータ2
* min_piece_area ピースの最小面積[pixel<sup>2</sup>]
* max_piece_area ピースの最大面積[pixel<sup>2</sup>]
* corner_detect_threshold_rate 角と角の最小距離(?)．ピースの外接矩形の最小辺長に対する割合で指定[%]

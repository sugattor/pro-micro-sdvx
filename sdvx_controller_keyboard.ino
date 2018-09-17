// ボルテコン用Pro Microスケッチ v0.2 Cloud対応用
#include <Keyboard.h>

// 出力するキーの定義
#define BT_A 'd'
#define BT_B 'f'
#define BT_C 'j'
#define BT_D 'k'
#define FX_L 'c'
#define FX_R 'm'
#define VOL_L_CW 'w'
#define VOL_L_CCW 'q'
#define VOL_R_CW 'p'
#define VOL_R_CCW 'o'
// #define ENTR KEY_RETURN

// 追加カウント回数(チャタリング防止用)
#define MIN_GAUGE 2  // ボタンが押されるまで何ループ必要か(増やせばラグが増える)
#define VOL_DELAY 15 // つまみの停止を検知してから何ループ後にキーを離すか


// グローバル変数宣言
// チャタリング防止用ゲージ変数
volatile int gaugeA = 0;
volatile int gaugeB = 0;
volatile int gaugeC = 0;
volatile int gaugeD = 0;
volatile int gaugeL = 0;
volatile int gaugeR = 0;
volatile int releaseL = 0;
volatile int releaseR = 0;

// ロータリーエンコーダの現在の値
volatile int valueL = 0;
volatile int valueR = 0;
// ロータリーエンコーダの前回読み取った値
volatile uint8_t prevL = 0;
volatile uint8_t prevR = 0;

// ロータリーエンコーダの現在と前回の差を取った時のパターン
static const int pattern[]={ 0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0 };

// ロータリーエンコーダの変化があるかどうか
volatile int checkL = 0;
volatile int checkR = 0;
volatile int flagL = 0;
volatile int flagR = 0;

void setup() 
{
  // ピンの動作設定 つまみ
  pinMode(3, INPUT_PULLUP);// L-A相は   D3,INT0  に接続
  pinMode(2, INPUT_PULLUP);// L-B相     D2,INT1
  pinMode(0, INPUT_PULLUP);// R-A相 RX1,D0,INT2
  pinMode(1, INPUT_PULLUP);// R-B相 TX0,D1,INT3
  
  // ボタン
  pinMode(4, INPUT_PULLUP);// BT-A     4,D4
  pinMode(5, INPUT_PULLUP);// BT-B     5,D5
  pinMode(6, INPUT_PULLUP);// BT-C     6,D6
  pinMode(7, INPUT_PULLUP);// BT-D     7,D7
  pinMode(8, INPUT_PULLUP);// FX-L     8,D8
  pinMode(9, INPUT_PULLUP);// FX-R     9,D9
  pinMode(10,INPUT_PULLUP);// START   10,D10
  
  // 波形変化で割り込みするように
  attachInterrupt(0, updateEncoderL, CHANGE);
  attachInterrupt(1, updateEncoderL, CHANGE);
  attachInterrupt(2, updateEncoderR, CHANGE);
  attachInterrupt(3, updateEncoderR, CHANGE);
  
  // キーボード使用
  Keyboard.begin();
}

// 左つまみ 割り込み
void updateEncoderL()
{
  uint8_t a = digitalRead(2);// D2(左A相)の状態を取得
  uint8_t b = digitalRead(3);// D3(左B相)の状態を取得
 
  uint8_t ab = (a << 1) | b;// A相とB相を合わせた2ビット変数を作成
  uint8_t abab = (prevL << 2) | ab;// 前の状態と合わせた4ビットの変数を作成
  
  //Mouse.move(pattern[abab],0,0);// マウスX座標移動
  valueL += pattern[abab];
  prevL = ab;// 前の状態を記録
}

// 右つまみ 割り込み
void updateEncoderR()
{
  uint8_t c = digitalRead(0);// D0(右A相)の状態を取得
  uint8_t d = digitalRead(1);// D1(右B相)の状態を取得
  
  uint8_t cd = (c << 1) | d;// A相とB相を合わせた2ビット変数を作成
  uint8_t cdcd = (prevR << 2) | cd;// 前の状態と合わせた4ビットの変数を作成
  
  //Mouse.move(0,pattern[cdcd],0);// マウスY座標移動
  valueR += pattern[cdcd];// 表示用 動いた分だけ数値が変わる
  prevR = cd;// 前の状態を記録
}

// メインループ
void loop()
{
  // 各ボタンが押されているとき、対応するキーを押下状態にする
  if(digitalRead(4) == LOW) gaugeA++;
  if(gaugeA > MIN_GAUGE){
    Keyboard.press(BT_A);
    gaugeA = MIN_GAUGE;
  }
  if(digitalRead(5) == LOW) gaugeB++;
  if(gaugeB > MIN_GAUGE){
    Keyboard.press(BT_B);
    gaugeB = MIN_GAUGE;
  }
  if(digitalRead(6) == LOW) gaugeC++;
  if(gaugeC > MIN_GAUGE){
    Keyboard.press(BT_C);
    gaugeC = MIN_GAUGE;
  }
  if(digitalRead(7) == LOW) gaugeD++;
  if(gaugeD > MIN_GAUGE){
    Keyboard.press(BT_D);
    gaugeD = MIN_GAUGE;
  }
  if(digitalRead(8) == LOW) gaugeL++;
  if(gaugeL > MIN_GAUGE){
    Keyboard.press(FX_L);
    gaugeL = MIN_GAUGE;
  }
  if(digitalRead(9) == LOW) gaugeR++;
  if(gaugeR > MIN_GAUGE){
    Keyboard.press(FX_R);
    gaugeR = MIN_GAUGE;
  }
  
  if(digitalRead(10)== LOW) Keyboard.press(KEY_RETURN);
  
  // 各ボタンが離されているとき、対応するキーを離す
  if(digitalRead(4) == HIGH){
    Keyboard.release(BT_A);
    gaugeA=0;
  }
  if(digitalRead(5) == HIGH){
    Keyboard.release(BT_B);
    gaugeB=0;
  }
  if(digitalRead(6) == HIGH){
    Keyboard.release(BT_C);
    gaugeC=0;
  }
  if(digitalRead(7) == HIGH){
    Keyboard.release(BT_D);
    gaugeD=0;
  }
  if(digitalRead(8) == HIGH){
    Keyboard.release(FX_L);
    gaugeL=0;
  }
  if(digitalRead(9) == HIGH){
    Keyboard.release(FX_R);
    gaugeR=0;
  }
  
  if(digitalRead(10)== HIGH) Keyboard.release(KEY_RETURN);
  
  // 前ループのつまみの状態比較
  if(valueL == checkL) flagL = 0;
  if(valueL > checkL) flagL = 1;
  if(valueL < checkL) flagL = -1;
  if(valueR == checkR) flagR = 0;
  if(valueR > checkR) flagR = 1;
  if(valueR < checkR) flagR = -1;
  checkL = valueL;
  checkR = valueR;
  
  // 左つまみ→キーボード
  // 時計回り
  if(flagL > 0){
    Keyboard.release(VOL_L_CCW);
    Keyboard.press(VOL_L_CW);
    releaseL = 0;
  }
  // 反時計回り
  if(flagL < 0){
    Keyboard.release(VOL_L_CW);
    Keyboard.press(VOL_L_CCW);
    releaseL = 0;
  }
  // 離しているとき
  if(flagL == 0){
    releaseL++;
    if(releaseL >= VOL_DELAY){
      Keyboard.release(VOL_L_CW);
      Keyboard.release(VOL_L_CCW);
      valueL = 0;
      checkL = 0;
      releaseL = VOL_DELAY;
    }
  }
  
  // 右つまみ→キーボード
  // 時計回り
  if(flagR > 0){
    Keyboard.release(VOL_R_CCW);
    Keyboard.press(VOL_R_CW);
    releaseR = 0;
  }
  // 反時計回り
  if(flagR < 0){
    Keyboard.release(VOL_R_CW);
    Keyboard.press(VOL_R_CCW);
    releaseR = 0;
  }
  // 離しているとき
  if(flagR == 0){
    releaseR++;
    if(releaseR >= VOL_DELAY){
      Keyboard.release(VOL_R_CW);
      Keyboard.release(VOL_R_CCW);
      valueR = 0;
      checkR = 0;
      releaseR = VOL_DELAY;
    }
  }
}


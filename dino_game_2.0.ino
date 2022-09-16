// Mario's ideas
// Displaying cat image on 8x8 LED Matrix (Multiplexing)

// R0ws
#define R1 2
#define R2 3
#define R3 4
#define R4 5
#define R5 6
#define R6 7
#define R7 8
#define R8 9
//Columns
#define C1 10
#define C2 11
#define C3 12
#define C4 13
#define C5 A0
#define C6 A1
#define C7 A2
#define C8 A3
//____________________________________________variables____________________________________________
int FrameFlag;
bool Lost;
/*const*/ int FrameFlagLimit = 100;
int pause = 2000; //in micros
bool Matrix [8] [8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};
const bool LostMatrix [8] [8] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 1, 0, 1, 1, 0, 1, 1},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {1, 1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
};
bool StillMatrix [11] [10] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
bool blockmapout[7][4][4] = {{
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 1, 0},
    {0, 0, 0, 0}
  }, {
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  }, {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 0}
  }, {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  }, {
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  }, {
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 0}
  }, {
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0}
  }
};

void Clear() {               //clearer
  for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) Matrix[i][j] = 0; //clearer for pixels
}
//________________________________classes________________________________
class Button {
  private:
    bool preRead;
    bool buttonFlag; //0 when clicked, 1 when unpressed
    bool Reverse;// reverse read value
    int ReadValue;
    int Pin;
  public:
    bool CButton() {
      ReadValue = analogRead(Pin);
      if (Reverse) ReadValue = 1023 - ReadValue;
      if (buttonFlag && ReadValue < 30) {
        buttonFlag = 0;
        return 1;
      } else {
        if (ReadValue > 20) {
          buttonFlag = 1;
        }
        return 0;
      }
    }
    Button(int pin, bool reverse) {
      Pin = pin;
      Reverse = reverse;
    }
};
class Block {
  private:
    //bool TempMatrix[8][8];// for matrix falling when deleting rows
    bool TempBlockMap[4][4]; //for spinning
    bool BlockMap[4][4];
    unsigned short Px = 2;
    unsigned short Py = -1;
  public:
    void VerifyLose() {
      for (int i = 1; i < 9; i++)if (StillMatrix[2][i])Lost = 1;
    }
    void RowChecking() {
      for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++)if (!StillMatrix[9 - i][j + 1]) j = 8; else if (j == 7) {
            for (int l = 0; l < 8; l++)StillMatrix[9 - i][l + 1] = 0;
            for (int m = i + 1; m < 9; m++)for (int n = 0; n < 9; n++)StillMatrix[10 - m][n] = StillMatrix[9 - m][n];
            i = -1;
          }
      }
    }


    void VerifyBlock() { //verify if should be killed
      if (Py >= 0) {   //avoid spawnkill
        for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)if (BlockMap[i][j])if (StillMatrix[Py + i + 3][Px + j + 1]) {
                for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)if (BlockMap[i][j]) StillMatrix[i + Py + 2][j + Px + 1] = 1;
                Px = 2;
                Py = -1;
                SetBlockMap(blockmapout[random(7)]);
                VerifyLose();
              }
        RowChecking();
        VerifyLose();
      }
    }
    void SetBlockMap(bool blockMap[4][4]) {
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)BlockMap[i][j] = blockMap[i][j];
    }
    void FallShow() {
      int l = Py;
      while (l < 8) {
        for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)if (BlockMap[i][j])if (StillMatrix[l + i + 3][Px + j + 1]) {
                VerifyBlock();
                return;
              }
        l++;
        Py = l;
      }
    }
    void LeftShow() {
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)if (BlockMap[i][j])if (StillMatrix[Py + i + 2][Px + j])return; Px--;
    }
    void RightShow() {
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)if (BlockMap[i][j])if (StillMatrix[Py + i + 2][Px + j + 2])return; Px++;
    }
    void SpinShow() {
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)TempBlockMap[i][j] = BlockMap[i][j];
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)BlockMap[i][j] = TempBlockMap[j][3 - i];
    }
    void StillShow() {
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)if (BlockMap[i][j]) Matrix[i + Py][j + Px] = 1;
      for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++)if (StillMatrix[i + 2][j + 1]) Matrix[i][j] = 1;
    }
    void GShow() {
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)if (BlockMap[i][j])if (StillMatrix[Py + i + 3][Px + j + 1]) {
              VerifyBlock();
              return;
            }
      Py++;
    }

    Block(bool blockMap[4][4]) {
      for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++)BlockMap[i][j] = blockMap[i][j];
    }
};
Button UpStick(18, 1);
Button DownStick(18, 0);
Button RightStick(19, 1);
Button LeftStick(19, 0);
Block block(blockmapout[1]);
void setup() {
  randomSeed(56);
  Lost = 0;
  FrameFlag = 0;
  pinMode(1, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);
  pinMode(C7, OUTPUT);
  pinMode(C8, OUTPUT);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);
  digitalWrite(R5, HIGH);
  digitalWrite(R6, HIGH);
  digitalWrite(R7, HIGH);
  digitalWrite(R8, HIGH);
  digitalWrite(C1, LOW);
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  digitalWrite(C5, LOW);
  digitalWrite(C6, LOW);
  digitalWrite(C7, LOW);
  digitalWrite(C8, LOW);
}



void SelectRow(int row) {
  if (row == 1) digitalWrite(R1, HIGH); else digitalWrite(R1, LOW);
  if (row == 2) digitalWrite(R2, HIGH); else digitalWrite(R2, LOW);
  if (row == 3) digitalWrite(R3, HIGH); else digitalWrite(R3, LOW);
  if (row == 4) digitalWrite(R4, HIGH); else digitalWrite(R4, LOW);
  if (row == 5) digitalWrite(R5, HIGH); else digitalWrite(R5, LOW);
  if (row == 6) digitalWrite(R6, HIGH); else digitalWrite(R6, LOW);
  if (row == 7) digitalWrite(R7, HIGH); else digitalWrite(R7, LOW);
  if (row == 8) digitalWrite(R8, HIGH); else digitalWrite(R8, LOW);
}

void Set_LED_in_Active_Row(int column, int state) {
  if (column == 1) digitalWrite(C1, !state);
  if (column == 2) digitalWrite(C2, !state);
  if (column == 3) digitalWrite(C3, !state);
  if (column == 4) digitalWrite(C4, !state);
  if (column == 5) digitalWrite(C5, !state);
  if (column == 6) digitalWrite(C6, !state);
  if (column == 7) digitalWrite(C7, !state);
  if (column == 8) digitalWrite(C8, !state);
}
//    3
//  2    1
//    4
//functions
void MoveBlock(bool frame) {
  if (frame)block.GShow();
  if (RightStick.CButton()) {
    block.RightShow();
  } else if (LeftStick.CButton()) {
    block.LeftShow();
  } else if (UpStick.CButton()) {
    block.SpinShow();
    Serial.println("spin");
  } else if (DownStick.CButton()) {
    block.FallShow();
  }
  block.StillShow();
}
void loop() {
  static float FFLFloat = 100;  //frame flag limit float
  static int lostBlinkDuration = 2000;
  static int lostBlinkFlag = 0;
  if (!Lost) {
    Clear();
    if (FrameFlag < FrameFlagLimit) {
      FrameFlag++;
      //__________________________________________________still shows__________________________________________________
      MoveBlock(0);
    } else {
      FrameFlag = 0;
      //frame show
      MoveBlock(1);
      FFLFloat = FFLFloat - pow(FFLFloat, 2) / 250;
      FrameFlagLimit = 15 + round(FFLFloat);
      //frame show
    }
  } else { //Lost
    FFLFloat = 100;
    FrameFlagLimit = 100;
    for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++) {
        if (lostBlinkFlag < lostBlinkDuration / 2) {
          Matrix[i][j] = LostMatrix[i][j];
          lostBlinkFlag++;
        } else {
          Matrix[i][j] = !LostMatrix[i][j];
          lostBlinkFlag++;
        }
        if (lostBlinkFlag > lostBlinkDuration - 1)lostBlinkFlag = 0;
        StillMatrix[i + 2][j + 1] = 0;
      }
    if (DownStick.CButton()){
      Lost = 0;
      block.~Block();
      Block block(blockmapout[1]);
      Clear();
    }
  }
  for (int j = 0; j < 8; j++) {//stuff for showing
    SelectRow(j + 1);
    for (int i = 0; i < 8; i++) {
      Set_LED_in_Active_Row(i + 1 , Matrix[j][i]);

    }
    delayMicroseconds(pause);
  }
}

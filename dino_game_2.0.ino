//          ||
//crediting\||/
//          \/
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
//_______________________________________ variables _______________________________________
short i = 0;//for dino
int DinoSpeed;
bool ButtonPressed;
int pause;
const int DinoJPat [6] = {2, 3, 3, 3, 2, 0};
int Matrix [8] [8] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
};

void clear() {
  for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) Matrix[i][j] = 0; //clearer
}
bool Cbutton() {
  static unsigned short buttonFlag = DinoSpeed * 6;
  if (digitalRead(A4) == HIGH && buttonFlag >= DinoSpeed * 6 - 1) {
    buttonFlag = 0;
    return 1;
  } else if (buttonFlag < DinoSpeed * 6 - 1) {
      buttonFlag++;
    }
    return 0;
  }

//_______________________________________ classes _______________________________________
class Mover {
  private:
    bool Available;
    int Px;
    int Hight;
    void Show() {
      for (int i = 0; i < Hight; i++) {
        Matrix[7 - i][Px] = 1;
      }
    }
  public:
    void SetHight(int hight) {
      Hight = hight;
    }
    void Tick() {
      if (Available) {
        if (Px < 7) {
          Px++;
        } else {
          Available = 0;
        }
      }
    }
    void StillTick() {
      if (Available) {
        Show();
      }
    }
    void MakeAvailable(){
      Available = 1;
    }
    Mover() {
      Available = 1;
    }
};
class Dino {
    int dinoFrame = 5;
    void Show(){
      Matrix[7 - DinoJPat[dinoFrame]][1] = 1;
      Matrix[6 - DinoJPat[dinoFrame]][1] = 1;
    }
  public:
    //wich frame of the jump the dino is in
    void Tick() {
      Show();
      if (dinoFrame < 5)  {
        dinoFrame++;
      } else {
        dinoFrame = 5;
      }
      Serial.print(dinoFrame);
    }
    void Jump() {
      dinoFrame = 0;
      Show();
      // dinoFrame++;
    }
    void StillTick() {
      Show();
    }
};
Dino dino;

void setup() {
  Serial.begin(9600);
  DinoSpeed = 1;
  pause = 1;
  pinMode(A4, INPUT);
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

void loop() {
  clear();
  //dino.StillTick();

  if (i < 5) {
    i++;
    dino.StillTick();
  } else {
    if (Cbutton()) {
      dino.Jump();
    } else {
      dino.Tick();
    }
    i = 0;
  }

  //stuff for showing pixels
  for (int j = 0; j < 8; j++) {
    SelectRow(j + 1);
    for (int i = 0; i < 8; i++) {
      Set_LED_in_Active_Row(i + 1 , Matrix[j][i]);

    }
    delay(2 + pause);
  }

}

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
int MoverFrameLength;
int DinoFrameLength;
bool ButtonPressed;
int pause;
const int DinoJPat [6] = {2, 3, 3, 3, 2, 0};
int Matrix [8] [8] = {//for what is showed on the screen (pixels)
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};
int HitboxMatrix [8] [8] = {//for movers hitboxes
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

void clear() {
  for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) Matrix[i][j] = 0; //clearer for pixels
  for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) HitboxMatrix[i][j] = 0; //clearer for hitboxes
}
bool Cbutton() {
  static unsigned short buttonFlag = 6;
  if (digitalRead(A4) == HIGH && buttonFlag >= 6 - 1) {
    buttonFlag = 0;
    return 1;
  } else if (buttonFlag < 6 - 1) {
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
        HitboxMatrix[7 - i][Px] = 1;
      }
    }
  public:
    void SetHight(int hight) {  //number of pixels/0 means nothing
      Hight = hight;
    }
    void Tick() {               //
      if (Available) {
        if (Px > 0) {
          Px--;
        } else {
          Available = 0;
        }
      }
    }
    void StillTick() {          //
      if (Available) {
        Show();
      }
    }
    void MakeAvailable() {      //available means on screen
      Available = 1;
      Px = 7;
    }
    bool IsAvailable() {        //
      return Available;
    }
    Mover() {                   //
      Available = 0;
      Px = 7;
    }
};
class Dino {
    int dinoFrame;
    void Show() {
      Matrix[7 - DinoJPat[dinoFrame]][1] = 1;
      Matrix[6 - DinoJPat[dinoFrame]][1] = 1;
      if (HitboxMatrix[7 - DinoJPat[dinoFrame]][1] == 1 || HitboxMatrix[6 - DinoJPat[dinoFrame]][1] == 1) {
        DinoAlive = 0;
      }
    }
  public:
    //wich frame of the jump the dino is in
    bool DinoAlive;
    void Tick() {
      Show();
      if (dinoFrame < 5)  {
        dinoFrame++;
      } else {
        dinoFrame = 5;
      }
    }
    void Jump() {
      dinoFrame = 0;
      Show();
      // dinoFrame++;
    }
    void StillTick() {
      Show();
    }
    Dino() {
      dinoFrame = 5;
      DinoAlive = 1;
    }
};
Dino dino;
Mover mover[3];
void setup() {//_______________________setup()_______________________
  randomSeed(76786);
  Serial.begin(9600);
  mover[0].SetHight(2);
  mover[1].SetHight(3);
  mover[2].SetHight(4);
  MoverFrameLength = 40; //Mover frame length(how many frames in one Mover frame for all movers)
  DinoFrameLength = 10; //dino frame length(how many frames in one dino frame)
  pause = 1000; //frame length in micros
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
  const static int EndScreen [8] [8] = {//for what is showed on the screen (pixels)
  {0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0, 1, 1, 1},
  {0, 0, 0, 1, 1, 1, 1, 1},
  {1, 0, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},};
  static float MFLfloat = 40;//to store speed's original value
  static unsigned short S = 10;//spawn speed increases when decreasing this
  static unsigned short T = 0;//to swtch between frames
  static unsigned short t = random(100);//for tests(seperating movers)
  static unsigned short ii = 0;//for dino controller
  static unsigned short jj = 0;//for mover controller
  clear();
  dino.DinoAlive = 0;
  if(dino.DinoAlive){
  if (jj < MoverFrameLength) { //controls in what frames the Mover moves (mover controller)
    for (int i = 0; i < 3; i++)mover[i].StillTick(); //for loop to loop all objects in object array
    jj++;
  } else {
    for (int i = 0; i < 3; i++)mover[i].Tick();
    jj = 0;
    ////////////////////////////////////////////
    ////////////////////////////////////////////
    if (dino.DinoAlive) {
      Serial.println("alive");
    } else {
      Serial.println("dead");
    }
    ////////////////////////////////////////////
    ////////////////////////////////////////////
  }
  if (ii < DinoFrameLength) { //controls in what frames the dino moves or starts the jump animation (dino controller)
    dino.StillTick();
    ii++;
  } else {
    if (Cbutton()) {
      dino.Jump();
    } else {
      dino.Tick();
    }
    ii = 0;
  }
  ////////////
  t++;
  if (t > S) {                                  //spawning movers stuff
    if (random(10) == 1) {
      if (!mover[T].IsAvailable()) {  //available means on screen
        mover[T].MakeAvailable();
        mover[T].SetHight(random(1, 4));
      }
    }
    t = 0;
    T++;
    if (T > 2)T = 0;
    S = MoverFrameLength / 4;
    MFLfloat = MFLfloat - pow(MFLfloat, 2) / 100000;
    MoverFrameLength = 3 + round(MFLfloat);
  }
  }else{
    for(int i=0;i<8;i++)for(int j = 0;j<8;j++)Matrix[i][j] = EndScreen[i][j];
  }

  for (int j = 0; j < 8; j++) {                   //stuff for showing pixels
    SelectRow(j + 1);
    for (int i = 0; i < 8; i++) {
      Set_LED_in_Active_Row(i + 1 , Matrix[j][i]);

    }
    delayMicroseconds(pause);
  }
}

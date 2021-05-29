//Pin Definitions
//Columns - shared, anode
const int C5 = 0;
const int C4 = 1;
const int C3 = 2;
const int C2 = 3;
const int C1 = 4;
//Light rows - cathode
const int L1 = 5;
const int L2 = 6;
const int L3 = 7;
const int L4 = 8;
const int L5 = 16;
//Switch rows - active high
const int S1 = 13;
const int S2 = 12;
const int S3 = 11;
const int S4 = 10;
const int S5 = 9;
//RNG Seed
const int analog = 14;

//Variable Definitions:
//Light matrix
bool light[5][5] = {
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0}
};
//Previous cycle's button states
bool oldbutton[5][5] = {
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0}
};
//Current cycle's button states
bool newbutton[5][5] = {
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0}
};
//Cells to change:
bool delta[5][5] = {
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0}
};
//Starting position:
int startrow = 0;
int startcolumn = 0;
//Score
int score = 0;

void setup() {
  // put your setup code here, to run once:

  //PinMode

  pinMode(C5, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C1, OUTPUT);

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  pinMode(analog, INPUT);

  //Set all the light cathodes to high:
  digitalWrite(L1, HIGH);
  digitalWrite(L2, HIGH);
  digitalWrite(L3, HIGH);
  digitalWrite(L4, HIGH);
  digitalWrite(L5, HIGH);

  startup();

}

void loop() {
  // put your main code here, to run repeatedly:

  //Step 1: See if the game is complete:
  complete();

  //Step 2: Read the state of all the buttons and check for changes:
  buttons();

  //Step 3: Apply the changes to the light board:
  changes();

}

void changes(){
  //Checks where there was a button pressed, and changes the corresponding lights. 
  //The light pressed should flip, and the one above, below, to the right, and left as well. If at the edge of the board, don't worry about it. 
  
  for (int i = 0;i<5;i++){
    for (int j = 0;j<5;j++){
      //Read through the delta matrix to find any changes
      if (delta[i][j] == 1){
        //Set the value of this light to the opposite of the current value
        light[i][j] = !light[i][j];
        //Check if we are at the top of the board. If not, set the light above this one to the opposite value.
        if(j > 0){
          light[i][j-1] = !light[i][j-1];
        }
        //Check if we are at the bottom of the board. If not, set the light below this one to the opposite value.
        if(j < 4){
          light[i][j+1] = !light[i][j+1];
        }
        //Check if we are at the left of the board. If not, set the light to the left of this one to the opposite value.
        if(i > 0){
          light[i-1][j] = !light[i-1][j];
        }
        //Check if we are at the right of the board. If not, set the right to the right of this one to the opposite value. 
        if(i < 4){
          light[i+1][j] = !light[i+1][j];
        }
      }
    }   
  }
  //Once all the changes are applied, show the new lights:
  showlight();
}

void buttons(){
  //Reads the state of the buttons and compares it to the state's in the last cycle, to check for changes. 

  //Read the state of the buttons, save the values to the matrix newbutton
  digitalWrite(C1, HIGH);
  newbutton[0][0] = digitalRead(S1);
  newbutton[1][0] = digitalRead(S2);
  newbutton[2][0] = digitalRead(S3);
  newbutton[3][0] = digitalRead(S4);
  newbutton[4][0] = digitalRead(S5);
  digitalWrite(C1, LOW);
  digitalWrite(C2, HIGH);
  newbutton[0][1] = digitalRead(S1);
  newbutton[1][1] = digitalRead(S2);
  newbutton[2][1] = digitalRead(S3);
  newbutton[3][1] = digitalRead(S4);
  newbutton[4][1] = digitalRead(S5);
  digitalWrite(C2, LOW);
  digitalWrite(C3, HIGH);
  newbutton[0][2] = digitalRead(S1);
  newbutton[1][2] = digitalRead(S2);
  newbutton[2][2] = digitalRead(S3);
  newbutton[3][2] = digitalRead(S4);
  newbutton[4][2] = digitalRead(S5);  
  digitalWrite(C3, LOW);
  digitalWrite(C4, HIGH);
  newbutton[0][3] = digitalRead(S1);
  newbutton[1][3] = digitalRead(S2);
  newbutton[2][3] = digitalRead(S3);
  newbutton[3][3] = digitalRead(S4);
  newbutton[4][3] = digitalRead(S5); 
  digitalWrite(C4, LOW);
  digitalWrite(C5, HIGH);   
  newbutton[0][4] = digitalRead(S1);
  newbutton[1][4] = digitalRead(S2);
  newbutton[2][4] = digitalRead(S3);
  newbutton[3][4] = digitalRead(S4);
  newbutton[4][4] = digitalRead(S5);
  digitalWrite(C5, LOW);
  
  //Check if the button has been pressed, but disregard if it was pressed in the previous check as well.
  for (int i = 0;i<5;i++){
    for (int j = 0;j<5;j++){
      if ((oldbutton[i][j] == 0) && (newbutton[i][j] == 1)){
        delta[i][j] = 1;
      }
      else{
        delta[i][j] = 0;
      }
    }
  }

  //Replace the values in oldbutton with the ones in newbutton, set newbutton to all 0.
  for (int i = 0;i<5;i++){
    for (int j = 0;j<5;j++){
      oldbutton[i][j] = newbutton[i][j];
      newbutton[i][j] = 0;
    }
  }
}

void startup(){
  //Code to run when the system is first powered on, or when the game is solved.

  //Return the score to 0
  score = 0;

  //Clear the light array:
  for (int i = 0;i<5;i++){
    for (int j = 0;j<5;j++){
      light[i][j] = 0;
    }
  }
  
  //Play some cool light features:
  for (int i = 0;i<5;i++){
    for (int j = 0;j<5;j++){
      light[i][j] = 1;
      for (long delaytime = 0; delaytime <100; delaytime++){
        showlight();
      }
    }
  }
  
  //Clear the light array:
  for (int i = 0;i<5;i++){
    for (int j = 0;j<5;j++){
      light[i][j] = 0;
    }
  }
  //Set RNG and choose one light to be on. Set it to on.
  randomSeed(analogRead(analog));
  startrow = random(1,6);
  startcolumn = random(1,6);
  light[startrow][startcolumn] = 1;
  showlight();
}

void complete(){
  //See if the game is done yet
  for (int i = 0;i<5;i++){
    for (int j = 0;j<5;j++){
      if(light[i][j] == 0){
        score = score + 1;
      }
    }
  }
  if(score == 25){
    startup();
  } else{
    score = 0;
  }
}

void showlight(){
  //Used to display the current state of the light matrix. 
  for(int i = 0; i<5; i++){
    for(int j = 0; j<5; j++){
      int stat = light[i][j];
      //Turn the corresponding column hign if the value of stat is high
      if (j == 0){
        digitalWrite(C1, stat);
      }
      if (j == 1){
        digitalWrite(C2, stat);
      }
      if (j == 2){
        digitalWrite(C3, stat);
      }
      if (j == 3){
        digitalWrite(C4, stat);
      }
      if ( j == 4){
        digitalWrite(C5, stat);
      }
      
      //Set the corresponding row to low
      if (i == 0){
        digitalWrite(L1, LOW); 
      }
      if (i == 1){
        digitalWrite(L2, LOW);
      }
      if (i == 2){
        digitalWrite(L3, LOW);
      }
      if (i == 3){
        digitalWrite(L4, LOW);
      }
      if (i == 4){
        digitalWrite(L5, LOW);
      }
      //Return everything to default states:
        digitalWrite(C1, LOW);
        digitalWrite(C2, LOW);
        digitalWrite(C3, LOW);
        digitalWrite(C4, LOW);
        digitalWrite(C5, LOW);
        digitalWrite(L1, HIGH);
        digitalWrite(L2, HIGH);
        digitalWrite(L3, HIGH);
        digitalWrite(L4, HIGH);
        digitalWrite(L5, HIGH);
    }
  }
}

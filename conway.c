#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VIS_SIZE 40
#define WHOLE_SIZE VIS_SIZE + 2
#define INPUTA '#'
#define INPUTD 'O'
#define ALIVE '#'
#define DEAD '.'
// Conways game of life rendered on VIS_SIZE x VIS_SIZE
// with a single row border all around with DEAD cells to negate border effects
char screen[WHOLE_SIZE][WHOLE_SIZE];
char newstates[WHOLE_SIZE][WHOLE_SIZE];

// convert input simbols to preattier simbols for better viewing
// if you want to input with different simbols change INPUTA and INPUTD for ALIVE and DEAD RESPECTIVELY
int convert() {
  for(int i = 0; i < WHOLE_SIZE; i++) {
    for(int j = 0; j <WHOLE_SIZE; j++) {
      if (screen[i][j] == INPUTA)
        screen[i][j] = ALIVE;
      if (screen[i][j] == INPUTD)
        screen[i][j] = DEAD;
    }
  }
  return 0;
}

// initialize 'screen' with DEAD cells 
int initScreen(char arr[WHOLE_SIZE][WHOLE_SIZE]) {
  for(int i = 0; i < WHOLE_SIZE; i++) {
    for(int j = 0; j < WHOLE_SIZE; j++) {
      arr[i][j] = DEAD;
    }
  }
  return 0;
}

// read in input file and convert it to 2D matrix
int readInitCond(FILE* f) {
  char temp;
  for(int i = 1; i < WHOLE_SIZE; i++) {
    for(int j = 1; j < WHOLE_SIZE; j++) {
      fscanf(f, "%c", &temp);
      if(temp == '\n') {
        break;
      }
      screen[i][j] = temp;
    }
  }
  return 0;
}

// iterate over matrix and check each cells next generation state and save it seperate matrix
int checkStates() {
  int neighbours = 0;
  for(int i = 1; i < WHOLE_SIZE - 1; i++) {
    for(int j = 1; j < WHOLE_SIZE - 1; j++) {

      /* Neighbour counter */
      for(int k = -1; k < 2; k++) {
        for(int m = -1; m < 2; m++) {
          if(k == 0 && m == 0)
            continue;
          if (screen[i-k][j-m] == ALIVE) {
            neighbours++;
          }
        }
      }

      /* Rule actuator(The Rules Of The Game) */ 
      if (screen[i][j] == ALIVE) {
        if (neighbours < 2)
          newstates[i][j] = DEAD;
        if (neighbours == 2 || neighbours == 3)
          newstates[i][j] = ALIVE;
        if (neighbours > 3)
          newstates[i][j] = DEAD;
      }
      if (screen[i][j] == DEAD) {
        if (neighbours == 3)
          newstates[i][j] = ALIVE;
      }
      neighbours = 0;
    }
  }
  return 0;
}

// copy next generation states to current screen
int updateScreen() {
  for(int i = 1; i < WHOLE_SIZE - 1; i++) {
    for(int j = 1; j < WHOLE_SIZE - 1; j++) {
      screen[i][j] = newstates[i][j];
    }
  }
  return 0;
}

// print 2D array in an ok way
int printScreen() {
  for(int i = 1; i < WHOLE_SIZE - 1; i++) {
    for(int j = 1; j < WHOLE_SIZE - 1; j++) {
      printf(" %c", screen[i][j]);
    }
    printf("\n");
  }
  return 0;
}

int main() {
  FILE* f;
  if ((f = fopen("input.txt", "r")) == NULL)
    exit(1);
  
  // fill current screen and next gen state matricies with dead cells
  initScreen(screen);
  initScreen(newstates);
  
  // read in input file
  readInitCond(f);
  // convert input simbols to output simbols
  convert();

  // print starting gen
  printScreen();

  // infinitely calculate next generation
  while(1) {
    checkStates();
    updateScreen();
    printf("\033[2J"); // clear terminal C escape character BS
    printf("\033[0;0H");  // place cursor at top left also C escape char BS
    printScreen();
    fflush(stdout);
    usleep(1000 * 100);  // sleep in microseconds
  }

}

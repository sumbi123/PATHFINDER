/*
Author: Subash Lakshminarayanan
Date: 11/6/22
Description: The purpose is to make a program that does the pathfinding application from one point to another on a grid
Usage: ./wavepath
*/

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
using namespace std;

void PrintEnvironment (int**arr, int w, int h){ //prints out the environment made through user input of the field width and height

int mw = 3;
cout << setw(mw)<<" ";
for(int x = 0; x < w; x++){
    cout << setw(mw) << x; 
}
cout << endl;
for(int x = 0; x < h; x++){

    for(int y= 0; y < w; y++){
        cout << setw(mw) << arr[x][y];
    }
    cout << endl;
}
}

void PrintC(char** cArr, int w, int h) // the method performs the same function as the PrintEnvironment but instead as an character array
{
  int mw = 3;
  cout << setw(mw) << " ";
  for(int x = 0; x < w; x++){
    cout << setw(mw) << x;
  }
  cout << endl;
  for(int x = 0; x < h; x++){
    cout << setw(mw) << x;
    for(int y = 0; y < w; y++){
      cout << setw(mw) << cArr[x][y];
    }
    cout << endl;
  }
}


int mWave[8][2] =   // declares the difference in idex of the moore neighborhood of the current position
{
  {-1,+1}, // NW
  {0,+1},  // N
  {+1,+1}, // NE
  {-1,0},  // W
  {+1,0},  // E
  {-1,-1}, // SW
  {0,-1},  // S
  {+1,-1}, // SE
};

// Checks if the x and y coordinates are within the array dimensions
bool Valid(int y, int x, int w, int h){
  return ((x >= 0) && (x < h) && (y >= 0) && (y < w));
}

// a function that is recursive running the wave for the each poisition on the field
void Run(int**arr, int y, int x, int w, int h){
  if(Valid(y, x, w, h) == false){
    return;
  }
  for(int a = 0; a < 8; a++){
    int xM = x + mWave[a][0];
    int yM = y + mWave[a][1];

    if((Valid(yM, xM, w, h)) && (arr[xM][yM] != -1)){  // checking wheather its in the bounds of field and if the coordinates match with that of an obstacle
      // Checking if Moore Neighbor reached for the first time
      if((arr[xM][yM] == 0) || (arr[x][y] + 1 < arr[xM][yM])){
	arr[xM][yM] = arr[x][y] + 1;
	Run(arr, yM, xM, w, h);
      }
    }
  }
}


void pathFinder(int** arr, char** cArr, int w, int h, int startx, int starty, int goalx, int goaly){ //method that creates the character array based of environment array
  // replaces with a # for all the obstacles
  for(int x = 0; x < h; x++){
    for(int y = 0; y < w; y++)
    {
      cArr[x][y] = (arr[x][y] == -1 ? '#' : ' ');
    }
  }
  // replaces with @ for start position and $ for the end goal position
  cArr[startx][starty] = '@';
  cArr[goalx][goaly] = '$';
  
  while((startx != goalx) && (starty != goaly)){
    // shows the moore neighbor
    int xmin = -1, ymin = -1;
    for(int a = 0; a < 8; a++){
      int xM = startx + mWave[a][0];
      int yM = starty + mWave[a][1];
      if(Valid(yM, xM, w, h) == false){
	continue;
      }
      if(arr[xM][yM] == -1){ // checks if Mneighbor is an obstacle
	continue;
      }
      if(((xmin == -1) && (ymin == -1)) || (arr[xM][yM] < arr[xmin][ymin])){
	xmin = xM;
	ymin = yM;
      }
    }
    cArr[xmin][ymin] = '*'; // adding a * to the min moore neighbor making that position the new start
    startx = xmin;
    starty = ymin;
  }
  cArr[goalx][goaly] = '$';
}

// if there is no path the character array is made without it or using *s
void PathnotFound(int** arr, char** cArr, int w, int h, int startx, int starty, int goalx, int goaly)
{
  for(int x = 0; x < h; x++){
    for(int y = 0; y < w; y++){
      cArr[x][y] = (arr[x][y] == -1 ? '#' : ' ');
    }
  }
  cArr[startx][starty] = '@';
  cArr[goalx][goaly] = '$';
}

int main(){
  printf("Welcome to Subash's CSE240 WaveFront Pather\n\n");
  printf("please tell me about the grid you want to generate\n");
  
  int w, h;
  // function runs until the user height and width requirement are met
  do{
    printf("Enter the width (at least 10): ");
    scanf("%d", &w);
  } while(w < 10);
  do{
    printf("Enter the height (at least 10): ");
    scanf("%d", &h);
    printf("\n");
  } while(h < 10);
  
  //until the 2d array represents the environment it dynamically allocating
  int** arr = new int*[h];
  for(int x = 0; x < h; x++){
    arr[x] = new int[w];
  }

  for(int x = 0; x < h; x++){
    for(int y = 0; y < w; y++){ // Inializes values to zero
      arr[x][y] = 0;
    }
  }
  
  int per;
  printf("Enter the percent of impassible terrain (0 - 100): ");
  scanf("%d", & per);
  printf("\n");
  if(per > 35){
    char a[1];
    printf("Having a value greater than 35%% might create poor results, do you wish to continue? (y/n): ");
    scanf("%s", a);
    printf("\n");
    // function runs if user decides to change the percentage
    if(a[0] == 'n')
    {
      //function runs until the impassible percent is met
      do
      {
	printf("Enter the percent of impassible terrain (0-35): ");
	scanf("%d", & per);
	printf("\n");
      } while(per > 35);
    }
  }
  
  int obs = (w * h * per)/100;
  int count = 0; 
  int rx, cx;
  // randomizing obstacles based in the impassible percentage entered by user
  while(count < obs){
    rx = rand() % h;
    cx = rand() % w;
    
    if(arr[rx][cx] == 0){
      arr[rx][cx] = -1;
      count++;
    }
  }

  int goalx, goaly;
  printf("Please enter the Goal Position X: ");
  scanf("%d", &goalx);
  printf("Please enter the Goal Position Y: ");
  scanf("%d", &goaly);
  printf("\n");
  while((Valid(goalx, goaly, w, h) == false) || (arr[goaly][goalx] == -1)){
    if(Valid(goalx, goaly, w, h) == false){ 
      printf("Sorry, that position is out of bounds.\n\n");
    }
    if(Valid(goalx, goaly, w, h) == true){
      printf("Sorry, that position is inside an obstacle.\n\n");
    }
    printf("Please enter the Goal Position X: ");
    scanf("%d", &goalx);
    printf("Please enter the Goal Position Y: ");
    scanf("%d", &goaly);
    printf("\n");
  }
  arr[goaly][goalx] = 1; //setting goal point
  
  Run(arr, goalx, goaly, w, h);
  PrintEnvironment(arr, w, h);
  printf("\n");
  
  int startx, starty;
  printf("Please enter the Start Position X: ");
  scanf("%d", &startx);
  printf("Please enter the Start Position Y: ");
  scanf("%d", &starty);
  printf("\n");

  while((Valid(startx, starty, w, h) == false) || (arr[starty][startx] == -1) || (arr[starty][startx] == 1)){  // function runs until start position is in bounds of the array and not a obstacle or goal 
    if(Valid(startx, starty, w, h) == false)
    {
      printf("Sorry, that position is out of bounds.\n\n");
    } 
    if(Valid(startx, starty, w, h) == true){
      if(arr[starty][startx] == -1){
	printf("Sorry, that position is inside an obstacle.\n\n");
      }
      if(arr[starty][startx] == 1){
	printf("Sorry, that position is the goal position.\n\n");
      }
    }
    printf("Please enter the Start Position X: ");
    scanf("%d", &startx);
    printf("Please enter the Start Position Y: ");
    scanf("%d", &starty);
    printf("\n");
  } 

  char** cArr = new char* [h]; //dynamically allocates a 2d array
  for(int a = 0; a < h; a++){
    cArr[a] = new char[w];
  }

  // checking wheather the start poin is attainable and otputing the character without a path
  if(arr[starty][startx] == 0){
    printf("No Path Found\n");
    PathnotFound(arr, cArr, w, h, starty, startx, goaly, goalx);
    PrintC(cArr, w, h);
  }
  // instead outputs the character with the path
  else{
    pathFinder(arr, cArr, w, h, starty, startx, goaly, goalx);
    PrintC(cArr, w, h);
  }
  
  ofstream outfile ("wave.txt"); //makes the out file and adds the envrionment array and character array
  int mw = 3;                                                                                     
  outfile << setw(mw) << " ";
  for(int x = 0; x < w; x++){
    outfile << setw(mw) << x; 
  }
  outfile << endl;
  for(int x = 0; x < h; x++){
    outfile << setw(mw) << x; 
    for(int y = 0; y < w; y++){
      outfile << setw(mw) << arr[x][y];
    }
    outfile << endl;
  }
  outfile << endl << endl;
  outfile << setw(mw) << " ";
  for(int x = 0; x < w; x++){
  outfile << setw(mw) << x;
  }
  outfile << endl;
  for(int x = 0; x < h; x++){
    outfile << setw(mw) << x;                                                            
    for(int y = 0; y < w; y++){
      outfile << setw(mw) << cArr[x][y];
    }
    outfile << endl;
  }
  outfile.close();

  return 0;
}
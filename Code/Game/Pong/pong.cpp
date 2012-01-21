#include <Windows.h>
#include <xnamath.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "pong.h"
#define gdt 0.1f


void pong_test() {
  
  PongSystem pongsystem;
  
  pongsystem.Initialize();

  printf("Initial Values\n");
  pongsystem.ball.PrintInfo();
  _getch();
  HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD a = {0,0};
  system("CLS");

  for (auto i=0;i<100000;++i) {
    SetConsoleCursorPosition(handle_out,a);
    printf("Running iteration %d\n",i);
    pongsystem.Update(gdt);
    pongsystem.ball.PrintInfo();

  }


}


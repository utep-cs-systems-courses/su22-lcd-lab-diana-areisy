#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "screen.h"
#include "lcdutils.h"
#include "lcddraw.h"

void drawMiddleDashLine()
{
  fillRectangle(0, screenHeight/2, screenWidth, 1, COLOR_WHITE);
}

void draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 3, 3, color);
}

short ballDrawPos[2] = {screenWidth/2,screenHeight/2-4};         // Axis 0 for col, axis 1 for row
short ballControlPos[2] = {1+screenWidth/2, screenHeight/2-3};   // Axis 0 for col, axis 1 for row
short ballColLim[2] = {1, screenWidth-3}; // -3 because of ball size
short ballRowLim[2] = {1, screenHeight/2-3};// -3 because of ball size

void screen_update_ball()
{
  for (char axis = 0; axis < 2; axis++) 
    if (ballDrawPos[axis] != ballControlPos[axis]) // position changed?
      goto redraw;
  return;			// nothing to do
 redraw:
  draw_ball(ballDrawPos[0], ballDrawPos[1], COLOR_MAGENTA);  // erase
  draw_ball(ballDrawPos[0], screenHeight-ballDrawPos[1], COLOR_MAGENTA);  // erase
  for (char axis = 0; axis < 2; axis++) 
    ballDrawPos[axis] = ballControlPos[axis];
  draw_ball(ballDrawPos[0], ballDrawPos[1], COLOR_WHITE); // draw
  draw_ball(ballDrawPos[0], screenHeight-ballDrawPos[1], COLOR_WHITE); // draw
}

void position_update_ball()
{
  if (switches & SW1 && ballControlPos[1] > ballRowLim[0]) ballControlPos[1] -= 1;
  if (switches & SW2 && ballControlPos[1] < ballRowLim[1]) ballControlPos[1] += 1;
  if (switches & SW3 && ballControlPos[0] > ballColLim[0]) ballControlPos[0] -= 1;
  if (switches & SW4 && ballControlPos[0] < ballColLim[1]) ballControlPos[0] += 1;
}

short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;
  if (++secCount >= 20) {		// 12.5/sec
    position_update_ball();
    redrawScreen = 1;
    secCount = 0;
  }
}


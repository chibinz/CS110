// #include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "lcd/lcd.h"
#include "utils.h"

#define TOP 10
#define DOWN 70
#define RIGHT 150
#define LEFT 10

typedef struct
{
    // Top left coordinate
    int x;
    int y;

    // Number of pixels between top and down
    int length;
    int width;
} pad;

typedef struct
{
    // Top left Coordinates
    int x;
    int y;

    // Actually length/width of a square
    int size;

    // Velocity
    int vx;
    int vy;

} ball;

// Player 1 and 2
pad p1, p2;
// Score of p1, p2
int s1, s2;
// Pong
ball pong;

int ticks = 1;

void draw_ball(ball *b, u16 color)
{
    LCD_Fill(b->x, b->y, b->x + b->size, b->y + b->size, color);
}

void draw_pad(pad *p, u16 color)
{
    LCD_Fill(p->x, p->y, p->x + p->width, p->y + p->length, color);
}

void draw_score(int s, int x, u16 color)
{
    LCD_ShowNum(80 - x, 0, s, 1, color);
}

void draw_midline(u16 color) { LCD_DrawLine(80, 0, 80, 80, color); }

void clear()
{
    // Clear
    draw_ball(&pong, BLACK);
    draw_pad(&p1, BLACK);
    draw_pad(&p2, BLACK);
    draw_midline(WHITE);
}

void draw()
{
    // Draw
    draw_ball(&pong, GREEN);
    draw_pad(&p1, RED);
    draw_pad(&p2, BLUE);

    delay_1ms(20);
}

void reset()
{
    srand(ticks);

    init();

    LCD_Clear(BLACK);
}

void score(ball *b)
{
    draw_score(s1, -10, BLACK);
    draw_score(s2, 10, BLACK);

    if (b->x + b->size >= 160)
    {
        s2++;
        reset();
    }
    else if (b->x <= 0)
    {
        s1++;
        reset();
    }

    draw_score(s1, -10, WHITE);
    draw_score(s2, 10, WHITE);
}

void tick()
{
    clear();

    ticks += 1;

    ball_tick(&pong);

    cpu_tick(&p1);
    pad_tick(&p2);

    ball *b = &pong;
    pad *p = &p1;
    if ((b->y >= p->y) && (b->y <= p->y + p->length))
    {
        if (b->x <= LEFT + p->width)
        {
            b->vx = ((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
    }

    p = &p2;
    if ((b->y >= p->y) && (b->y <= p->y + p->length))
    {
        if (b->x + b->size >= RIGHT)
        {
            b->vx = -((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
    }

    collide_wall(&pong);
    score(&pong);

    // Draw pad, ball, midline, score, etc.
    draw();
}

void draw_ball(ball *b)
{
    LCD_Fill(b->x, b->y, b->x + b->size, b->y + b->size, GREEN);
}

void draw_pad(pad *p)
{
    LCD_Fill(p->x, p->y, p->x + p->width, p->y + p->length, RED);
}

void draw_score(int s, int x) { LCD_ShowNum(80 - x, 0, s, 1, WHITE); }

void draw_midline() { LCD_DrawLine(80, 0, 80, 80, WHITE); }

void draw()
{
    LCD_Clear(BLACK);
    draw_ball(&pong);
    draw_pad(&p1);
    draw_pad(&p2);
    draw_score(s1, -10);
    draw_score(s2, 10);
    draw_midline();
    // draw_score(Get_Button(0), -20);
}

void tick()
{
    ticks += 1;

    ball_tick(&pong);

    cpu_tick(&p1);
    pad_tick(&p2);

    ball *b = &pong;
    pad *p = &p1;
    if ((b->y >= p->y) && (b->y <= p->y + p->length))
    {
        if (b->x <= LEFT + p->width)
        {
            b->vx = ((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
    }

    p = &p2;
    if ((b->y >= p->y) && (b->y <= p->y + p->length))
    {
        if (b->x + b->size >= RIGHT)
        {
            b->vx = -((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
    }

    collide_wall(&pong);
    score(&pong);

    // Draw pad, ball, midline, score, etc.
    draw();
}
// #include <stdbool.h>
#include "lcd/lcd.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>

#define TOP 10
#define DOWN 70
#define RIGHT 150
#define LEFT 10

#define abs(x) x > 0 ? x : -(x)
#define sign(x) x > 0 ? 1 : -1

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

void pad_tick(pad *p)
{
    if (Get_Button(0))
        p->y += 2;
    else if (Get_Button(1))
        p->y -= 2;

    p->y = p->y <= TOP ? TOP : p->y;
    p->y = p->y + p->length >= DOWN ? DOWN - p->length : p->y;
}

void cpu_tick(pad *p)
{
    p->y = pong.y - (p->length / 2);
}

void ball_tick(ball *b)
{
    b->x += b->vx;
    b->y += b->vy;
}

void collide_pad(ball *b, pad *p)
{
    // On collision reverse pong's horizontal velocity
    if ((b->y >= p->y) && (b->y <= p->y + p->length))
    {
        if (b->x <= LEFT + p->width)
        {
            b->vx = ((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
        else if (b->x + b->size >= RIGHT)
        {
            b->vx = -((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
    }
}

void collide_wall(ball *b)
{
    // On collision reverse pong's vertical velocity
    if (b->y <= TOP || b->y >= DOWN)
    {
        b->vy = -(b->vy);
    }
}

void reset()
{

    srand(ticks);

    p1.x = LEFT;
    p2.x = RIGHT;
    p1.y = p2.y = 40;
    p1.length = p2.length = 20;
    p1.width = p2.width = 4;

    pong.x = 80;
    pong.y = 40;
    pong.size = 2;
    while (pong.vx == 0)
    {
        pong.vx = (rand() % 7) - 3;
        pong.vy = (rand() % 7) - 3;
    }
}

void score(ball *b)
{
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
}

void draw_ball(ball *b)
{
    LCD_Fill(b->x, b->y, b->x + b->size, b->y + b->size, GREEN);
}

void draw_pad(pad *p)
{
    LCD_Fill(p->x, p->y, p->x + p->width, p->y + p->length, RED);
}

void draw_score(int s, int x)
{
    LCD_ShowNum(80 - x, 0, s, 1, WHITE);
}

void draw_midline()
{
    LCD_DrawLine(80, 0, 80, 80, WHITE);
}

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

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>


#define sWIDTH 900
#define sHEGIHT 600
#define GRAVITY 1
#define DAMPENING .9

struct Circle
{
	int pos_x;
	int pos_y;
	int radius;
	int vel_x;
	int vel_y;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, int color)
{
	int xmin = circle.pos_x - circle.radius;
	int xmax = circle.pos_x + circle.radius;
	int ymin = circle.pos_y - circle.radius;
	int ymax = circle.pos_y + circle.radius;
	int rsquared = circle.radius * circle.radius;

	for (int x = xmin; x <= xmax; x++) {
		for (int y = ymin; y <= ymax; y++){
			if (((x - circle.pos_x) * (x - circle.pos_x) + (y - circle.pos_y) * (y - circle.pos_y)) <= rsquared)
			{
				SDL_Rect pixel = (SDL_Rect){ x, y, 1, 1 };
				SDL_FillRect(surface, &pixel, color);
			}
		}
	}
}

bool MouseInCircle(struct Circle circle) {
	int x;
	int y;
	
	SDL_GetMouseState(&x, &y);

	int rsquared = circle.radius * circle.radius;
	if (((x - circle.pos_x) * (x - circle.pos_x) + (y - circle.pos_y) * (y - circle.pos_y)) <= rsquared)
	{
		return true;
	}
	return false;
}

void MoveCircle(struct Circle* circle)
{
	int x;
	int y;
	SDL_GetMouseState(&x, &y);

	int newvx;
	int newvy;

	newvx = (x - circle->pos_x);
	newvy = (y - circle->pos_y);

	circle->pos_x = x;
	circle->pos_y = y;
	circle->vel_x = newvx;
	circle->vel_y = newvy;
}

void StepSimulation(struct Circle *circle)
{
	if (circle->pos_y >= (sHEGIHT - circle->radius))
	{
		circle->pos_y = sHEGIHT - circle->radius;
		circle->vel_y = circle->vel_y * -1 * DAMPENING;
		circle->pos_x += circle->vel_x;
		circle->pos_y += circle->vel_y;
	}
	else if (circle->pos_y <= (0 + circle->radius))
	{
		circle->pos_y = 0 + circle->radius;
		circle->vel_y = circle->vel_y * -1 * DAMPENING;
		circle->pos_x += circle->vel_x;
		circle->pos_y += circle->vel_y;
	}
	else if (circle->pos_x >= sWIDTH - circle->radius)
	{
		circle->pos_x = sWIDTH - circle->radius;
		circle->vel_x = circle->vel_x * -1;
		circle->pos_x += circle->vel_x;
		circle->pos_y += circle->vel_y;
	}
	else if (circle->pos_x <= 0 + circle->radius)
	{
		circle->pos_x = 0 + circle->radius;
		circle->vel_x = circle->vel_x * -1;
		circle->pos_x += circle->vel_x;
		circle->pos_y += circle->vel_y;
	}
	else
	{

		circle->vel_y += GRAVITY;
		circle->pos_x += circle->vel_x;
		circle->pos_y += circle->vel_y;
	}
}

int main() {
	printf("Hello world!");

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("Bouncy Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sWIDTH, sHEGIHT, 0);
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	int simulation_running = 1;
	int ball_grabbed = 0;
	int paused = 0;
	SDL_Event event;
	SDL_Rect eraserrect = (SDL_Rect){ 0, 0, sWIDTH, sHEGIHT };
	struct Circle ball = { 100, 100, 25, 2, 0 };

	while (simulation_running)
	{
		SDL_FillRect(surface, &eraserrect, 0x0f0f0f);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				simulation_running = 0;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					simulation_running = 0;
					break;
				}
				if (event.key.keysym.sym == SDLK_SPACE) {
					if (paused) {
						paused = 0;
					}
					else {
						paused = 1;
					}
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (MouseInCircle(ball))
					{
						ball_grabbed = 1;
					}
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					ball_grabbed = 0;
				}
			}
		}

		
		FillCircle(surface, ball, 0xffffff);
		if (!ball_grabbed && !paused) {
			StepSimulation(&ball);
		}
		else if (ball_grabbed){
			MoveCircle(&ball);
		}
		SDL_UpdateWindowSurface(window);



		SDL_Delay(20);
	}
	

	
	return 0;
}
#pragma once
#include "SDL.h"


enum KEYS_PRESSED_LIST
{
	KEY_ESCAPE, KEY_W, KEY_A, KEY_S, KEY_D, SIZE_OF_KEYS_PRESSED_ENUM
};


class Camera;

class Input
{
public:
	void Update(Camera& cam );
	bool KeyIsPressed(KEYS_PRESSED_LIST key);



	Input();
	~Input();

	static Input* Instance();

private:
	static Input* _instance;
	SDL_Event m_event;
	bool m_keysPressed[SIZE_OF_KEYS_PRESSED_ENUM];
};
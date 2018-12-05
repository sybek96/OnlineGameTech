#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <functional>
#include <vector>
#include <memory>
#include "Circle.h"
#undef main //some weird vodoo magic with sdl sdl_main is included automatically so have to do this define
//Deal with cyclical referencing
class Client;
class Game
{
public:
	Game(Client* session);
	~Game();
	//Starts up SDL and creates window
	bool init();

	//Loads media
	bool loadMedia();
	//method to load a texture
	SDL_Texture* loadTexture(std::string path);

	void loop();

	//Frees media and shuts down SDL
	void close();
	//draw method
	void draw();

	void setPlayer(Circle newPlayer);

	void setAuthorative(bool newState);

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* gScreenSurface = NULL;

	//The image we will load and show on the screen
	SDL_Surface* gXOut = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Current displayed texture
	SDL_Texture* gTexture = NULL;

	SDL_Texture* gTexture2 = NULL;

	SDL_Texture* textureGreenCircle = NULL;
	SDL_Texture* textureRedCircle = NULL;
	SDL_Texture* textureBlueCircle = NULL;

	std::vector<SDL_Texture*> m_wallTextures;

	Circle m_player;

	bool isAuthorative = false;
	//hold pointer to session to be able to call server functions in the game
	Client* session;
};

#include "Client.h"

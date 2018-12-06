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
	void update();

	void setPlayer(Circle newPlayer);
	void setEnemy(Circle enemy);
	void setEnemyPos(int x, int y);

	void setAuthorative(bool newState);
	bool isEnemyInit() { return m_enemyInit; }
	void setEnemyInit(bool state) { m_enemyInit = state; }
	Circle getPlayer() { return m_player; }
	void setEndGame(bool state) { m_gameEnd = state; }
	bool getEndGame() { return m_gameEnd; }

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
	Circle m_enemy;

	bool isAuthorative = false;
	//hold pointer to session to be able to call server functions in the game
	Client* session;
	int m_playerSpeed = 2;
	bool m_enemyInit = false;
	//used when the server sends info on textures prior to loading
	bool m_loadedMedia = false;

	bool m_gameEnd = false;
};

#include "Client.h"

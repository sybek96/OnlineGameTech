#include "Game.h"


Game::Game(Client* socket)
	: session(socket)
{
	m_wallTextures.push_back(NULL);
	m_wallTextures.push_back(NULL);



}


Game::~Game()
{

}

bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool Game::loadMedia()
{
	m_loadedMedia = true;
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture("wall.png");
	if (gTexture == NULL)
	{
		printf("Failed to load texture image wall.png!\n");
		success = false;
	}
	gTexture2 = loadTexture("x.bmp");
	if (gTexture2 == NULL)
	{
		printf("Failed to load texture image x.png!\n");
		success = false;
	}
	textureBlueCircle = loadTexture("blue.png");
	if (textureBlueCircle == NULL)
	{
		printf("Failed to load texture image blue.png!\n");
		success = false;
	}
	textureRedCircle = loadTexture("red.png");
	if (textureRedCircle == NULL)
	{
		printf("Failed to load texture image red.png!\n");
		success = false;
	}
	textureGreenCircle = loadTexture("green.png");
	if (textureGreenCircle == NULL)
	{
		printf("Failed to load texture image green.png!\n");
		success = false;
	}
	switch (m_player.m_color)
	{
	case Color::RED:
		m_player.setTexture(textureRedCircle);
		break;
	case Color::BLUE:
		m_player.setTexture(textureBlueCircle);
		break;
	case Color::GREEN:
		m_player.setTexture(textureGreenCircle);
		break;
	default:
		break;
	}
	return success;
}

void Game::close()
{
	//Free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;


	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Game::draw()
{
	if (m_enemy.texture == NULL)
	{
		switch (m_enemy.m_color)
		{
		case Color::RED:
			m_enemy.setTexture(textureRedCircle);
			break;
		case Color::BLUE:
			m_enemy.setTexture(textureBlueCircle);
			break;
		case Color::GREEN:
			m_enemy.setTexture(textureGreenCircle);
			break;
		default:
			break;
		}
	}
	//Clear screen
	SDL_RenderClear(gRenderer);

	//Render background texture to screen
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

	m_player.draw(gRenderer);
	m_enemy.draw(gRenderer);

	//Update screen
	SDL_RenderPresent(gRenderer);
}

void Game::update()
{
	if (!m_gameEnd)
	{
		m_player.update();
		m_enemy.update();
		if (isAuthorative)
		{
			float vectPlayerToEnemyX = m_enemy.m_xPos - m_player.m_xPos;
			float vectPlayerToEnemyY = m_enemy.m_yPos - m_player.m_yPos;
			float lenPlayerToEnemy = sqrt((vectPlayerToEnemyX * vectPlayerToEnemyX) + (vectPlayerToEnemyY * vectPlayerToEnemyY)); //srt( a^2, b^2)
			float lenRadii = m_player.m_radius + m_enemy.m_radius;
			if (lenPlayerToEnemy <= lenRadii)
			{
				//collision!!
				std::cout << "COLLISION JUST HAPPENED OMG" << std::endl;
				m_gameEnd = true;
				session->sendEndGame(m_gameEnd);
			}
			
		}
	}
}

void Game::setPlayer(Circle newPlayer)
{
	m_player = newPlayer;
}

void Game::setEnemy(Circle enemy)
{
	m_enemy.m_xPos = enemy.m_xPos;
	m_enemy.m_yPos = enemy.m_yPos;
	m_enemy.playerRect = enemy.playerRect;
	m_enemy.m_color = enemy.m_color;
	switch (m_enemy.m_color)
	{
	case Color::RED:
		m_enemy.setTexture(textureRedCircle);
		break;
	case Color::BLUE:
		m_enemy.setTexture(textureBlueCircle);
		break;
	case Color::GREEN:
		m_enemy.setTexture(textureGreenCircle);
		break;
	default:
		break;
	}
}

void Game::setEnemyPos(int x, int y)
{
	m_enemy.m_xPos = x;
	m_enemy.m_yPos = y;
}

void Game::setAuthorative(bool newState)
{
	isAuthorative = newState;
}

SDL_Texture* Game::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void Game::loop()
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					switch (e.type)
					{
					case SDL_KEYDOWN:
						if (e.key.keysym.sym == SDLK_DOWN)
						{

						}
						if (e.key.keysym.sym == SDLK_ESCAPE) //if escape pressed undo last command
						{
							quit = true;
						}
						if (e.key.keysym.sym == SDLK_d)
						{
							m_player.m_xPos+= m_playerSpeed;
							session->SendCircle(m_player);
						}
						if (e.key.keysym.sym == SDLK_a)
						{
							m_player.m_xPos-= m_playerSpeed;
							session->SendCircle(m_player);
						}
						if (e.key.keysym.sym == SDLK_w)
						{
							m_player.m_yPos-= m_playerSpeed;
							session->SendCircle(m_player);
						}
						if (e.key.keysym.sym == SDLK_s)
						{
							m_player.m_yPos+= m_playerSpeed;
							session->SendCircle(m_player);
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						break;
					default:
						break;
					}
				}
				update();
				draw();
			}
		}
	}

	//Free resources and close SDL
	close();
}

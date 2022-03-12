// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;//sera usado para setar a altura de alguns objetos
const float paddleH = 100.0f;//tamanho da raquete
const float paddle2H = 100.0f;//tamanho da raquete

Game::Game()
:mWindow(nullptr)//para criar uma janela
,mRenderer(nullptr)//para fins de renderiza��o na tela
,mTicksCount(0)//para guardar o tempo decorrido no jogo
,mIsRunning(true)//verificar se o jogo ainda deve continuar sendo executado
,mPaddleDir(0)//direcao da bolinha
,mPaddle2Dir(0)//direcao da bolinha
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	scoreboard.x = 512.0f;
	scoreboard.y = 700.0f;
	mPaddlePos.x = 10.0f;//posi��o inicial da raquete eixo x
	mPaddlePos.y = 768.0f/2.0f;//posi��o inicial da raquee eixo y
	mPaddle2Pos.x = 1004.0f; //posi��o inicial da raquete eixo x
	mPaddle2Pos.y = 768.0f / 2.0f;//posi��o inicial da raquee eixo y
	mBallPos.x = 1024.0f/2.0f;//posi��o da bola eixo x
	mBallPos.y = 768.0f/2.0f;//posi��o da bola eixo y
	mBallVel.x = -200.0f;//velocidade de movimenta��o da bola no eixo x
	mBallVel.y = 300.0f;//velocidade de movimenta��o da bola no eixo y

	mBall2Pos.x = 1200.0f / 2.0f;//posi��o da bola eixo x
	mBall2Pos.y = 1200.0f / 2.0f;//posi��o da bola eixo y
	mBall2Vel.x = -100.0f;//velocidade de movimenta��o da bola no eixo x
	mBall2Vel.y = 200.0f;//velocidade de movimenta��o da bola no eixo y

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;//evento, inputs do jogador s�o armazenados aqui
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard - podemos buscar por alguma tecla espec�fica pressionada no teclado, nesse caso, Escape
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys - atualize a dire��o da raquete com base na entrada do jogador
	// W -> move a raquete para cima, S -> move a raquete para baixo
	mPaddleDir = 0;
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 3;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 3;
	}
	if (state[SDL_SCANCODE_O])
	{
		mPaddle2Dir -= 3;
	}
	if (state[SDL_SCANCODE_L])
	{
		mPaddle2Dir += 3;
	}
}

void Game::UpdateGame()
{
	// Espere que 16ms tenham passado desde o �ltimo frame - limitando os frames
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time � a diferen�a em ticks do �ltimo frame
	// (convertido pra segundos) - calcula o delta time para atualiza��o do jogo
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// "Clamp" (lima/limita) valor m�ximo de delta time
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// atualize a contagem de ticks par ao pr�ximo frame
	mTicksCount = SDL_GetTicks();
	
	// atualiza a posi��o da raquete
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// verifique que a raquete n�o se move para fora da tela - usamos "thickness", que definimos como a altura dos elementos
		if (mPaddlePos.y < (paddleH/2.0f + thickness))
		{
			mPaddlePos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	// atualiza a posi��o da raquete 2
	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;
		// verifique que a raquete n�o se move para fora da tela - usamos "thickness", que definimos como a altura dos elementos
		if (mPaddle2Pos.y < (paddle2H / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddle2H / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (768.0f - paddle2H / 2.0f - thickness))
		{
			mPaddle2Pos.y = 768.0f - paddle2H / 2.0f - thickness;
		}
	}
	
	// atualiza a posi��o da bola com base na sua velocidade
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
	
	// atualiza a posi��o da bola se ela colidiu com a raquete
	float diff = mPaddlePos.y - mBallPos.y;

	//pegue o valor absoluto de diferen�a entre o eixo y da bolinha e da raquete
	//isso � necess�rio para os casos em que no pr�ximo frame a bolinha ainda n�o esteja t�o distante da raquete
	//verifica se a bola est� na area da raquete e na mesma posi��o no eixo x
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// A diferen�a no eixo y y-difference is small enough
		diff <= paddleH / 2.0f &&
		// Estamos na posicao x correta
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// A bolinha est� se movendo para a esquerda
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.1f;
	}

	float diff2 = mPaddle2Pos.y - mBallPos.y;

	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
	if (
		// A diferen�a no eixo y y-difference is small enough
		diff2 <= paddle2H / 2.0f &&
		// Estamos na posicao x correta
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// A bolinha est� se movendo para a esquerda
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	//Verifica se a bola saiu da tela (no lado esquerdo, onde � permitido)
	//Se sim, encerra o jogo
	// 
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
		mBallPos.x = 1024.0f / 2.0f;//posi��o da bola eixo x
		mBallPos.y = 768.0f / 2.0f;//posi��o da bola eixo y
	}

	// Atualize (negative) a velocidade da bola se ela colidir com a parede do lado direito
	// 
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	
	// Atualize (negative) a posi��o da bola se ela colidir com a parede de cima
	// 
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}

	// Atualize (negative) a posi��o da bola se ela colidiu com a parede de baixo
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------

	// atualiza a posi��o da bola com base na sua velocidade
	mBall2Pos.x += mBall2Vel.x * deltaTime;
	mBall2Pos.y += mBall2Vel.y * deltaTime;

	// atualiza a posi��o da bola se ela colidiu com a raquete
	float diff3 = mPaddlePos.y - mBall2Pos.y;
	//pegue o valor absoluto de diferen�a entre o eixo y da bolinha e da raquete
	//isso � necess�rio para os casos em que no pr�ximo frame a bolinha ainda n�o esteja t�o distante da raquete
	//verifica se a bola est� na area da raquete e na mesma posi��o no eixo x
	diff3 = (diff3 > 0.0f) ? diff3 : -diff3;
	if (
		// A diferen�a no eixo y y-difference is small enough
		diff3 <= paddleH / 2.0f &&
		// Estamos na posicao x correta
		mBall2Pos.x <= 25.0f && mBall2Pos.x >= 20.0f &&
		// A bolinha est� se movendo para a esquerda
		mBall2Vel.x < 0.0f)
	{
		mBall2Vel.x *= -1.4f;
	}

	float diff4 = mPaddle2Pos.y - mBall2Pos.y;

	diff4 = (diff4 > 0.0f) ? diff4 : -diff4;
	if (
		// A diferen�a no eixo y y-difference is small enough
		diff4 <= paddle2H / 2.0f &&
		// Estamos na posicao x correta
		mBall2Pos.x <= 25.0f && mBall2Pos.x >= 20.0f &&
		// A bolinha est� se movendo para a esquerda
		mBall2Vel.x < 0.0f)
	{
		mBall2Vel.x *= -1.0f;
	}

	//Verifica se a bola saiu da tela (no lado esquerdo, onde � permitido)
	//Se sim, encerra o jogo
	// 
	else if (mBall2Pos.x <= 0.0f)
	{
		mIsRunning = false;
		mBall2Pos.x = 1024.0f / 2.0f;//posi��o da bola eixo x
		mBall2Pos.y = 768.0f / 2.0f;//posi��o da bola eixo y
	}

	// Atualize (negative) a velocidade da bola se ela colidir com a parede do lado direito
	// 
	else if (mBall2Pos.x >= (1024.0f - thickness) && mBall2Vel.x > 0.0f)
	{
		mBall2Vel.x *= -1.0f;
	}

	// Atualize (negative) a posi��o da bola se ela colidir com a parede de cima
	// 
	if (mBall2Pos.y <= thickness && mBall2Vel.y < 0.0f)
	{
		mBall2Vel.y *= -1;
	}

	// Atualize (negative) a posi��o da bola se ela colidiu com a parede de baixo
	// Did the ball collide with the bottom wall?
	else if (mBall2Pos.y >= (768 - thickness) &&
		mBall2Vel.y > 0.0f)
	{
		mBall2Vel.y *= -1;
	}

	
}

//Desenhando a tela do jogo
void Game::GenerateOutput()
{
	// Setamos a cor de fundo par azul
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// limpa o back buffer
	SDL_RenderClear(mRenderer);

	// Desenha as paredes - mudamos a cor de mRenderer para o desenho dos retangulos que formaram as paredes
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 255, 255);
	
	// Desenha a parede de cima - desenhando um retangulo no topo da tela, coordenada x e y = 0, 0 representa o topo esquerdo
	//primeiro definimos as coordenadas e tamanhos do triangulo
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);//finalmente, desenhamos um retangulo no topo
	
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	//desenhamos as outras paredes apenas mudando as coordenadas de wall

	// parede de baixo
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	SDL_SetRenderDrawColor(mRenderer, 0, 255, 5, 255);

	// parede da direita
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = 1024;
	wall.h = thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	//como as posi��es da raquete e da bola ser�o atualizadas a cada itera��o do game loop, criamos "membros" na classe
	//Game.h para tal

	//mudar a cor da raquete
	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);

	//desenhando a raquete - usando mPaddlePos que � uma struc de coordenada que foi definida em Game.h
	// 
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),//static_cast converte de float para inteiros, pois SDL_Rect trabalha com inteiros
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);

	SDL_Rect paddle2{
		static_cast<int>(mPaddle2Pos.x),//static_cast converte de float para inteiros, pois SDL_Rect trabalha com inteiros
		static_cast<int>(mPaddle2Pos.y - paddle2H / 2),
		thickness,
		static_cast<int>(paddle2H)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);
	

	//desenhando a bola - usando mBallPos que � uma struc de coordenadas definida como membro em Game.h
	
	//mudar a cor do renderizador para a bola
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);
	// Draw ball
	SDL_Rect ball{	
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);	

	//mudar a cor do renderizador para a bola 2
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
	// Draw ball 2
	SDL_Rect ball2{
		static_cast<int>(mBall2Pos.x - thickness / 2),
		static_cast<int>(mBall2Pos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball2);

	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);

	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

//Para encerrar o jogo
void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);//encerra o renderizador
	SDL_DestroyWindow(mWindow);//encerra a janela aberta
	SDL_Quit();//encerra o jogo
}

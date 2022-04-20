// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
class Pedra : public Actor
{
public:
	Pedra(class Game* game);
	void UpdateActor(float deltaTime) override;
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
	int GetTipo()const { return numero; }

	
private:
	std::vector<class Pedra*> pedras;
	std::vector<std::pair<int, Pedra*>> pedrasApagar;

	int numero;
	float mRightSpeed;
	float mDownSpeed;

};


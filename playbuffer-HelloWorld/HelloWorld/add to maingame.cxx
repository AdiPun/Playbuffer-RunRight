//Player punches and creates a damaging invisible object
//void UpdateShip();
//void UpdateDebris();

enum ObjectTypes;
//{
//	TYPE_SHIP
//		TYPE_DEBRIS
//		TYPE_PUNCH
//}

int main(int argc, char* argv[])
{
	Play::CreateGameObjectByType(TYPE_SHIP, { 300,0 }, 20, "ship")
}

void update()
{
	UpdateShip();
	UpdateDebris();
}
//Spaceship 
void UpdateShip()
{

	GameObject& obj_ship = Play::GetGameObjectByType(TYPE_SHIP);

	int id_debris = Play::CreateGameObjectByType(TYPE_DEBRIS), obj_ship.pos, 2, "debris";//Debris spawns from ship

	int randomRoll_debris = Play::RandomRoll(4); //spawns a random debris each time

	switch (randomRoll)
	{
	case 1:
		Play::SetSprite(obj_debris, "other_debris", 0);
		break;
	case 2:
		Play::SetSprite(obj_debris, "another_debris", 0);
		break;
	case 3: //set sprite another one
	}

	obj_ship.velocity.y = Point2f(0, Play::RollRandomRange(-2, 2 * 6)); //Make it bob up and down

	Play::UpdateGameObject(obj_ship);
	if (Play::IsObjectLeavingDisplayArea(obj_ship)
	{
		obj_ship.pos = obj_ship.oldPos;
			obj_ship.velocity *= -1;
	}

	Play::DrawObject(obj_ship);
}

//Debris
void UpdateDebris()
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	std::vector<int> vDebris = Play::CollectGameObjectIDsByType(TYPE_DEBRIS);

	for (int id_debris : vDebris)
	{
		GameObject& obj_debris = Play::GetGameObject(id_debris);
	}
	obj_debris.velocity = { -0.08f,-0.1f };//Move towards the left thrown up
	obj_debris.acceleration = { 0,0.9f };//Fall down

	if (Play::IsColliding(obj_debris, obj_player))
	{
		//gState.pState = STATE_DEAD;
		Play::StopAudioLoop("music");

		Play::PlayAudio("die");

		obj_player.pos = { -100, -100 };
	}
	Play::UpdateGameObject(obj_debris);
	if (Play::IsObjectLeavingDisplayArea(obj_debris, Play::VERTICAL)
	{
		obj_debris.pos = obj_debris.oldPos
			obj_debris.velocity.y = -0.7f; //Bounce up
	}
	Play::DrawObjectRotated(obj_debris); //Draws the object rotated if its been rotated

		if (!Play::IsVisible(obj_debris))
		{
			Play::DestroyGameObject(id_debris);
		}
}

void UpdateAirborneControls()
{
	//under spacebar
	//change gState.pState to 
}

void UpdatePlayer()
{
	//under STATE_ATTACK
	//Create game object by type(TYPE_PUNCH,on player's punch,hitbox,"some flash effect would be cool")
	//Change gState.PunchState to ON
	//Wait for animation to finish
	//Change gState.PunchState to OFF
	//Change to STATE_PLAY
}
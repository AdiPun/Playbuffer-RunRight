#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

int DISPLAY_WIDTH = 320;
int DISPLAY_HEIGHT = 240;
int DISPLAY_SCALE = 2;

enum PlayerState
{
	STATE_APPEAR = 0,
	STATE_AIRBORNE,
	STATE_ROLL,  //If debris and player state_roll colide, debris state is inactive
	STATE_ATTACK, //Merge Attack and airattack, if gstate.pstate = airborne statement
	STATE_PLAY,
	STATE_DEAD,
};

enum DebrisState
{
	STATE_ACTIVE = 0, //Damaging
	STATE_INACTIVE, //Dodged
	STATE_DESTROYED, //Attacked by player
};

enum BackgroundState
{
	STATE_STATIONARY = 0,
	STATE_FORWARDS,
	STATE_BACKWARDS,
};

struct GameState
{
	int score{ 0 };
	PlayerState pState{ STATE_APPEAR };
	BackgroundState bState{ STATE_STATIONARY };
	DebrisState dState{ STATE_ACTIVE };
};

GameState gState;

enum GameObjectType
{
	TYPE_NULL = -1,
	TYPE_PLAYER,
	TYPE_FCLOUD,
	TYPE_NCLOUD,
	TYPE_MOUNTAIN,
	TYPE_NMOUNTAIN,
	TYPE_ROAD,
	TYPE_TREE,
	TYPE_SHIP,
	TYPE_DEBRIS,
	TYPE_DESTROYED,
	TYPE_INERT,
	TYPE_TEXT,
};

void HandlePlayerControls();		//UpdatePlayer()
void UpdateShip();
void UpdateDebris();
void HandleAirbornePlayerControls();//UpdatePlayer()
void UpdateBackgrounds();			//HandlePlayerControls()
void UpdateDestroyed();
void UpdateInert();
void UpdatePlayer();

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	Play::CentreAllSpriteOrigins();
	Play::LoadBackground("Data\\Backgrounds\\background.png");
	Play::StartAudioLoop("music");
	Play::CreateGameObject(TYPE_FCLOUD, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 0, "fclouds");
	Play::CreateGameObject(TYPE_NCLOUD, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 0, "nclouds");
	Play::CreateGameObject(TYPE_MOUNTAIN, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 0, "mountains");
	Play::CreateGameObject(TYPE_NMOUNTAIN, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 0, "nmountains");
	Play::CreateGameObject(TYPE_ROAD, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 0, "road");
	Play::CreateGameObject(TYPE_TREE, { DISPLAY_WIDTH / 2, 110 }, 0, "trees");
	Play::CreateGameObject(TYPE_SHIP, { 260, DISPLAY_HEIGHT / 2 }, 10, "ship");
	Play::CreateGameObject(TYPE_PLAYER, { 70, 25 }, 15, "player_idle");
	Play::CreateGameObject(TYPE_TEXT, { DISPLAY_WIDTH / 2, 15 }, 0, "text");
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate(float elapsedTime)
{
	Play::DrawBackground();
	UpdateBackgrounds();
	UpdatePlayer();
	UpdateShip();
	UpdateDebris();
	UpdateDestroyed();
	UpdateInert();
	Play::PresentDrawingBuffer();
	return Play::KeyDown(VK_ESCAPE);
}
// Gets called once when the player quits the game 
int MainGameExit(void)
{
	Play::DestroyManager();
	return PLAY_OK;
}

void HandlePlayerControls()
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	if (Play::KeyDown(VK_RIGHT))
	{
		gState.bState = STATE_FORWARDS;
		Play::SetSprite(obj_player, "player_run", 0.2f); //Run right
	}
	else if (Play::KeyDown(VK_LEFT))
	{
		gState.bState = STATE_BACKWARDS;
		Play::SetSprite(obj_player, "player_leftrun", 0.2f); //Run left
	}
	else if (Play::IsAnimationComplete(obj_player))
	{
		gState.bState = STATE_STATIONARY;
		Play::SetSprite(obj_player, "player_idle", 0.15f); //Idle
	}

	if (Play::KeyPressed(VK_SPACE))
	{
		gState.pState = STATE_ATTACK;
		//Add sound effect!
	}

	if (Play::KeyPressed(VK_UP))
	{
		obj_player.velocity.y = -9;
		Play::SetSprite(obj_player, "player_jump", 0.05f);
		gState.pState = STATE_AIRBORNE;
		//Add sound effect!
	}

	if (Play::KeyPressed(VK_DOWN))
	{
		gState.bState = STATE_FORWARDS;
		Play::SetSprite(obj_player, "player_roll", 0.2f);
		gState.pState = STATE_ROLL; //ROLL
	}
}

void HandleAirbornePlayerControls()
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	if (Play::KeyDown(VK_RIGHT))
	{
		gState.bState = STATE_FORWARDS;
	}
	else if (Play::KeyDown(VK_LEFT))
	{
		gState.bState = STATE_BACKWARDS;
	}
	else
	{
		gState.bState = STATE_STATIONARY;
	}

	if (Play::KeyPressed(VK_SPACE)) //AIR SWORD
	{
		gState.pState = STATE_ATTACK;
		//Add sound effect!
	}
	if (Play::KeyPressed(VK_DOWN)) //AIR SPIN
	{
		gState.pState = STATE_ROLL;
	}
}

void UpdateShip() //Ship movement, item spawns and item movement
{

	GameObject& obj_ship = Play::GetGameObjectByType(TYPE_SHIP);

	if (Play::RandomRoll(100) == 1) //Spawns an item 1 in 150
	{
		int id_debris = Play::CreateGameObject(TYPE_DEBRIS, obj_ship.pos, 16, "debris_crate");//Debris spawns from ship
		GameObject& obj_debris = Play::GetGameObject(id_debris);
		obj_debris.velocity = Point2f(Play::RandomRollRange(-1.8, -1.3), Play::RandomRollRange(-5, -3));
		obj_debris.acceleration.y = 0.12f;
		obj_debris.rotSpeed = 0.1f;

		int randomRoll_debris = Play::RandomRoll(5); //spawns a random debris each time

		if (randomRoll_debris == 1)
		{
			Play::SetSprite(obj_debris, "debris_tv", 0);
			obj_debris.radius = 12;
		}
		else if (randomRoll_debris == 2)
		{
			Play::SetSprite(obj_debris, "debris_toaster", 0);
			obj_debris.radius = 14;
		}
		else if (randomRoll_debris == 3)
		{
			Play::SetSprite(obj_debris, "debris_speaker", 0); //set their obj_debris.radius
			obj_debris.radius = 14;
		}
		else if (randomRoll_debris == 4)
		{
			Play::SetSprite(obj_debris, "debris_soup", 0); //set their obj_debris.radius
			obj_debris.radius = 14;
		}
		else if (randomRoll_debris == 5)
		{
			Play::SetSprite(obj_debris, "debris_microwave", 0); //set their obj_debris.radius
			obj_debris.radius = 15;
		}

	}
	obj_ship.animSpeed = 0.2f;

	Play::UpdateGameObject(obj_ship);
	Play::DrawObject(obj_ship);
}

void UpdateDebris() //Debris collisions and interactions
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	std::vector<int> vDebris = Play::CollectGameObjectIDsByType(TYPE_DEBRIS);
	for (int id_debris : vDebris)
	{
		GameObject& obj_debris = Play::GetGameObject(id_debris);

		if (Play::IsColliding(obj_debris, obj_player))
		{
			if (gState.pState == STATE_ATTACK)
			{
				obj_debris.type = TYPE_DESTROYED;
				gState.score += 100;
			}
			else if (gState.pState == STATE_ROLL)
			{
				obj_debris.type = TYPE_INERT;
				gState.score += 50;
			}
			else
			{
				Play::StopAudioLoop("music");
				Play::PlayAudio("die");
				gState.pState = STATE_DEAD;
			}
		}

		Play::UpdateGameObject(obj_debris);

		if (Play::IsLeavingDisplayArea(obj_debris, Play::VERTICAL) && obj_debris.velocity.y > 0)
		{
			obj_debris.pos = obj_debris.oldPos;
			obj_debris.velocity.y = Play::RandomRollRange(-7.0f, -4.0f); //Bounce up random speed when it hits the ground
		}

		Play::DrawObjectRotated(obj_debris);

		if (!Play::IsVisible(obj_debris) && obj_debris.pos.x < 0) //Destroys the debris if the leave the left side of the screen
		{
			Play::DestroyGameObject(id_debris);
		}
	}
}

void UpdateDestroyed() //Explodes destroyed objects
{
	std::vector<int> vDead = Play::CollectGameObjectIDsByType(TYPE_DESTROYED);

	for (int id_dead : vDead)
	{
		GameObject& obj_dead = Play::GetGameObject(id_dead);
		Play::SetSprite(obj_dead, "explosion3", 0.6f);
		obj_dead.velocity *= 0.99f;
		obj_dead.rotation = 0;
		Play::UpdateGameObject(obj_dead);

		Play::DrawObject(obj_dead);

		if (!Play::IsVisible(obj_dead) || obj_dead.frame >= 10)
		{
			Play::DestroyGameObject(id_dead);
		}
	}
}

void UpdateInert() //Debris turns inert after coliding with 
{
	std::vector<int> vInert = Play::CollectGameObjectIDsByType(TYPE_INERT);

	for (int id_inert : vInert)
	{
		GameObject& obj_inert = Play::GetGameObject(id_inert);

		if (Play::IsLeavingDisplayArea(obj_inert, Play::VERTICAL) && obj_inert.velocity.y > 0)
		{
			obj_inert.pos = obj_inert.oldPos;
			obj_inert.velocity.y = Play::RandomRollRange(-7.0f, -4.0f); //Bounce up random speed when it hits the ground
		}

		Play::UpdateGameObject(obj_inert);


		Play::DrawObjectRotated(obj_inert);

		if (!Play::IsVisible(obj_inert) && obj_inert.pos.x < 0) //Destroys the debris if the leave the left side of the screen
		{
			Play::DestroyGameObject(id_inert);
		}
	}
}

void UpdatePlayer() //Updates player velocity, acceleration, pState, sets sprites
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	switch (gState.pState)
	{
	case STATE_APPEAR:
		Play::SetSprite(obj_player, "player_downjump", 0);
		if (obj_player.pos.y >= 210)
		{
			gState.pState = STATE_ROLL;
		}
		break;

	case STATE_AIRBORNE: //Air controls and animations when player is in the air
		obj_player.radius = 15;
		HandleAirbornePlayerControls();
		break;

	case STATE_ROLL: //Rolls after falling and landing or AIRSPIN if in the air
		obj_player.radius = 40;
		if (obj_player.pos.y < 210)
		{
			Play::SetSprite(obj_player, "player_airspin", 0.17f);

			if (Play::IsAnimationComplete(obj_player))
			{
				Play::SetSprite(obj_player, "player_downjump", 0);
				gState.pState = STATE_AIRBORNE;
			}
		}
		else
		{
			Play::SetSprite(obj_player, "player_roll", 0.2f);
			if (Play::IsAnimationComplete(obj_player))
			{
				gState.pState = STATE_PLAY;
			}
		}
		break;

	case STATE_ATTACK:
		obj_player.radius = 25;
		Play::SetSprite(obj_player, "player_sword", 0.3f);
		if (obj_player.pos.y < 210)
		{
			if (Play::IsAnimationComplete(obj_player))
			{
				Play::SetSprite(obj_player, "player_downjump", 0);
				gState.pState = STATE_AIRBORNE;
			}
		}
		else
		{
			if (Play::IsAnimationComplete(obj_player))
			{
				gState.pState = STATE_PLAY;
			}
		}
		break;

	case STATE_PLAY:
		obj_player.radius = 15;
		HandlePlayerControls();
		break;

	case STATE_DEAD:
		Play::SetSprite(obj_player, "player_hurt", 0.2f);
		if (Play::KeyPressed(VK_SPACE) == true)
		{
			gState.pState = STATE_APPEAR;
			Play::StartAudioLoop("music");
			gState.score = 0;

			for (int id_obj : Play::CollectGameObjectIDsByType(TYPE_DEBRIS))
				Play::GetGameObject(id_obj).type = TYPE_DESTROYED;
		}
		break;
	}

	obj_player.acceleration.y = 0.3; //Gravity
	if (obj_player.velocity.y > 4)
	{
		obj_player.acceleration.y *= 0.99;
	}

	Play::UpdateGameObject(obj_player);

	if (Play::IsLeavingDisplayArea(obj_player) && obj_player.velocity.y > 1) //Player rolls after dropping
	{
		gState.pState = STATE_ROLL;
	}
	if (Play::IsLeavingDisplayArea(obj_player)) //Stops player going out of bounds
	{
		obj_player.velocity.y = 0;
		obj_player.acceleration.y = 0;
		obj_player.pos = obj_player.oldPos;
	}

	Play::DrawObject(obj_player);
}

void UpdateBackgrounds() //Moves the backgrounds when arrow keys change their states
//Loops the background layers when their centres go off screen
{
	GameObject& obj_fcloud = Play::GetGameObjectByType(TYPE_FCLOUD);
	GameObject& obj_ncloud = Play::GetGameObjectByType(TYPE_NCLOUD);
	GameObject& obj_mountains = Play::GetGameObjectByType(TYPE_MOUNTAIN);
	GameObject& obj_nmountains = Play::GetGameObjectByType(TYPE_NMOUNTAIN);
	GameObject& obj_trees = Play::GetGameObjectByType(TYPE_TREE);
	GameObject& obj_road = Play::GetGameObjectByType(TYPE_ROAD);
	GameObject& obj_ship = Play::GetGameObjectByType(TYPE_SHIP);
	GameObject& obj_text = Play::GetGameObjectByType(TYPE_TEXT);

	Play::SetSprite(obj_fcloud, "fclouds", 0);
	Play::SetSprite(obj_ncloud, "nclouds", 0);
	Play::SetSprite(obj_mountains, "mountains", 0);
	Play::SetSprite(obj_nmountains, "nmountains", 0);
	Play::SetSprite(obj_trees, "trees", 0);
	Play::SetSprite(obj_road, "road", 0);
	Play::SetSprite(obj_text, "text", 0);

	switch (gState.bState)
	{
	case STATE_FORWARDS:
		obj_fcloud.velocity.x = -0.09f;
		obj_ncloud.velocity.x = -0.32f;
		obj_mountains.velocity.x = -0.62f;
		obj_nmountains.velocity.x = -0.82f;
		obj_trees.velocity.x = -1.52f;
		obj_road.velocity.x = -2.2f;
		obj_ship.velocity.x = -1;
		if (obj_ship.pos.x < 260)
		{
			obj_ship.pos = obj_ship.oldPos;
			obj_ship.velocity.x *= 0.99f;
		}
		break;
	case STATE_BACKWARDS:
		obj_fcloud.velocity.x = 0.12f;
		obj_ncloud.velocity.x = 0.32f;
		obj_mountains.velocity.x = 0.62f;
		obj_nmountains.velocity.x = 0.82f;
		obj_trees.velocity.x = 1.52f;
		obj_road.velocity.x = 2.2;
		obj_ship.velocity.x = 0.6f;
		break;
	case STATE_STATIONARY:
		obj_fcloud.velocity.x = 0.22f;
		obj_ncloud.velocity.x = 0.12f;
		obj_mountains.velocity.x *= 0.9f;
		obj_nmountains.velocity.x *= 0.91f;
		obj_trees.velocity.x *= 0.92f;
		obj_road.velocity.x *= 0.93f;
		obj_ship.velocity.x = 0.3f;
		break;
	}

	Play::UpdateGameObject(obj_fcloud);
	Play::UpdateGameObject(obj_ncloud);
	Play::UpdateGameObject(obj_mountains);
	Play::UpdateGameObject(obj_nmountains);
	Play::UpdateGameObject(obj_trees);
	Play::UpdateGameObject(obj_road);
	Play::UpdateGameObject(obj_text);

	if (obj_fcloud.pos.x < 0)
	{
		obj_fcloud.pos.x = 255; //Perfect loop
	}
	if (obj_ncloud.pos.x < 0)
	{
		obj_ncloud.pos.x = 286; //Perfect loop
	}
	if (obj_mountains.pos.x < 0)
	{
		obj_mountains.pos.x = 318; //Perfect loop
	}
	if (obj_nmountains.pos.x < 0)
	{
		obj_nmountains.pos.x = 319; //Perfect loop
	}
	if (obj_trees.pos.x < 0)
	{
		obj_trees.pos.x = 239; //Perfect loop
	}

	if (obj_road.pos.x < 0)
	{
		obj_road.pos.x = 320;
	}

	if (obj_fcloud.pos.x > 320)
	{
		obj_fcloud.pos.x = 65; //Perfect loop
	}
	if (obj_ncloud.pos.x > 320)
	{
		obj_ncloud.pos.x = 34; //Perfect loop
	}
	if (obj_mountains.pos.x > 320)
	{
		obj_mountains.pos.x = 2; //Perfect loop
	}
	if (obj_nmountains.pos.x > 320)
	{
		obj_nmountains.pos.x = 1; //Perfect loop
	}
	if (obj_trees.pos.x > 320)
	{
		obj_trees.pos.x = 81; //Perfect loop
	}
	if (obj_road.pos.x > 320)
	{
		obj_road.pos.x = 0; //Perfect loop
	}

	Play::DrawFontText("64px", "SCORE: " + std::to_string(gState.score),
		{ DISPLAY_WIDTH / 2, 45 }, Play::CENTRE);
	Play::DrawObject(obj_text);
	Play::DrawObject(obj_fcloud);
	Play::DrawObject(obj_ncloud);
	Play::DrawObject(obj_mountains);
	Play::DrawObject(obj_nmountains);
	Play::DrawObject(obj_trees);
	Play::DrawObject(obj_road);
}
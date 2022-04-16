// Mihail_Ica_Hover_Car_Racing.cpp: A program using the TL-Engine

/*Mihail Ica*/

#include <TL-Engine.h>
#include <sstream>
#include <iostream>
							// TL-Engine include file and namespace
using namespace tle;

struct PosVector {
	float x;
	float y;
	float z;
};



void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder("./Media");

	/**** Set up your scene here ****/

	const int checkpointNo = 3;
	const int isleNo = 6;
	const int wallNo = 3;
	const float kRotation = 90.0f;
	const float kMovementSpeed = 50.0f;
	const int negativeDirection = -1;
	const float kMouseRotation = 15.0f;
	const float kCameraMovementSpeed = 5.0f;
	const float playerCarRadius = 6.46;

	const PosVector checkpoint1Pos = { 0.0f,0.0f,0.0f };
	const PosVector checkpoint2Pos = { -20.0f,0.0f,120.0f };
	const PosVector checkpoint3Pos = { -25.0f,0.0f,56.0f };

	PosVector checkpointsArray[checkpointNo] = { checkpoint1Pos,checkpoint2Pos,checkpoint3Pos };

	const PosVector isle1 = { -10.0f,0.0f,40.0f };
	const PosVector isle2 = { 10.0f,0.0f,40.0f };
	const PosVector isle3 = { -10.0f,0.0f,56.0f };
	const PosVector isle4 = { 10.0f,0.0f,56.0f };
	const PosVector isle5 = { -8.0f,0.0f,130.0f };
	const PosVector isle6 = { 8.0f,0.0f,130.0f };

	PosVector islesArray[isleNo] = { isle1,isle2,isle3,isle4,isle5,isle6 };

	const PosVector wall1 = { -10.0f,0.0f,48.0f };
	const PosVector wall2 = { 10.0f,0.0f,48.0f };
	const PosVector wall3 = { 0.0f,0.0f,130.0f };

	PosVector wallsArray[wallNo] = { wall1,wall2,wall3 };

	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	IModel* checkpointModels[checkpointNo];

	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	IModel* isleModels[isleNo];

	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IModel* wallModels[wallNo];

	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	PosVector playerCar = { 0.0f,0.0f,-150.0f };

	IMesh* carMesh = myEngine->LoadMesh("hovercar.x");

	IModel* carModel = carMesh->CreateModel(playerCar.x, playerCar.y, playerCar.z);

	float playerCarX = carModel->GetX();
	float playerCarY = carModel->GetY();
	float playerCarZ = carModel->GetZ();

	ICamera* testCamera = myEngine->CreateCamera(kTargeted);
	testCamera->AttachToParent(carModel);

	const PosVector cameraLocalCoordinates = { 0.0f,15.0f,-25.0f };

	testCamera->RotateLocalX(kRotation / 5);
	testCamera->SetLocalPosition(cameraLocalCoordinates.x, cameraLocalCoordinates.y, cameraLocalCoordinates.z);

	IMesh* groundMesh = myEngine->LoadMesh("ground.x");
	IModel* ground = groundMesh->CreateModel();

	ISprite* backdrop = myEngine->CreateSprite("RedGlow.jpg", 0, 500);
	IFont* font = myEngine->LoadFont("Sans Sherif");

	float hitBoxMinX[checkpointNo];
	float hitBoxMaxX[checkpointNo];
	float hitBoxMinY[checkpointNo];
	float hitBoxMaxY[checkpointNo];
	float hitBoxMinZ[checkpointNo];
	float hitBoxMaxZ[checkpointNo];


	enum GameStates { Waiting, Countdown, Racing, GameWon, GameLost };
	GameStates gameState = Waiting;

	enum RaceStates { StageZeroComplete, StageOneComplete, StageTwoComplete, StageThreeComplete };
	RaceStates raceState = StageZeroComplete;

	float movementSpeed = kMovementSpeed;


	for (int i = 0; i < checkpointNo; i++)
	{
		checkpointModels[i] = checkpointMesh->CreateModel(checkpointsArray[i].x, checkpointsArray[i].y, checkpointsArray[i].z);
		if (i == 1)
		{
			checkpointModels[i]->RotateLocalY(kRotation);
		}

		/*Checkpoint bounding volumes*/

		hitBoxMinX[i] = checkpointModels[i]->GetX();
		hitBoxMaxX[i] = checkpointModels[i]->GetX();
		hitBoxMinY[i] = checkpointModels[i]->GetY();
		hitBoxMaxY[i] = checkpointModels[i]->GetY();
		hitBoxMinZ[i] = checkpointModels[i]->GetZ();
		hitBoxMaxZ[i] = checkpointModels[i]->GetZ();

	}

	for (int i = 0; i < isleNo; i++)
	{
		isleModels[i] = isleMesh->CreateModel(islesArray[i].x, islesArray[i].y, islesArray[i].z);
		if (i == 4 || i == 5)
		{
			isleModels[i]->RotateLocalY(kRotation);
		}
	}

	for (int i = 0; i < wallNo; i++)
	{
		wallModels[i] = wallMesh->CreateModel(wallsArray[i].x, wallsArray[i].y, wallsArray[i].z);
		if (i == 2)
		{
			wallModels[i]->RotateLocalY(kRotation);
		}
	}

	float timePassed = 0.0f;

	

	
	

	myEngine->Timer();
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		float frameTime = myEngine->Timer();
		
		/**** Update your scene each frame here ****/
		timePassed += frameTime;
		float playerCarX = carModel->GetX();
		float playerCarY = carModel->GetY();
		float playerCarZ = carModel->GetZ();

		stringstream gameStateText;
		stringstream timerText;
		stringstream debugText;
		

		if (gameState == Waiting)
		{
			timerText << "Hit Space to Start";
			font->Draw(timerText.str(), 0, 600, kBlack);

			gameStateText << "Status: Waiting ";
			movementSpeed = 0;
			
		}

		if (gameState == Countdown)
		{
			gameStateText << "Status: Countdown";
			

			if (timePassed < 1)
			{

				timerText << "3";
				font->Draw(timerText.str(), 0, 600, kBlack);
			}

			if (timePassed < 2 && timePassed > 1)
			{

				timerText << "2";
				font->Draw(timerText.str(), 0, 600, kBlack);
			}

			if (timePassed < 3 && timePassed > 2)
			{

				timerText << "1";
				font->Draw(timerText.str(), 0, 600, kBlack);
			}

			if (timePassed < 4 && timePassed > 3)
			{
				timerText << "GO";
				font->Draw(timerText.str(), 0, 600, kBlack);
				
				
			}
			if (timePassed < 5 && timePassed > 4)
			{
				gameState = Racing;
				gameStateText << "Status: Racing";
				movementSpeed = kMovementSpeed;
			}
		}

		if (gameState == Racing)
		{
			gameStateText << "Status: Racing";
			movementSpeed = kMovementSpeed;

			if (raceState == StageOneComplete)
			{
				debugText << "Stage 1 Complete";
			}

			else if (raceState == StageTwoComplete)
			{
				debugText << "Stage 2 Complete";
			}

			else if (raceState == StageThreeComplete)
			{
				debugText << "Race Complete";
				gameState = GameWon;
			}
		}

		if (gameState == GameWon)
		{
			gameStateText << "Status: GameWon";
			
		}
		
		/*Collision checks*/

		for (int i = 0; i < checkpointNo; i++)
		{
			if ((playerCarX > hitBoxMinX[i]) && (playerCarX < hitBoxMaxX[i])
				&& (playerCarY > hitBoxMinY[i]) && (playerCarY < hitBoxMaxY[i])
				&& (playerCarZ > hitBoxMinZ[i]) && (playerCarZ < hitBoxMaxZ[i]))
			{
				if (i == 0 && raceState == StageZeroComplete)
				{
					raceState = StageOneComplete;
					
				}
				
				else if (i == 1 && raceState == StageOneComplete)
				{
					raceState = StageTwoComplete;
					
				}
				
				else if (i == 2 && raceState == StageTwoComplete)
				{
					raceState = StageThreeComplete;
					
				}
			}
		}
		

		//Key controls for playerCar
		if (myEngine->KeyHeld(Key_W))
		{
			carModel->MoveLocalZ(movementSpeed * frameTime);
		}

		if (myEngine->KeyHeld(Key_S))
		{
			carModel->MoveLocalZ((movementSpeed/2) * frameTime * negativeDirection);
		}

		if (myEngine->KeyHeld(Key_A))
		{
			carModel->RotateLocalY(movementSpeed * frameTime * negativeDirection);
		}

		if (myEngine->KeyHeld(Key_D))
		{
			carModel->RotateLocalY(movementSpeed * frameTime);
		}


		//Key controls for testCamera
		if (myEngine->KeyHeld(Key_Up))
		{
			testCamera->MoveLocalZ(kCameraMovementSpeed * frameTime);
		}

		if (myEngine->KeyHeld(Key_Down))
		{
			testCamera->MoveLocalZ(kCameraMovementSpeed * negativeDirection * frameTime);
		}

		if (myEngine->KeyHeld(Key_Left))
		{
			testCamera->MoveLocalX(kCameraMovementSpeed * negativeDirection * frameTime);
		}

		if (myEngine->KeyHeld(Key_Right))
		{
			testCamera->MoveLocalX(kCameraMovementSpeed * frameTime);
		}

		

		if (myEngine->KeyHit(Key_Space))
		{
			
			if (gameState == Waiting)
			{
				timePassed = 0;

				gameState = Countdown;

				
			}
		}

		//Mouse controls for the camera
		int mouseMoveX = myEngine->GetMouseMovementX();
		testCamera->RotateLocalY(mouseMoveX * kMouseRotation * frameTime);

		int mouseMoveY = myEngine->GetMouseMovementY();
		testCamera->RotateLocalX(mouseMoveY * kMouseRotation * frameTime);

		
		/*debugText << "GameState is: " << gameState;*/
		font->Draw(debugText.str(), 0, 550, kBlack);
		
		
		font->Draw(gameStateText.str(), 0, 500, kBlack);
		
		if (myEngine->KeyHit(Key_1))
		{
			testCamera->SetLocalPosition(cameraLocalCoordinates.x, cameraLocalCoordinates.y, cameraLocalCoordinates.z);
			testCamera->ResetOrientation();
			testCamera->RotateLocalX(kRotation / 5);
		}

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}


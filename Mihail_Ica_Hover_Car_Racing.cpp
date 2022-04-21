// Mihail_Ica_Hover_Car_Racing.cpp: A program using the TL-Engine

/*Mihail Ica*/

#include <TL-Engine.h>
#include <sstream>
#include <iostream>
#include <vector>
							// TL-Engine include file and namespace
using namespace tle;

struct PosVector {
	float x;
	float y;
	float z;
};

struct BoundingBoxCoords {
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
};


int CheckpointCompletion(BoundingBoxCoords[], PosVector p, int objectsNo);

bool SphereToSphereCollision(PosVector car, float radius1, PosVector obj, float radius2);

bool sphereToBoxCollision(PosVector car, BoundingBoxCoords hitBoxes[], int objectsNo);

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder("./Media");

	/**** Set up your scene here ****/

	

	const int kCheckpointNo = 3;
	const int kIsleNo = 6;
	const int kWallNo = 3;
	const int kRotation = 90;
	const int kMovementSpeed = 50;
	const int kNegativeDirection = -1;
	const int kMouseRotation = 15;
	const int kCameraMovementSpeed = 5;
	const float kPlayerCarRadius = 6.46f;
	const float kCheckpointXSize = 9.86159f;
	const float kCheckpointYSize = 0.00210619f;
	const float kCheckpointZSize = 1.28539f;

	const float kWallXSize = 0.934082f;
	const float kWallYSize = 0.000674248f;
	const float kWallZSize = 4.83559f;

	const float kIsleXSize = 2.67529f;
	const float kIsleYSize = 0.0178144f;
	const float kIsleZSize = 3.41748f;

	const int kStrutsNo = 6;

	const float kMarginOfError = 0.5f;

	const float kCameraRotation = kRotation / 5;

	int checkpointIdentity[kCheckpointNo] = { 0,1,2 }; // Array holding the int values used to identify each checkpoint

	const PosVector checkpoint1Pos = { 0.0f,0.0f,0.0f };
	const PosVector checkpoint2Pos = { -20.0f,0.0f,120.0f };
	const PosVector checkpoint3Pos = { -25.0f,0.0f,56.0f };

	PosVector checkpointsArray[kCheckpointNo] = { checkpoint1Pos,checkpoint2Pos,checkpoint3Pos };

	const PosVector isle1 = { -10.0f,0.0f,40.0f };
	const PosVector isle2 = { 10.0f,0.0f,40.0f };
	const PosVector isle3 = { -10.0f,0.0f,56.0f };
	const PosVector isle4 = { 10.0f,0.0f,56.0f };
	const PosVector isle5 = { -8.0f,0.0f,130.0f };
	const PosVector isle6 = { 8.0f,0.0f,130.0f };

	PosVector islesArray[kIsleNo] = { isle1,isle2,isle3,isle4,isle5,isle6 };

	const PosVector wall1 = { -10.0f,0.0f,48.0f };
	const PosVector wall2 = { 10.0f,0.0f,48.0f };
	const PosVector wall3 = { 0.0f,0.0f,130.0f };

	PosVector wallsArray[kWallNo] = { wall1,wall2,wall3 };

	BoundingBoxCoords wallsBoundingBoxes[kWallNo];

	BoundingBoxCoords isleBoundingBoxes[kIsleNo];

	for (int i = 0; i < kWallNo; i++)
	{
		if(i == 0 || i == 1)
		wallsBoundingBoxes[i].minX = wallsArray[i].x - kPlayerCarRadius;
		wallsBoundingBoxes[i].maxX = wallsArray[i].x + kPlayerCarRadius;
		wallsBoundingBoxes[i].minY = wallsArray[i].y - kPlayerCarRadius;
		wallsBoundingBoxes[i].maxY = wallsArray[i].y + kPlayerCarRadius;
		wallsBoundingBoxes[i].minZ = wallsArray[i].z - kPlayerCarRadius;
		wallsBoundingBoxes[i].maxZ = wallsArray[i].z + kPlayerCarRadius;

		if (i == 2)
		{
		wallsBoundingBoxes[i].minX = wallsArray[i].x - kPlayerCarRadius;
		wallsBoundingBoxes[i].maxX = wallsArray[i].x + kPlayerCarRadius;
		wallsBoundingBoxes[i].minY = wallsArray[i].y - kPlayerCarRadius;
		wallsBoundingBoxes[i].maxY = wallsArray[i].y + kPlayerCarRadius;
		wallsBoundingBoxes[i].minZ = wallsArray[i].z - kPlayerCarRadius;
		wallsBoundingBoxes[i].maxZ = wallsArray[i].z + kPlayerCarRadius;
		}
	}

	for (int i = 0; i < kIsleNo; i++)
	{
		isleBoundingBoxes[i].minX = islesArray[i].x - kPlayerCarRadius;
		isleBoundingBoxes[i].maxX = islesArray[i].x + kPlayerCarRadius;
		isleBoundingBoxes[i].minY = islesArray[i].y - kPlayerCarRadius;
		isleBoundingBoxes[i].maxY = islesArray[i].y + kPlayerCarRadius;
		isleBoundingBoxes[i].minZ = islesArray[i].z - kPlayerCarRadius;
		isleBoundingBoxes[i].maxZ = islesArray[i].z + kPlayerCarRadius;

	}

	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	IModel* checkpointModels[kCheckpointNo];

	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	IModel* isleModels[kIsleNo];

	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IModel* wallModels[kWallNo];

	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	PosVector playerCar = { 0.0f,0.0f,-150.0f };

	IMesh* carMesh = myEngine->LoadMesh("hovercar.x");

	IModel* carModel = carMesh->CreateModel(playerCar.x, playerCar.y, playerCar.z);

	BoundingBoxCoords checkpointHitBox[kCheckpointNo];

	ICamera* testCamera = myEngine->CreateCamera(kTargeted);
	testCamera->AttachToParent(carModel);

	const PosVector cameraLocalCoordinates = { 0.0f,15.0f,-25.0f };

	testCamera->RotateLocalX(kCameraRotation);
	testCamera->SetLocalPosition(cameraLocalCoordinates.x, cameraLocalCoordinates.y, cameraLocalCoordinates.z);

	IMesh* groundMesh = myEngine->LoadMesh("ground.x");
	IModel* ground = groundMesh->CreateModel();

	ISprite* backdrop = myEngine->CreateSprite("RedGlow.jpg", 0, 500);
	IFont* font24 = myEngine->LoadFont("Sans Sherif");
	IFont* font30 = myEngine->LoadFont("Sans Sherif", 30);
	IFont* font36 = myEngine->LoadFont("Sans Sherif", 36);
	IFont* font42 = myEngine->LoadFont("Sans Sherif", 42);

	float hitBoxMinX[kCheckpointNo];
	float hitBoxMaxX[kCheckpointNo];
	float hitBoxMinY[kCheckpointNo];
	float hitBoxMaxY[kCheckpointNo];
	float hitBoxMinZ[kCheckpointNo];
	float hitBoxMaxZ[kCheckpointNo];


	enum GameStates { Waiting, Countdown, Racing, GameWon, GameLost };
	GameStates gameState = Waiting;

	enum RaceStates { StageZeroComplete, StageOneComplete, StageTwoComplete, StageThreeComplete };
	RaceStates raceState = StageZeroComplete;

	float movementSpeed = kMovementSpeed;

	
	for (int i = 0; i < kCheckpointNo; i++)
	{
		checkpointModels[i] = checkpointMesh->CreateModel(checkpointsArray[i].x, checkpointsArray[i].y, checkpointsArray[i].z);

		if (i == checkpointIdentity[1])
		{
			checkpointModels[i]->RotateLocalY(kRotation);
		}

		/*Checkpoint bounding volumes*/

		checkpointHitBox[i].minX = checkpointModels[i]->GetX() - kCheckpointXSize;
		checkpointHitBox[i].maxX = checkpointModels[i]->GetX()+ kCheckpointXSize;
		checkpointHitBox[i].minY = checkpointModels[i]->GetY() - kCheckpointYSize;
		checkpointHitBox[i].maxY = checkpointModels[i]->GetY() + kCheckpointYSize;
		checkpointHitBox[i].minZ = checkpointModels[i]->GetZ() - kCheckpointZSize;
		checkpointHitBox[i].maxZ = checkpointModels[i]->GetZ() + kCheckpointZSize;

	}

	for (int i = 0; i < kIsleNo; i++)
	{
		isleModels[i] = isleMesh->CreateModel(islesArray[i].x, islesArray[i].y, islesArray[i].z);


		if (i == 4 || i == 5)
		{
			isleModels[i]->RotateLocalY(kRotation);
		}
	}

	for (int i = 0; i < kWallNo; i++)
	{
		wallModels[i] = wallMesh->CreateModel(wallsArray[i].x, wallsArray[i].y, wallsArray[i].z);


		if (i == 2)
		{
			wallModels[i]->RotateLocalY(kRotation);
		}
	}

	float timePassed = 0.0f;

	bool collision;
	

	myEngine->Timer();
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		float frameTime = myEngine->Timer();

		/**** Update your scene each frame here ****/
		timePassed += frameTime;


		float oldPlayerCarX = carModel->GetX();
		float oldPlayerCarY = carModel->GetY();
		float oldPlayerCarZ = carModel->GetZ();

		float playerCarX = carModel->GetX();
		float playerCarY = carModel->GetY();
		float playerCarZ = carModel->GetZ();

		PosVector playerCarVector = { playerCarX, playerCarY, playerCarZ };

		stringstream gameStateText;
		stringstream timerText;
		stringstream debugText;
		stringstream debugText2;


		if (gameState == Waiting)
		{
			timerText << "Hit Space to Start";
			font24->Draw(timerText.str(), 0, 600, kBlack);

			gameStateText << "Status: Waiting ";
			movementSpeed = 0;

		}

		if (gameState == Countdown)
		{
			gameStateText << "Status: Countdown";


			if (timePassed < 1)
			{

				timerText << "3";
				font24->Draw(timerText.str(), 0, 600, kBlack);

			}

			if (timePassed < 2 && timePassed > 1)
			{

				timerText << "2";
				font30->Draw(timerText.str(), 0, 600, kBlack);
			}

			if (timePassed < 3 && timePassed > 2)
			{

				timerText << "1";
				font36->Draw(timerText.str(), 0, 600, kBlack);

			}

			if (timePassed < 4 && timePassed > 3)
			{
				timerText << "GO";
				font42->Draw(timerText.str(), 0, 600, kBlack);



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

			/*Collision checks*/


			/*Checkpoint Progression checks*/

			int checkPointCrossed = CheckpointCompletion(checkpointHitBox, playerCarVector, kCheckpointNo);

			if (checkPointCrossed == checkpointIdentity[0] && raceState == StageZeroComplete)
			{
				raceState = StageOneComplete;

			}

			else if (checkPointCrossed == checkpointIdentity[1] && raceState == StageOneComplete)
			{
				raceState = StageTwoComplete;

			}

			else if (checkPointCrossed == checkpointIdentity[2] && raceState == StageTwoComplete)
			{
				raceState = StageThreeComplete;

			}

			/* Wall and isles collision checks */

			

			if (sphereToBoxCollision(playerCarVector, isleBoundingBoxes, kIsleNo))
			{
				debugText << "Crashed!!!";
				carModel->SetPosition(oldPlayerCarX, oldPlayerCarY, oldPlayerCarZ);
			}


			if (sphereToBoxCollision(playerCarVector, wallsBoundingBoxes, kWallNo))
			{
				debugText << "Crashed!!!";
				carModel->SetPosition(oldPlayerCarX, oldPlayerCarY, oldPlayerCarZ);
				
			}

			/*if (SphereToSphereCollision(playerCarVector, kPlayerCarRadius,)*/

		}

		if (gameState == GameWon)
		{
			gameStateText << "Status: GameWon";

		}

		

		//Key controls for playerCar
		if (myEngine->KeyHeld(Key_W))
		{
			carModel->MoveLocalZ(movementSpeed * frameTime);
		}

		if (myEngine->KeyHeld(Key_S))
		{
			carModel->MoveLocalZ((movementSpeed/2) * frameTime * kNegativeDirection);
		}

		if (myEngine->KeyHeld(Key_A))
		{
			carModel->RotateLocalY(movementSpeed * frameTime * kNegativeDirection);
		}

		if (myEngine->KeyHeld(Key_D))
		{
			carModel->RotateLocalY(movementSpeed * frameTime);
		}

		if (myEngine->KeyHit(Key_Space))
		{

			if (gameState == Waiting)
			{
				timePassed = 0;

				gameState = Countdown;


			}
		}


		//Key controls for testCamera
		if (myEngine->KeyHeld(Key_Up))
		{
			testCamera->MoveLocalZ(kCameraMovementSpeed * frameTime);
		}

		if (myEngine->KeyHeld(Key_Down))
		{
			testCamera->MoveLocalZ(kCameraMovementSpeed * kNegativeDirection * frameTime);
		}

		if (myEngine->KeyHeld(Key_Left))
		{
			testCamera->MoveLocalX(kCameraMovementSpeed * kNegativeDirection * frameTime);
		}

		if (myEngine->KeyHeld(Key_Right))
		{
			testCamera->MoveLocalX(kCameraMovementSpeed * frameTime);
		}

		

		//Mouse controls for the camera
		int mouseMoveX = myEngine->GetMouseMovementX();
		testCamera->RotateLocalY(mouseMoveX * kMouseRotation * frameTime);

		int mouseMoveY = myEngine->GetMouseMovementY();
		testCamera->RotateLocalX(mouseMoveY * kMouseRotation * frameTime);

		font24->Draw(debugText.str(), 0, 650, kBlack);
		font24->Draw(gameStateText.str(), 0, 500, kBlack);
		
		if (myEngine->KeyHit(Key_1))
		{
			testCamera->SetLocalPosition(cameraLocalCoordinates.x, cameraLocalCoordinates.y, cameraLocalCoordinates.z);
			testCamera->ResetOrientation();
			testCamera->RotateLocalX(kCameraRotation);
		}

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

int CheckpointCompletion(BoundingBoxCoords hitBoxes[], PosVector p, int objectsNo)
{
	for (int i = 0; i < objectsNo; i++)
	{
		if ((p.x > hitBoxes[i].minX) && (p.x < hitBoxes[i].maxX)
			&& (p.y > hitBoxes[i].minY) && (p.y < hitBoxes[i].maxY)
			&& (p.z > hitBoxes[i].minZ) && (p.z < hitBoxes[i].maxZ))
		{
			return i;
		}
	}
}

bool SphereToSphereCollision(PosVector car, float radius1, PosVector obj, float radius2, int objectsNo)
{
	float distX = obj.x - car.x;
	float distY = obj.y - car.y;
	float distZ = obj.z - car.z;

	float distance = sqrt(distX * distX + distY * distY + distZ * distZ);
	bool collision;

	if (distance < (radius1 + radius2)) {
		collision = true;
	}
	else {
		collision = false;
	}

	return collision;
}

bool sphereToBoxCollision(PosVector car, BoundingBoxCoords hitBoxes[], int objectsNo)
{
	bool collision = false;

	for (int i = 0; i < objectsNo; i++)
	{
		if ((car.x > hitBoxes[i].minX) && (car.x < hitBoxes[i].maxX)
			&& (car.y > hitBoxes[i].minY) && (car.y < hitBoxes[i].maxY)
			&& (car.z > hitBoxes[i].minZ) && (car.z < hitBoxes[i].maxZ))
		{
			collision = true;
		}
	}

	return collision;

}


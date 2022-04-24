// Mihail_Ica_Hover_Car_Racing.cpp: A program using the TL-Engine

/*Mihail Ica*/

#include <TL-Engine.h>
#include <sstream>
#include <iostream>
#include <vector>
							// TL-Engine include file and namespace
using namespace tle;

struct PosVector {  //Structure used for holding the coordinates of any given object
	float x = 0;
	float y = 0;
	float z = 0;
};

struct BoundingBoxCoords {  //Structure used for holding bounding box coordinates
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;
	float minZ = 0;
	float maxZ = 0;
};

enum CollisionAxis { xAxis, zAxis, none };

int CheckpointCompletion(BoundingBoxCoords[], PosVector p, int objectsNo);

bool SphereToSphereCollision(PosVector car, float radius1, PosVector obj, float radius2);

CollisionAxis sphereToBoxCollision(PosVector car, BoundingBoxCoords hitBox);

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder("./Media");

	/**** Set up your scene here ****/

	
	// Number of objects
	const int kCheckpointNo = 5;
	const int kIsleNo = 18;
	const int kWallNo = 8;
	const int kTanksNo = 18;
	
	// Movement speed vars
	const int kRotation = 90;
	const int kMovementSpeed = 90;
	const int kNegativeDirection = -1;
	const int kMouseRotation = 15;
	const int kCameraMovementSpeed = 5;

	// Object dimensions (originally obtained from mesh files)
	const float kPlayerCarOriginal = 6.46f;
	const float kPlayerCarRadiusIsles = 6.46f/2;
	const float kPlayerCarRadius = 6.46f / 3;
	const float kCheckpointXSize = 9.86159f - 1;
	const float kCheckpointYSize = 0.00210619f;
	const float kCheckpointZSize = 1.28539f;

	const float kWallXSize = 0.934082f;
	const float kWallYSize = 0.000674248f;
	const float kWallZSize = 4.83559f;

	const float kIsleXSize = 2.67529f;
	const float kIsleYSize = 0.0178144f;
	const float kIsleZSize = 3.41748f;

	const float kTankDiameter = 1.5f;
	const int kTankHeight = 16;
	const float kToppledTankRadius = kTankDiameter * 4;

	const int kScale = 1;

	int playerCarHealth = 100;

	const float kMarginOfError = 0.5f; // Margin of error constant used for improving collisions and such processes

	const float kCameraRotation = kRotation / 5;

	const float strutSpace = 8.7f; // Distance of checkpoint struts from the checkpoint centre

	int checkpointIdentity[kCheckpointNo] = { 0,1,2,3,4 }; // Array holding the int values used to identify each checkpoint

	const PosVector checkpoint1Pos = { 0.0f,0.0f,0.0f };
	const PosVector checkpoint2Pos = { -20.0f,0.0f,120.0f };
	const PosVector checkpoint3Pos = { -25.0f,0.0f,56.0f };
	const PosVector checkpoint4Pos = { -80.0f,0.0f,150.0f };
	const PosVector checkpoint5Pos = { -150.0f,0.0f,120.0f };

	PosVector checkpointsArray[kCheckpointNo] = { checkpoint1Pos,checkpoint2Pos,checkpoint3Pos,checkpoint4Pos,checkpoint5Pos };

	const PosVector isle1 = { -10.0f,0.0f,40.0f };
	const PosVector isle2 = { 10.0f,0.0f,40.0f };
	const PosVector isle3 = { -10.0f,0.0f,56.0f };
	const PosVector isle4 = { 10.0f,0.0f,56.0f };
	const PosVector isle5 = { -8.0f,0.0f,130.0f };
	const PosVector isle6 = { 8.0f,0.0f,130.0f };
	const PosVector isle7 = { -17.0f,0.0f,40.0f };
	const PosVector isle8 = { -33.0f, 0.0f, 40.0f };
	const PosVector isle9 = { -40.0f, 0.0f, 56.0f };
	const PosVector isle10 = { -56.0f, 0.0f, 56.0f };
	const PosVector isle11 = { -55.0f,0.0f,158.5f };
	const PosVector isle12 = { -70.0f,0.0f,158.5f };
	const PosVector isle13 = { -100.0f, 0.0f, 700.0f };
	const PosVector isle14 = { 100.0f, 0.0f, 690.0f };
	const PosVector isle15 = { 600.0f,0.0f,1000.0f };
	const PosVector isle16 = { -42.0f,0.0f,125.0f };
	const PosVector isle17 = { -42.0f,0.0f,138.0f };
	const PosVector isle18 = { -42.0f,0.0f,112.0f };

	PosVector islesArray[kIsleNo] = { isle1,isle2,isle3,isle4,isle5,isle6,isle7,isle8,isle9,isle10,isle11,isle12,isle13,isle14,isle15,isle16,isle17,isle18 };

	const PosVector wall1 = { -10.0f,0.0f,48.0f };
	const PosVector wall2 = { 10.0f,0.0f,48.0f };
	const PosVector wall3 = { 0.0f,0.0f,130.0f };
	const PosVector wall4 = { -25.0f,0.0f,40.0f };
	const PosVector wall5 = { -48.0f,0.0f,56.0f };
	const PosVector wall6 = { -63.0f,0.0f,158.5f };
	const PosVector wall7 = { -42.0f, 0.0f, 132.0f };
	const PosVector wall8 = { -42.0f, 0.0f, 118.0f };

	PosVector wallsArray[kWallNo] = { wall1,wall2,wall3, wall4, wall5, wall6,wall7,wall8};

	const PosVector tank1 = { 70.0f,0.0f,160.0f };
	const PosVector tank2 = { 100.0f,0.0f,-100.0f };
	const PosVector tank3 = { -190.0f,0.0f,155.0f };
	const PosVector tank4 = { -150.0f,0.0f,-20.0f };
	const PosVector tank5 = { 30.0f,0.0f,120.0f };
	const PosVector tank6 = { 26.0f,0.0f,90.0f };
	const PosVector tank7 = { 20.0f,0.0f,69.0f };
	const PosVector tank8 = { 20.0f,0.0f,-15.0f };
	const PosVector tank9 = { 30.0f,0.0f,-40.0f };
	const PosVector tank10 = { -23.0f,0.0f,-8.0f };
	const PosVector tank11 = { -21.0f,0.0f,-33.0f };
	const PosVector tank12 = { -16.0f,0.0f,22.0f };
	const PosVector tank13 = { -42.0f,0.0f,86.0f };
	const PosVector tank14 = { -44.0f,0.0f,66.0f };
	const PosVector tank15 = { -43.0f,0.0f,152.0f };
	const PosVector tank16 = { -46.0f,0.0f,159.0f };
	const PosVector tank17 = { -29.0f,0.0f,138.0f };
	const PosVector tank18 = { 0.0f,-5.0f,90.0f };

	PosVector tanksArray[kTanksNo] = { tank1,tank2,tank3,tank4,tank5,tank6,tank7,tank8,tank9,tank10,tank11,tank12,tank13,tank14,
		tank15,tank16,tank17,tank18
	};

	BoundingBoxCoords wallsBoundingBoxes[kWallNo];

	BoundingBoxCoords isleBoundingBoxes[kIsleNo];

	BoundingBoxCoords tankBoundingBoxes[kTanksNo];

	//Bounding boxes for walls
	for (int i = 0; i < kWallNo; i++)
	{
		wallsBoundingBoxes[i].minX = wallsArray[i].x - kPlayerCarRadius - kMarginOfError;
		wallsBoundingBoxes[i].maxX = wallsArray[i].x + kPlayerCarRadius + kMarginOfError;
		wallsBoundingBoxes[i].minY = wallsArray[i].y - kPlayerCarRadius - kMarginOfError;
		wallsBoundingBoxes[i].maxY = wallsArray[i].y + kPlayerCarRadius + kMarginOfError;
		wallsBoundingBoxes[i].minZ = wallsArray[i].z - kPlayerCarRadius - kMarginOfError;
		wallsBoundingBoxes[i].maxZ = wallsArray[i].z + kPlayerCarRadius + kMarginOfError;

	}

	//Bounding boxes for isles
	for (int i = 0; i < kIsleNo; i++)
	{
		
		isleBoundingBoxes[i].minX = islesArray[i].x - kPlayerCarRadiusIsles - kMarginOfError;
		isleBoundingBoxes[i].maxX = islesArray[i].x + kPlayerCarRadiusIsles + kMarginOfError;
		isleBoundingBoxes[i].minY = islesArray[i].y - kPlayerCarRadiusIsles - kMarginOfError;
		isleBoundingBoxes[i].maxY = islesArray[i].y + kPlayerCarRadiusIsles + kMarginOfError;
		isleBoundingBoxes[i].minZ = islesArray[i].z - kPlayerCarRadiusIsles - kMarginOfError;
		isleBoundingBoxes[i].maxZ = islesArray[i].z + kPlayerCarRadiusIsles + kMarginOfError;

	}

	//Bounding boxes for tanks
	for (int i = 0; i < kTanksNo; i++)
	{
		
		tankBoundingBoxes[i].minX = tanksArray[i].x - kPlayerCarRadiusIsles;
		tankBoundingBoxes[i].maxX = tanksArray[i].x + kPlayerCarRadiusIsles;
		tankBoundingBoxes[i].minY = tanksArray[i].y - kPlayerCarRadiusIsles;
		tankBoundingBoxes[i].maxY = tanksArray[i].y + kPlayerCarRadiusIsles;
		tankBoundingBoxes[i].minZ = tanksArray[i].z - kPlayerCarRadiusIsles;
		tankBoundingBoxes[i].maxZ = tanksArray[i].z + kPlayerCarRadiusIsles;

	}


	//Mesh loading and model creation section
	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	IModel* checkpointModels[kCheckpointNo];

	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	IModel* isleModels[kIsleNo];

	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IModel* wallModels[kWallNo];

	IMesh* tankMesh = myEngine->LoadMesh("TankSmall2.x");
	IMesh* tankMesh2 = myEngine->LoadMesh("TankSmall1.x");
	IModel* tankModels[kTanksNo];
	IModel* tankModelsV2[kTanksNo];

	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	PosVector playerCar = { 0.0f,0.0f,-150.0f };

	IMesh* carMesh = myEngine->LoadMesh("hovercar.x");

	IModel* carModel = carMesh->CreateModel(playerCar.x, playerCar.y, playerCar.z);
	carModel->Scale(0.2f);

	BoundingBoxCoords checkpointHitBox[kCheckpointNo];

	ICamera* Camera = myEngine->CreateCamera(kTargeted);
	Camera->AttachToParent(carModel);

	const PosVector cameraLocalCoordinatesChase = { 0.0f,15.0f,-25.0f};
	const PosVector cameraLocalCoordinatesFPS = { 0.0f, 10.0f, 0.0f };

	Camera->RotateLocalX(kCameraRotation);
	Camera->SetLocalPosition(cameraLocalCoordinatesChase.x, cameraLocalCoordinatesChase.y, cameraLocalCoordinatesChase.z);

	IMesh* groundMesh = myEngine->LoadMesh("ground.x");

	IModel* ground = groundMesh->CreateModel();

	ISprite* backdrop = myEngine->CreateSprite("RedGlow.jpg", 0, 500);

	//Different sized fonts
	IFont* font24 = myEngine->LoadFont("Sans Sherif");
	IFont* font30 = myEngine->LoadFont("Sans Sherif", 30);
	IFont* font36 = myEngine->LoadFont("Sans Sherif", 36);
	IFont* font42 = myEngine->LoadFont("Sans Sherif", 42);

	//Game states used to track game status
	enum GameStates { Waiting, Countdown, Racing, GameWon, GameLost };
	GameStates gameState = Waiting;


	//Race states used to track race progression
	enum RaceStates { StageZeroComplete, StageOneComplete, StageTwoComplete, StageThreeComplete, StageFourComplete };
	RaceStates raceState = StageZeroComplete;

	enum CameraStates {ChaseCamera, FPSCamera};
	CameraStates cameraState = ChaseCamera;

	

	float movementSpeed = kMovementSpeed;

	
	for (int i = 0; i < kCheckpointNo; i++)
	{
		checkpointModels[i] = checkpointMesh->CreateModel(checkpointsArray[i].x, checkpointsArray[i].y, checkpointsArray[i].z);

		if (i == checkpointIdentity[1] || i == checkpointIdentity[3])
		{
			checkpointModels[i]->RotateLocalY(kRotation);
		}


		/*Checkpoint progression bounding volumes*/

		if (i == checkpointIdentity[1] || i == checkpointIdentity[3])
		{
			checkpointHitBox[i].minX = checkpointModels[i]->GetX() - kCheckpointZSize; // For the rotated checkpoint, the x and z coordinates are treated differently,
			checkpointHitBox[i].maxX = checkpointModels[i]->GetX() + kCheckpointZSize; // for example, the checkpoint's x size becomes z distance from the central point 
			checkpointHitBox[i].minY = checkpointModels[i]->GetY() - kCheckpointYSize;
			checkpointHitBox[i].maxY = checkpointModels[i]->GetY() + kCheckpointYSize;
			checkpointHitBox[i].minZ = checkpointModels[i]->GetZ() - kCheckpointXSize;
			checkpointHitBox[i].maxZ = checkpointModels[i]->GetZ() + kCheckpointXSize;
		}
		else
		{
			checkpointHitBox[i].minX = checkpointModels[i]->GetX() - kCheckpointXSize;
			checkpointHitBox[i].maxX = checkpointModels[i]->GetX() + kCheckpointXSize;
			checkpointHitBox[i].minY = checkpointModels[i]->GetY() - kCheckpointYSize;
			checkpointHitBox[i].maxY = checkpointModels[i]->GetY() + kCheckpointYSize;
			checkpointHitBox[i].minZ = checkpointModels[i]->GetZ() - kCheckpointZSize;
			checkpointHitBox[i].maxZ = checkpointModels[i]->GetZ() + kCheckpointZSize;
		}
		
	}

	for (int i = 0; i < kIsleNo; i++)
	{
		isleModels[i] = isleMesh->CreateModel(islesArray[i].x, islesArray[i].y, islesArray[i].z);


		if (i >= 4 && i < 15)
		{
			isleModels[i]->RotateLocalY(kRotation);
		}
		if (i >= 13 && i < 15)
		{
			isleModels[i]->Scale(50);
		}
	}

	for (int i = 0; i < kWallNo; i++)
	{
		wallModels[i] = wallMesh->CreateModel(wallsArray[i].x, wallsArray[i].y, wallsArray[i].z);


		if (i >= 2 && i < 6)
		{
			wallModels[i]->RotateLocalY(kRotation);
		}
	}

	for (int i = 0; i < kTanksNo; i++)
	{
		tankModels[i] = tankMesh->CreateModel(tanksArray[i].x, tanksArray[i].y, tanksArray[i].z);
		if (i == 17)
		{
			tankModels[i]->RotateLocalX(25);
		}
	}

	float timePassed = 0.0f;

	bool collision;

	float rightStrutX[kCheckpointNo];
	float leftStrutX[kCheckpointNo];

	for (int i = 0; i < kCheckpointNo; i++)
	{

		if (i == checkpointIdentity[1] || i == checkpointIdentity[3])
		{
			rightStrutX[i] = checkpointsArray[i].x;
			leftStrutX[i] = checkpointsArray[i].x;
		}

		else
		{
			rightStrutX[i] = checkpointsArray[i].x + strutSpace;
			leftStrutX[i] = checkpointsArray[i].x - strutSpace;
		}
		
		
		
	}

	PosVector rightStrut[kCheckpointNo];
	PosVector leftStrut[kCheckpointNo];

	for (int i = 0; i < kCheckpointNo; i++)
	{
		

		if (i == checkpointIdentity[1] || i == checkpointIdentity[3])
		{
			rightStrut[i] = { rightStrutX[i], 0.0f, (checkpointsArray[i].z + strutSpace)};
			leftStrut[i] = { leftStrutX[i], 0.0f, (checkpointsArray[i].z - strutSpace)};
		}

		else
		{
			rightStrut[i] = { rightStrutX[i], 0.0f, checkpointsArray[i].z };
			leftStrut[i] = { leftStrutX[i], 0.0f, checkpointsArray[i].z };
		}

	}

	PosVector momentum, oldMomentum, thrust, drag;

	const float speed = 0.2f;
	const float dragFactor = -0.0004f;

	float velocity = momentum.x * kScale;

	CollisionAxis checkAxis;

	bool boost = false;
	bool boostCooldown = false;
	
	bool boostAvailable = true;
	bool carFloats = true;

	float boostTimer = 0.0f;

	myEngine->Timer();
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		float frameTime = myEngine->Timer();

		/**** Update your scene each frame here ****/
		timePassed += frameTime;

		float matrix[4][4];
		carModel->GetMatrix(&matrix[0][0]);
		PosVector localZ = { matrix[2][0],matrix[2][1],matrix[2][2] };

		PosVector oldCarPos = { carModel->GetX(), carModel->GetY(), carModel->GetZ() };


		float playerCarX = carModel->GetX();
		float playerCarY = carModel->GetY();
		float playerCarZ = carModel->GetZ();

		PosVector playerCarVector = { playerCarX, playerCarY, playerCarZ };

		

		
		stringstream gameStateText;
		stringstream timerText;
		stringstream raceStage;
		stringstream carHealth;
		stringstream velocityDisplay;

		

		/* Game States */
		///////////////////////////////////

		drag = { oldMomentum.x * dragFactor, oldMomentum.y * dragFactor, oldMomentum.z * dragFactor };

		momentum = { thrust.x + oldMomentum.x + drag.x, thrust.y + oldMomentum.y + drag.y,
			thrust.z + oldMomentum.z + drag.z };



		carModel->Move(momentum.x* frameTime, momentum.y* frameTime, momentum.z* frameTime);


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
			carHealth << "Car HP" << playerCarHealth;
			velocityDisplay << velocity;

			font24->Draw(velocityDisplay.str(), 0, 450, kBlack);

			if (raceState == StageOneComplete)
			{
				raceStage << "Stage 1 Complete";
			}

			else if (raceState == StageTwoComplete)
			{
				raceStage << "Stage 2 Complete";
			}

			else if (raceState == StageThreeComplete)
			{
				raceStage << "Stage 3 Complete";

			}

			else if (raceState == StageFourComplete)
			{
				raceStage << "Stage 4 Complete";

			}

			if (boost && !boostCooldown && boostAvailable)
			{
				boostTimer += frameTime;

				if (boostTimer < 1)
				{

					timerText << "Boosters overheating in 3 seconds!";
					font24->Draw(timerText.str(), 0, 600, kBlack);

				}

				if (boostTimer < 2 && boostTimer > 1)
				{

					timerText << "Boosters overheating in 2 seconds!";
					font24->Draw(timerText.str(), 0, 600, kBlack);
				}

				if (boostTimer < 3 && boostTimer > 2)
				{

					timerText << "Boosters overheating in 1 second!";
					font24->Draw(timerText.str(), 0, 600, kBlack);

				}
				if (boostTimer > 3 && boostTimer < 4)
				{
					timerText << "Boosters overheated!";
					font24->Draw(timerText.str(), 0, 600, kBlack);
					boostCooldown = true;
					boost = false;
					timePassed = 0;
				}

			}

			if (timePassed < 5)
			{
				timerText << "Boosters overheated!";
			}

			if (timePassed > 5)
			{
				boostCooldown = false;

			}

			/* Collision checks */
			///////////////////////////////////


			/* Checkpoint Progression checks */

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

			else if (checkPointCrossed == checkpointIdentity[3] && raceState == StageThreeComplete)
			{
				raceState = StageFourComplete;

			}

			else if (checkPointCrossed == checkpointIdentity[4] && raceState == StageFourComplete)
			{
				gameState = GameWon;
			}


			/* Checkpoint strut collision checks */


			for (int i = 0; i < kCheckpointNo; i++)
			{
				if (SphereToSphereCollision(playerCarVector, kPlayerCarRadius, rightStrut[i], 1.25f))
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.z *= (-1.25f * 0.5f);
					momentum.x *= (-1.25f * 0.5f);
					carModel->Move(momentum.x* frameTime, momentum.y* frameTime, momentum.z* frameTime);
					playerCarHealth --;
					raceStage << "Crashed!!!";
					
				}

				if (SphereToSphereCollision(playerCarVector, kPlayerCarRadius, leftStrut[i], 1.25f))
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.z *= (-1.25f * 0.5f);
					momentum.x *= (-1.25f * 0.5f);
					carModel->Move(momentum.x* frameTime, momentum.y* frameTime, momentum.z* frameTime);
					playerCarHealth--;
					raceStage << "Crashed!!!";
					
				}
				
			}

			/* Tanks collision checks */

			for (int i = 0; i < kTanksNo; i++)
			{
				if (i == 17 && SphereToSphereCollision(playerCarVector, kPlayerCarRadius, tanksArray[i], kToppledTankRadius))
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.z *= (-1.25f * 0.5f);
					momentum.x *= (-1.25f * 0.5f);
					carModel->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime);
					playerCarHealth--;
					raceStage << "Crashed!!!";
					
				}

				if (SphereToSphereCollision(playerCarVector, kPlayerCarRadius, tanksArray[i], kTankDiameter))
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.z *= (-1.25f * 0.5f);
					momentum.x *= (-1.25f * 0.5f);
					carModel->Move(momentum.x* frameTime, momentum.y* frameTime, momentum.z* frameTime);
					playerCarHealth--;
					raceStage << "Crashed!!!";
					
				}
				
			}

			/* Isle collision checks */

			for (int i = 0; i < kIsleNo; i++)
			{
				checkAxis = sphereToBoxCollision(oldCarPos, isleBoundingBoxes[i]);

				
				if (checkAxis == xAxis)
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.z *= (-1.0f * 0.5f);
					carModel->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime);
					playerCarHealth --;
					raceStage << "Crashed!!!";
				}

				else if (checkAxis == zAxis)
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.x *= (-1.0f * 0.5f);
					carModel->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime);
					playerCarHealth --;
					raceStage << "Crashed!!!";
				}
				
				
			}

			

			/* Wall collision checks */

			for (int i = 0; i < kWallNo; i++)
			{
				checkAxis = sphereToBoxCollision(oldCarPos, wallsBoundingBoxes[i]);

				
				if (checkAxis == xAxis)
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.z *= (-1.0f * 0.5f);
					carModel->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime);
					playerCarHealth -= 1;
					raceStage << "Crashed!!!";
				}

				else if (checkAxis == zAxis)
				{
					carModel->SetPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					momentum.x *= (-1.0f * 0.5f);
					carModel->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime);
					playerCarHealth -= 1;
					raceStage << "Crashed!!!";
				}
				
				
			}
			
			/* Car health Mechanics */

			if (carFloats)
			{

				/*if (carModel->GetY() < 0.5)
				{
					carModel->MoveY(0.05 * frameTime);
					
				}

				else if (carModel->GetY() > 0.5 && carModel->GetY() < 0.1)
				{
					carModel->MoveY(-0.15 * frameTime);
					
				}*/
				
			}

			if (playerCarHealth <= 30)
			{
				boostAvailable = false;
				carFloats = false;
			}

			if (playerCarHealth <= 0)
			{
				gameState = GameLost;
			}
			
		}

		if (gameState == GameWon)
		{
			gameStateText << "Status: GameWon";
			
		}

		if (gameState == GameLost)
		{
			gameStateText << "Status: GameLost";
		}


		//Key controls for playerCar
		if (myEngine->KeyHeld(Key_W) && gameState == Racing)
		{
			if (boost)
			{
				thrust = { (localZ.x * speed) * 1.5f, (localZ.y * speed) * 1.5f, (localZ.z * speed) * 1.5f };
			}

			else
			{
				thrust = { localZ.x * speed, localZ.y * speed, localZ.z * speed };
			}
			
		}

		else if (myEngine->KeyHeld(Key_S) && gameState == Racing)
		{
			if (boost)
			{
				thrust = { (localZ.x * -speed) * 1.5f, (localZ.y * -speed) * 1.5f, (localZ.z * -speed) * 1.5f };
			}

			else
			{
				thrust = { localZ.x * -speed, localZ.y * -speed, localZ.z * -speed };
			}
			
		}

		else 
		{
			thrust = { 0,0,0 };
		}

		if (myEngine->KeyHeld(Key_A) && gameState == Racing)
		{
			carModel->RotateLocalY(movementSpeed * frameTime * kNegativeDirection);
			
		}

		if (myEngine->KeyHeld(Key_D) && gameState == Racing)
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

		if (myEngine->KeyHeld(Key_Space) && gameState == Racing && !boostCooldown && boostAvailable)
		{
			boost = true;
			
		}

		
		
		oldMomentum = momentum;

		//Key controls for Camera
		

		if (cameraState == ChaseCamera)
		{
			if (myEngine->KeyHit(Key_1))
			{
				Camera->SetLocalPosition(cameraLocalCoordinatesChase.x, cameraLocalCoordinatesChase.y, cameraLocalCoordinatesChase.z);
				Camera->ResetOrientation();
				Camera->RotateLocalX(kCameraRotation);
			}

			if (myEngine->KeyHit(Key_2))
			{
				Camera->SetLocalPosition(cameraLocalCoordinatesFPS.x, cameraLocalCoordinatesFPS.y, cameraLocalCoordinatesFPS.z);
				Camera->ResetOrientation();
				cameraState = FPSCamera;
			}

			if (myEngine->KeyHeld(Key_Up))
			{
				Camera->MoveLocalZ(kCameraMovementSpeed * frameTime);
			}

			if (myEngine->KeyHeld(Key_Down))
			{
				Camera->MoveLocalZ(kCameraMovementSpeed * kNegativeDirection * frameTime);
			}

			if (myEngine->KeyHeld(Key_Left))
			{
				Camera->MoveLocalX(kCameraMovementSpeed * kNegativeDirection * frameTime);
			}

			if (myEngine->KeyHeld(Key_Right))
			{
				Camera->MoveLocalX(kCameraMovementSpeed * frameTime);
			}

		}

		if (cameraState == FPSCamera)
		{
			if (myEngine->KeyHit(Key_1))
			{
				Camera->SetLocalPosition(cameraLocalCoordinatesChase.x, cameraLocalCoordinatesChase.y, cameraLocalCoordinatesChase.z);
				Camera->ResetOrientation();
				Camera->RotateLocalX(kCameraRotation);
				cameraState = ChaseCamera;
			}
		}

		
		//Mouse controls for the camera
		int mouseMoveX = myEngine->GetMouseMovementX();
		Camera->RotateLocalY(mouseMoveX * kMouseRotation * frameTime);

		int mouseMoveY = myEngine->GetMouseMovementY();
		Camera->RotateLocalX(mouseMoveY * kMouseRotation * frameTime);

		font24->Draw(raceStage.str(), 0, 650, kBlack);
		font24->Draw(gameStateText.str(), 0, 500, kBlack);
		font24->Draw(carHealth.str(), 0, 700, kBlack);
		
		

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

int CheckpointCompletion(BoundingBoxCoords hitBoxes[], PosVector p, int objectsNo) // Checkpoint completion function that gets the bounding box coordinates in structure arrays,
																				   // point coordinates of an object and number of objects that are to be checked for collisions

{
	for (int i = 0; i < objectsNo; i++)
	{
		if ((p.x > hitBoxes[i].minX) && (p.x < hitBoxes[i].maxX)
			&& (p.y > hitBoxes[i].minY) && (p.y < hitBoxes[i].maxY)
			&& (p.z > hitBoxes[i].minZ) && (p.z < hitBoxes[i].maxZ))
		{
			return i; // Function returns i as the identifier of which object the collision has occured with, solely for checkpoint progression tracking
		}
	}
}

bool SphereToSphereCollision(PosVector car, float radius1, PosVector obj, float radius2) // Sphere to sphere collision function used for checkpoint strut collision
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

	return collision; // Boolean is returned as true, should collision occur and false if no collision
}

CollisionAxis sphereToBoxCollision(PosVector car, BoundingBoxCoords hitBox) // Similar to CheckpointCompletion function, but adapted for use with walls, isles and other objects
{
	
		if ((car.x > hitBox.minX) && (car.x < hitBox.maxX)
			&& (car.y > hitBox.minY) && (car.y < hitBox.maxY)
			&& (car.z > hitBox.minZ) && (car.z < hitBox.maxZ))
		{
			if (car.x > hitBox.minX && car.x < hitBox.maxX)
			{
				return xAxis;
			}

			else if (car.z > hitBox.minZ && car.z < hitBox.maxZ)
			{
				return zAxis;
			}
		}
	

	return none;

}




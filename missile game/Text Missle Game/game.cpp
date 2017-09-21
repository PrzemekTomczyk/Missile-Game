// Author: Przemyslaw Tomczyk
// ID: C00218004
// Summary: Small text based game to fire missiles at an enemy

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

/// <summary>
/// menu: done
/// missile selection: done
/// invalid userInput catching: done
/// target/coordinates: in progress
/// launch code: next
/// arm+fire: to be done
/// exit: done
/// </summary>

// constant ints for menu just incase it ever gets larger
const int SMALLEST_MENU = 1;
const int HIGHEST_MENU = 6;

// enum for the warhead types
enum WarHead { Explosive, Nuclear};

// typedef struct for position, this is used for both rocket crash coords and enemy location
typedef struct Position
{
	// x.y coordinates
	int x = 0;
	int y = 0;

	// function to print out these coordinates
	void printCoords()
	{
		std::cout << "X coordinate: " << x << ", " << "Y coordinate: " << y << std::endl;
	}
}Coordinates;

/// <summary>
/// data structure that holds enemy's coordinates
/// </summary>
typedef struct Enemy
{
	Coordinates coordinates;
}Target;

/// <summary>
/// defined struct for warhead radius used when checking for collision
/// </summary>
typedef struct WarheadSize
{
	// explosive radius
	const int EXPLO_SIZE = 10;
	// nuke radius 
	const int NUKE_SIZE = 5;
}Size;

struct Missile
{
	// amount of missiles user has to work with
	int missileCount = 10;
	// type of missile
	WarHead payload;
	// target for the missile
	Coordinates coordinates;
	// actual location of the target
	Target target;
	// warhead sizes
	Size size;
	// bool to check if enemy is ded
	bool enemyNuked = false;
	// bool to check if warhead type has been selected
	bool warheadSelected = false;
	// bool to check if enemy has x/y made already
	bool enemyMade = false;
	// bool to check if target has been entered
	bool targetEntered = false;
	// warhead armed?
	bool armed = false;
	// bool to check if launch code has been made already
	bool codeMade = false;
	// bool to check if launch code has already been entered
	bool launchConfirmed = false;
	// 2 display variables that are enemy's x/y but with +/-25 difference
	int enemyDisplayX = 0;
	int enemyDisplayY = 0;
	// variable to store launch code
	int launchCode = 0;
	// int to hold user entered code for confirmation
	int userCode = 0;

	// function to confirm and make launch code
	void launchConfirmation()
	{
		// check if codes hasnt been made already
		if (!codeMade)
		{
			// generate a 4 digit launch code
			launchCode = rand() % 8999 + 1000;
			// flip the bool
			codeMade = true;
		}
		// check if code hasnt been confirmed already
		if (!launchConfirmed)
		{
			// display the random launch code
			std::cout << "####################" << std::endl;
			std::cout << "Code: " << launchCode << std::endl;
			std::cout << "####################" << std::endl;

			// output asking user to input launch code displayed above
			std::cout << "Please confirm launch sequence by re-entering the launch code..." << std::endl;
			std::cin >> userCode;

			// check if launch code has been entered correctly
			if (userCode == launchCode)
			{
				// clear function that clears console/cout stream
				clear();
				std::cout << "Launch code entered correctly." << std::endl;
				// set bool to true for menu checking
				launchConfirmed = true;
			}
			else
			{
				// clear function that clears console/cout stream
				clear();
				// clear error flag as 'menuInput' has been reset
				std::cin.clear();
				// ignore X new lines so it doesn't take trash that is on the same line
				std::cin.ignore(100000, '\n');

				std::cout << "Launch code invalid!" << std::endl;
			}
		}
		// if code has been entered correctly already, output this
		else
		{
			std::cout << "Launch code already confirmed!" << std::endl;
		}
	}



	/// <summary>
	/// arm function that flips the switch from true to false and vice versa
	/// </summary>
	void arm()
	{
		// if 'true' then disarm
		if (armed)
		{
			armed = false;
			std::cout << "Rocket disarmed." << std::endl;
		}
		// if 'false' then arm
		else
		{
			armed = true;
			std::cout << "Rocket armed!" << std::endl;
		}
	}

	/// <summary>
	/// function that generates enemy's coordinates
	/// </summary>
	void generateEnemy()
	{
		// generate location coordinates for the enemy
		target.coordinates.x = rand() % 200 + 100;
		target.coordinates.y = rand() % 200 + 100;

		// assign enemy's location and +/- 25 to it's actual location
		enemyDisplayX = target.coordinates.x + (rand() % 51 + (-25));
		enemyDisplayY = target.coordinates.y + (rand() % 51 + (-25));

		// enemy has its x/y assigned so enemyMade is now 'true'
		enemyMade = true;
	}

	/// <summary>
	/// function for enemy location output ONLY
	/// </summary>
	void enemyLocationOutput()
	{
		std::cout << "Enemy is located somewhere around " << "x: " << enemyDisplayX << ", y: " << enemyDisplayY << " +/-25 respectively" << std::endl;
	}

	/// <summary>
	/// asks user to choose a missile type
	/// </summary>
	void missileType()
	{
		// bool used to check for invalid input
		bool validInput = false;
		// keep looping until input is valid (true)
		while (!validInput)
		{
			// string used to get input from the user
			// string is used because the whole input is then converted to lowercase
			std::string userInput = "";
			std::cout << "Choose missile type (explosive/nuclear): ";
			std::cin >> userInput;

			// convert every character of input to lowercase
			for (int i = 0; i < userInput.length(); i++)
			{
				// function 'tolower' to convert string 'userInput' into lower case letters
				userInput[i] = tolower(userInput[i]);
			}
			 // clear function that clears console/cout stream
			clear();
			// if user entered explosive then set payload to explosive, flip validInput and warheadSelected to true
			if (userInput == "explosive")
			{
				payload = Explosive;
				validInput = true;
				std::cout << "Explosive missile has been selected." << std::endl;
				warheadSelected = true;
			}
			// works same as above if user entered nuclear
			else if (userInput == "nuclear")
			{
				payload = Nuclear;
				validInput = true;
				std::cout << "Nuclear missile has been selected." << std::endl;
				warheadSelected = true;
			}
			// if user enetered anything else then its invalid input and throw error messsage
			else
			{
				std::cout << "Invalid missile type!" << std::endl << std::flush;
				// clear console window and pause
				clearPause();
				warheadSelected = false;
			}

		}
	}

	/// <summary>
	/// funtion to check if rocket has hit the enemy
	/// </summary>
	void checkCollision()
	{
		// variables that hold parts of the distance formula
		int lengthDif = (target.coordinates.x - coordinates.x) * (target.coordinates.x - coordinates.x);
		int heightDif = (target.coordinates.y - coordinates.y) * (target.coordinates.y - coordinates.y);
		int sqrTotal = lengthDif + heightDif;
		// this variable has the end result of the distance formula
		int distance = sqrt(sqrTotal);

		// these nested if statements check if payload is explosive, if not then it HAS to be nuclear
		if (payload == Explosive)
		{
			// check if distance is smaller than the radius, if so then it collided
			if (size.EXPLO_SIZE > distance)
			{
				std::cout << "Target has been hit by the Explosive WarHead!\n Distance between Enemy base and centre of explosion was " << distance << " units squared." << std::endl;
			}
			// if above is false then it didn't collide
			else
			{
				std::cout << "Explosive missile was short by " << distance - size.EXPLO_SIZE << " units squared." << std::endl;
			}
		}
		// if above statement was false then it HAS to be nuclear
		else
		{
			if (size.NUKE_SIZE > distance)
			{
				std::cout << "Target has been hit by the Nuclear WarHead! Enemy is ded. " << std::endl;
				// variable flipped to true because enemy is no more
				enemyNuked = true;
			}
			else
			{
				std::cout << "Nuclear missile was short by " << distance - size.NUKE_SIZE << " units squared." << std::endl;
			}
		}
	}
};

// function to display the menu window
void menuDisplay(int &menuInput);
// function to clear and then pause the game
void clearPause();
// function to clear the window without pausing
void clear();

///########################################################### MAIN
///########################################################### MAIN
///########################################################### MAIN

int main()
{
	// seed for the random function
	srand(time(NULL));

	// generated launch code
	//int launchCode = rand() % 1000 + 1;
	// menuInput set to 0 so it goes right to it
	int menuInput = 0;
	// game is not over
	bool gameover = false;

	// create missile
	Missile rocket;
	
	// run while gameover is not true
	while (!gameover)
	{
		// if missile count is less than 1 then gameover
		if (rocket.missileCount < 1)
		{
			clear();
			std::cout << "You're out of missiles! GAME OVER" << std::endl;
			std::cout << "Game will now exit..." << std::endl;
			gameover = true;
		}
		// if player is not out of missiles then go to game
		else
		{
			// switch statement for menu
			switch (menuInput)
			{
			case 0:
				menuDisplay(menuInput);
				break;
			case 1:
				// clear screen before new output
				clear();
				std::cout << "Select a missile type..." << std::endl;
				std::cout << "Explosive = bigger radius, Nuclear = smaller radius but lethal" << std::endl;
				rocket.missileType();
				clearPause();
				menuInput = 0;
				break;
			case 2:
				// clear screen before new output
				clear();

				if (rocket.warheadSelected)
				{
					std::cout << "Enter target & info..." << std::endl;
					// check if enemy has been generated, if here for the first time then make it otherwise no so coordinates dont change
					if (rocket.enemyMade == false)
					{
						rocket.generateEnemy();
					}
					// displays enemy x/y with the 25 unit difference
					rocket.enemyLocationOutput();

					std::cout << "Player is aiming at..." << std::endl;
					rocket.coordinates.printCoords();

					std::cout << "Enter new target coordinates..." << std::endl;
					std::cout << "X: ";
					std::cin >> rocket.coordinates.x;
					std::cout << "Y: ";
					std::cin >> rocket.coordinates.y;
					rocket.targetEntered = true;

				}
				else
				{
					std::cout << "Warhead has not been selected!" << std::endl;
				}
				// clear screen and pause
				clear();
				// reset input so it goes back to the menu
				menuInput = 0;
				break;
			case 3:
				clear();
				if (rocket.targetEntered)
				{
					// clear screen before new output
					std::cout << "Launch code confirmation..." << std::endl;

					// use function to make the launch code and ask for confirmation
					rocket.launchConfirmation();

				}
				else
				{
					std::cout << "Target has not been entered yet!" << std::endl;
				}
				clearPause();
				menuInput = 0;
				break;
			case 4:
				if (rocket.launchConfirmed)
				{
					// clear screen before new output
					clear();
					std::cout << "Arm warhead..." << std::endl;
					rocket.arm();
				}
				else
				{
					clear();
					std::cout << "Launch code has not been confirmed yet!" << std::endl;
				}
				clearPause();
				menuInput = 0;
				break;
			case 5:
				if (rocket.armed)
				{
					clear();
					rocket.missileCount--;
					rocket.checkCollision();
					if (rocket.enemyNuked)
					{
						std::cout << "You have NUKED the enemy! Good Job Mr. T, you can go back to your tweeting ;)" << std::endl;
						std::cout << "Game will now exit..." << std::endl;
						gameover = true;
					}
				}				
				else
				{
					clear();
					std::cout << "Warhead cannot be fired because its not armed!" << std::endl;
				}
				
				clearPause();
				menuInput = 0;
				break;
			case 6:
				clear();
				gameover = true;
				std::cout << "Quitting game..." << std::endl;
				break;
			default:
				break;
			}
		}
	}

	system("pause");
	std::cout << std::endl;
	return 0;
}
///########################################################### MAIN END
///########################################################### MAIN END
///########################################################### MAIN END

void menuDisplay(int &menuInput)
{
//	int menuInput = 0;
	while (menuInput < SMALLEST_MENU || menuInput > HIGHEST_MENU)
	{
		std::cout << "1. Select missile type\n2. Input target coordinates\n3. Launch code confirmation\n4. Arm warhead\n5. Fire warhead\n6. Exit" << std::endl;
		std::cout << "Pick a number from the menu: ";	
		
		std::cin >> menuInput;

		// set variable back to 0 incase input wasn't a number
		if (menuInput < SMALLEST_MENU || menuInput > HIGHEST_MENU)
		{
			menuInput = 0;
			// clear error flag as 'menuInput' has been reset
			std::cin.clear();
			// ignore X new lines so it doesn't take trash that is on the same line
			std::cin.ignore(100000, '\n');
			std::cout << "Invalid number entered!" << std::endl << std::flush;
			clearPause();
		}
	}
}

void clearPause()
{
	system("pause");
	std::cout << std::flush;
	system("cls");
}

void clear()
{
	std::cout << std::flush;
	system("cls");
}

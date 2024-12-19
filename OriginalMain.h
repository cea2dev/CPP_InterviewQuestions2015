/////////////////////////////////////////////////////////////////////////////////////////
//																						
//	The following is the ----- programming test. You have 2 hours
//	to complete the test. You may use any resources you wish including a compiler or IDE.		
//	Please make sure that your solutions compile without errors in the compiler of		
//	your choice. We also suggest that you test all or your answers. You may include 
//	any additional headers you wish from the C++ standard library.							
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <cmath>

// Additional includes here. You may only include headers from the standard library.


//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 1: below is a function that (assuming a bullet impact) is intended to determine 
// if a character was shot from in front or behind. Write the contents of the function. 
// Do not change the function signature. Use comments to explain your reasoning.
//
//////////////////////////////////////////////////////////////////////////////////////////
struct Vec3D
{
	float x,y,z;
};

bool WasShotFromBehind(const Vec3D& bulletVelocity, const Vec3D& CharacterFacing)
{

	return false;
}



//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 2: Write a template function below which returns the size (in bytes) of 
// its template type without using the sizeof() operator. Use comments to explain your 
// reasoning.
//
//////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 3: Write a function that reverses only the portion of a linked list that comes after 
// the element "TestElement" in the list, starting with that element. For example, given the initial 
// list "A-B-C-D-E-F-G" and TestElement of "D", the resulting list would be "A-B-C-G-F-E-D". Your 
// function should be as robust as possible making no assumptions about the validity of the initial list. 
// Use comments to explain your reasoning. Your function should return the head of the resulting list.
// Do not change the ListNode class.
//
//////////////////////////////////////////////////////////////////////////////////////////

struct ListNode
{
	ListNode* Next;
};

ListNode* ReverseAfterTestElement(ListNode* Head, const ListNode* TestElement)
{

	return Head;
}



//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 4: The code below is required to find all the COMMON assets between two levels. 
// The passed in asset lists may contain duplicate values, but the result must contain NO 
// duplicates. You can assume two assets are equivalent if and only if they are located at 
// the same memory address. Implement the FindCommonAssets function as optimally as possible, 
// given levels with thousands of assets and (potentially) many duplicate entries. The result
// should contain a single unique reference to every asset that is contained in BOTH level lists.
//
// EXAMPLE
// Level1 = 3,4,1,2,3,5,6
// Level2 = 4,6,8,2,3,4,9
// Out = 2,3,4,6
//
// Do not change the FindCommonAssets function signature. Use comments to explain your reasoning.
//
//////////////////////////////////////////////////////////////////////////////////////////

class Asset
{
	// ...
};

void FindCommonAssets
(
 const std::vector<Asset*>& LevelOneAssets, 
 const std::vector<Asset*>& LevelTwoAssets, 
 std::vector<Asset*>& outUniqueCommonAssets
 )
{

}


//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 5 - A departing programmer has written the following code that you will be expected
// to maintain. Fix all the potential bugs and programming errors, and refactor any bad practices
// that you can find in the code below. You can do anything you wish to the code, including
// adding new classes and functions, as long as you preserve the intended functionality.
// Use comments to explain your reasoning.
//
//////////////////////////////////////////////////////////////////////////////////////////

// Abstract player class
class Player
{
public:
	Vec3D Location;
	// Holds info about the Joystick Direction last pressed by the player
	Vec3D JoystickDirection;
	// True if this player is an AI
	bool bIsAI;

	// Announce this player's entry into the match
	Player()
	{
		AnnounceEntry();
	}

	// Current max move speed
	float GetMaxMoveSpeed() { return 0.0f; }

	Vec3D GetDesiredMoveDirection() 
	{ 
		// Dummy variable because the compiler is complaining
		Vec3D dummy;
		return dummy; 
	}

	// Update the player's position
	void Tick(float DeltaTime)
	{
		// Move at max speed in the desired direction
		Vec3D MoveVec = GetDesiredMoveDirection();
		MoveVec.x *= GetMaxMoveSpeed();
		MoveVec.y *= GetMaxMoveSpeed();
		MoveVec.z *= GetMaxMoveSpeed();

		Location.x += MoveVec.x;
		Location.y += MoveVec.y;
		Location.z += MoveVec.z;
	}

	void AnnounceEntry()
	{
		if (bIsAI)
		{
			printf("AI has entered the match");
		}
		else
		{
			printf("Player has entered the match");
		}
	}
};

class HumanPlayer : public Player
{
public:
	HumanPlayer() { bIsAI = false; }

	// This is the total weight carried by this player in his inventory
	float CarryWeight;

	// Inventory items will slow down the player's max move speed
	float GetMaxMoveSpeed()
	{
		return 100.0f - CarryWeight;
	}

	Vec3D GetDesiredMoveDirection() { return JoystickDirection; }
};

class AIPlayer : public Player
{
public:
	Player* CurrentTarget;

	AIPlayer() { bIsAI = true; }

	float GetMaxMoveSpeed()
	{
		return 100.0f;
	}

	Vec3D GetDesiredMoveDirection()
	{
		Vec3D MoveDir;
		MoveDir.x = Location.x - CurrentTarget->Location.x;
		MoveDir.y = Location.y - CurrentTarget->Location.y;
		MoveDir.z = Location.z - CurrentTarget->Location.z;
		return MoveDir;
	}

};

//////////////////////////////////////////////////////////////////////////////////////////
//
// You may implement test cases for the functions here if you like.
//
//////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{

	return 0;
}
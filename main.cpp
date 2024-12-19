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
#include <algorithm>

// Additional includes here. You may only include headers from the standard library.

//for assert
#include <assert.h>

//printf
#include <stdio.h>

#include <unordered_set>

//my little Timer class
//to print out algorithm run times
#include "Timer.h"


//time()
#include <time.h>

//old school C I/O, short and sweet
//popping warnings from compilers
FILE* pFileProblem_1      = 0;
FILE* pFileProblem_2      = 0;
FILE* pFileProblem_3      = 0;
FILE* pFileProblem_4      = 0;


#pragma warning(push)

//I use fopen/fclose, old C I/O
//because its quick to set up, 
//MS wants is complaining about using safer versions
#pragma warning (disable : 4996)

//Below I use enums as return codes for some functions
//putting each enum in a namespace would resolve this warning
//but it's not an issue here
#pragma warning (disable : 4482)

//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 1: below is a function that (assuming a bullet impact) is intended to determine 
// if a character was shot from in front or behind. Write the contents of the function. 
// Do not change the function signature. Use comments to explain your reasoning.
//
//////////////////////////////////////////////////////////////////////////////////////////

/*
	Vec3D, 3 floats, sizeof(Vect3D) = 12 bytes
	Changes Made:
	Original struct only declared vars x,y,z
	Would likely initialize to junk

	Add Big 4: Default Ctor, Copy Ctor, Dtor, Assigment Op
	+ Overloaded ctor

	Vectors have lots of operations, so additional vector operations
	were added as they were needed throughout rest of the test
*/

struct Vec3D
{
	//public variables x, y, z
	float x,y,z;

	//default construction is a "zero" vector
	Vec3D()
		:x(0.0f), y(0.0f), z(0.0f)
	{
	}

	//special ctor
	Vec3D(const float _x, const float _y, const float _z)
		:x(_x), y(_y), z(_z)
	{
	}

	//copy
	Vec3D(const Vec3D& rhs)
		:x(rhs.x), y(rhs.y), z(rhs.z)
	{
	}

	//assignment
	Vec3D operator=(const Vec3D& rhs)
	{
		//check for self assignmet
		if(this != &rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
		}

		return *this;
	}
	
	//dtor
	//empty, nothing to explicitly free in this struct
	~Vec3D()
	{
	}

	const Vec3D Norm() const
	{
		const float len = this->Length();

		//0.00001f will be the "tolerance" here
		//since floating-point equality with == is unreliable

		//can't divide by 0, undefined behavior
		//so just crash here on assert
		assert(len > 0.00001f);

		//normailzer,
		//calculate it once
		//use multiplication and only one divide
		//to save cpu cycles
		const float normalizer = 1.0f / len;

		//RVO
		return Vec3D(this->x * normalizer, this->y * normalizer, this->y * normalizer);
	}

	const float Length() const
	{
		return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	const Vec3D operator-(const Vec3D& rhs) const
	{
		return Vec3D(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}

	//dot product
	static const float Dot(const Vec3D& v, const Vec3D& w)
	{
		return (v.x * w.x + v.y * w.y + v.z * w.z);
	}
};

//Problem One
//Changes made
/*
	This function is very dependent upon dot product, the component-wise product of
	2D or 3D Vectors.

	Function signature remains unchanged as specified

	Vectors on the same side of the plane have a dot product > 0 and an angle between them
	smaller than 90 degrees

	Vectors on the opposite side of the plane have a dot product < 0 and an angle between them
	greater than 90 degrees

	Vectors that are perpendicular to one another have a dot product = 0 and an angle between them
	= 90 degrees

	If the dot was precisely 0, that means the player was shot spot on on either side,
	If you want that to count as shot from behind is debatable, this only considers shots
	with postive dot products

	Note we don't need to normalize vectors here (Dot(v, w) = |v||w|cos(ang)), 
	we just care about the sign

*/
bool WasShotFromBehind(const Vec3D& bulletVelocity, const Vec3D& CharacterFacing)
{
	return Vec3D::Dot(bulletVelocity, CharacterFacing) > 0.0f;
}



//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 2: Write a template function below which returns the size (in bytes) of 
// its template type without using the sizeof() operator. Use comments to explain your 
// reasoning.
//
//////////////////////////////////////////////////////////////////////////////////////////


/*
	Changes Made:
	Function template argument T is passed by const reference

	Why?
	T could be huge, copy construction expensive
	Remove uneccesary copy ctor/dtor call

	How It Works:
		Say function template is compiled with Vect3D (12 bytes) 
		Say &rhs = 0x18 (24)

		Pointer Arithmetic: ptr + someNumber = (address ptr points to) + (someNumber * sizeof(type specified by pointer))

		In this case: sizeof(Vect3D) = 12 bytes, so (Vec3D*) &rhs (0x18)  + (1 * 12 bytes) = 0x24 (36)

		36 - 24 = 12 = sizeof(Vec3D)!

	Why cast to char* ?
		sizeof(char) = 1 byte

		Just (&rhs + 1)  - (&rhs) would return 1, because the pointer type is Vec3D*, 
		rhs + 1 is exactly 1 Vec3D ahead in memory (12 bytes)

		cast to char*, tell system to point treat as individual bytes rather than Vec3Ds
*/

template<class T>
const unsigned int MySizeOf(const T& rhs)
{
	return (unsigned int) ((const char*) (&rhs + 1) - (const char*) (&rhs));
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// QUESTION 3: Write a function that reverses only the portion of a linked list that comes after 
// the element "TestElement" in the list, starting with that element. For example, given the initial 
// list "A-B-C-D-E-F-G" and TestElement of "D", the resulting list would be "A-B-C-G-F-E-D". Your 
// function should be as robust as possible making no assumptions about the validity of the initial list. 
// Use comments to explain your reasoning. Your function should return the head of the resulting list.
//
//////////////////////////////////////////////////////////////////////////////////////////

/*
	Changes Made:

	No Default Ctor, Next ptr will likely init to garbage
	Init to NULL(0) for safety

	Since this problem really is about the reverse function,
	skipping on overloading big 4 here.

	//Note, integer was added for debugging purposes, commented out now
*/
struct ListNode
{
	//int Data;
	ListNode* Next;

	ListNode()
		:Next(0)
	{
	}
};

/*
	Changes Made:
	Let's be as const correct as possible
	
	If we wish to walk list and possibly reverse, head can't be const,
	but TestElement can (non-const ptr to const data)

	Original Function Sig
		ListNode* ReverseAfterTestElement(ListNode* Head, const ListNode* TestElement);
		-Function doesn't really have the clearest return type
		-Just returning a null pointer or head pointer if args are invalid and no reverse was made
		 doesn't convey to user what function actually did

	New Function Sig
		const ReturnCode ReverseAfterTestElement(ListNode*& Head, const ListNode* TestElement);
		-new enum ReturnCode, conveys to user what actually happened so they can handle accordingly
		-Head is now a non-const reference to a pointer, and "out" parameter
*/

enum ReturnCode
{
	NULL_ARGS,
	NULL_ARG_HEAD,
	NULL_ARG_TESTELMNT,
	TEST_ELMNT_NOT_FOUND,
	TEST_ELMNT_FOUND

};
const ReturnCode ReverseAfterTestElement(ListNode*& Head, const ListNode* TestElement)
{
	//declare our return type and change value as we fall through
	ReturnCode code = ReturnCode::TEST_ELMNT_FOUND;

	//trying to be as descriptive to user as possible

	//missing both?
	if(!Head && !TestElement)
		code = ReturnCode::NULL_ARGS;

	//missing Head?
	else if(!Head)
		code = ReturnCode::NULL_ARG_HEAD;

	//missing Test?
	else if(!TestElement)
		code = ReturnCode::NULL_ARG_TESTELMNT;

	//Good To Go
	//if list only consists of head, this will just fall through
	//and return TESTELMNT_NOT_FOUND
	else
	{
		//get ready to walk the list
		ListNode* pCurrNode = Head;

		//loop until we found the element before test
		//if we had list ABCDEF and C was test element,
		//this loop would break on B
		while(pCurrNode)
		{
			if (pCurrNode->Next == TestElement)
				break;
			pCurrNode = pCurrNode->Next;
		}

		//was test actually in the list?
		if(pCurrNode)
		{
			code = ReturnCode::TEST_ELMNT_FOUND;

			//save the sub list AB off somewhere,
			//since we will need to relink the entire thing
			//once we've reversed the 2nd part
			ListNode* pSubListTail = pCurrNode;

			//move to test element
			pCurrNode = pCurrNode->Next;

			//declare some more points
			ListNode* pPrev = 0;
			ListNode* pNext = 0;

			while(pCurrNode)
			{
				//pNext = D
				pNext = pCurrNode->Next;

				//C->Next = NULL (will eventually be our new end of list)
				pCurrNode->Next = pPrev;

				//pPrev = C
				pPrev = pCurrNode;

				//C = D, (which holds a pointer to E, which will continue to move the chains)
				pCurrNode = pNext;
			}

			//AB->Next = F, EDC
			pSubListTail->Next = pPrev;

		}

		else
		{
			//we loop through the list for test,
			//never found, list remains unchanged
			code = ReturnCode::TEST_ELMNT_NOT_FOUND;
		}
	}

	return code;
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

//return code for robustness,
//simple enums to convey to user what happened in function
enum AssetReturnCode
{
	EQUAL_PARAMS,
	NO_ASSETS_LEVEL_1,
	NO_ASSETS_LEVEL_2,
	SUCCESS
};

//okay, so the issue with this is that the two passed
//in vectors are const, which means we cannot sort them
//so we need to create temporary data structures to avoid
//brute-force n^2 search.

//I'm using an std::unordered_set, which allows for average case 0(1) lookups
const AssetReturnCode FindCommonAssets
(
 const std::vector<Asset*>& LevelOneAssets, 
 const std::vector<Asset*>& LevelTwoAssets, 
 std::vector<Asset*>& outUniqueCommonAssets
 )
{
	AssetReturnCode returnCode = AssetReturnCode::SUCCESS;

	//0 assets?
	if(LevelOneAssets.size() == 0)
		returnCode = AssetReturnCode::NO_ASSETS_LEVEL_1;

	//0 assets?
	else if(LevelTwoAssets.size() == 0)
		returnCode = AssetReturnCode::NO_ASSETS_LEVEL_2;

	//same vectors?
	else if(&LevelOneAssets == &LevelTwoAssets)
	{
		returnCode = AssetReturnCode::EQUAL_PARAMS;
	}

	//valid params
	else
	{
		//declare our pointers
		const std::vector<Asset*>* pToHash   = 0;
		const std::vector<Asset*>* pToSearch = 0;

		//the smaller of the two vectors will be converted to an undordered_set
		if(LevelOneAssets.size() < LevelTwoAssets.size())
		{
			pToHash = &LevelOneAssets;
			pToSearch = &LevelTwoAssets;
		}

		else
		{
			pToHash = &LevelTwoAssets;
			pToSearch = &LevelOneAssets;
		}

		//convert to unordered_set
		std::unordered_set<Asset*> hashedVector(pToHash->begin(), pToHash->end());

		//we need to weed out duplicates, a second unordered_set will handle this
		std::unordered_set<Asset*> finalHashedVec;

		std::vector<Asset*>::const_iterator currSearchItr = pToSearch->begin();
		std::vector<Asset*>::const_iterator endSearchItr  = pToSearch->end();

		std::unordered_set<Asset*>::const_iterator currEntry;

		while(currSearchItr != endSearchItr)
		{
			//find it.. (0)1
			currEntry = hashedVector.find((*currSearchItr));

			//did we find something
			if (currEntry != hashedVector.end())
				//if so, was it found previously (duplicate)?
				if(finalHashedVec.find((*currEntry)) == finalHashedVec.end())
					//add it
					finalHashedVec.insert((*currEntry));

			++currSearchItr;
		}

		//assign the vector to the out param.
		outUniqueCommonAssets = std::vector<Asset*>(finalHashedVec.begin(), finalHashedVec.end());
	}

	//tell user what happened
	return returnCode;
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

/*
	Changes Made:
	Lots of changes, check out OriginalMain.h to see the original classes before refactor

	1)If base Player was meant to be Abstract, it never was: 0 abstract functions and public ctor
		-Made GetDesiredMoveDirection a pure virtual seeming that both AI/Human players calculate late this very differently
		-Made GetMaxMoveSpeed() pure virtual, as the the old version screams for it
	2)Original Data was all public
		-Bad, 0 encapsulation, Vec3D members protected for easy access from derived and inaccessable outside of Player-based classes
	3)Useless Data Members
		-Removed bool bIsAI, useless data member used only once by AnnounceEntry(), refactored to take bool as argument
		-Made JoyStickDirection a member of HumanPlayer, it's extra data not used by the AIPlayer
	
	5)No copies
		-All Player classes all declaring their copy methods (Ctor/assignment) as private members
		-Trying to use them in code will generate compiler error, which is what we want
		-This will assure us that no Players are accidentally getting passed by value
	
	6)Virtual Dtor
		-One of the biggest issues; derived Human/AI players are likely to be deleted through base Player pointers
	
	7)Tick()
		-Another problem,
		-Add the virtual VOnTick() function: if the base Player class is going to hold the main Tick()
		 function, we need a way for derived classes to update their data members as well
*/

#define MAX_COORD 10
class Player
{
private:
	Player(const Player&);
	Player& operator=(const Player&);
protected:
	Vec3D Location;
	Vec3D CurrDirection;
	float MaxMoveSpeed;
public:
	// Announce this player's entry into the match
	explicit Player(const float maxSpeed, const bool isAI)
		:MaxMoveSpeed(maxSpeed)
	{
		AnnounceEntry(isAI);
		
		//give these players a random position between <0, 0, 0> and <1000, 1000, 1000>
		//rand() % upper bound trick works here since MAX_COORD is < RAND_MAX
		Location = Vec3D((float)(rand() % MAX_COORD), (float)(rand() % MAX_COORD), (float)(rand() % MAX_COORD));
	}

	//nothing to explicitly free,
	//but we NEED this to be virtual..
	virtual ~Player()
	{
		printf("A base player has left the match!\n");
	}
	
	//if derived Player wanted to do nothing but update
	//position, we define this as an empty function that will be 
	//called by default, compiler might even optimize the empty call out
	virtual void VOnTick() {}

	// Update the player's position
	void Tick(float DeltaTime)
	{
		//let derived classes run their logic,
		//then update the positions
		VOnTick();

		//an overloaded operator could be written for Vec3D here
		Location.x += CurrDirection.x * MaxMoveSpeed;
		Location.y += CurrDirection.y * MaxMoveSpeed;
		Location.z += CurrDirection.z * MaxMoveSpeed;
	}

	void AnnounceEntry(const bool isAI)
	{
		if (isAI)
		{
			printf("AI has entered the match\n");
		}
		else
		{
			printf("Player has entered the match\n");
		}
	}

	//Added this accessor to get Player's location
	//return the location by const-ref
	//this could be a function that gets called thousands
	//of times per Tick, so we can avoid needless ctor/dtors
	const Vec3D& GetLocation()
	{
		return this->Location;
	}

	const Vec3D& GetLocation() const
	{
		return this->Location;
	}

	const float GetMaxMoveSpeed() const
	{
		return this->MaxMoveSpeed;
	}
};

/*
	Changes Made
	-HumanPlayer must now call Player's ctor explicitly
	-Original CarryWeight Data Member used but never initialized = bad
		-Modify ctor, ensure that CarryWeight is properly initialized

	-GetMaxMoveSpeed()

	-CarryWeight struct
		-Original GetMaxMoveSpeed just returned 100.0f - CarryWeight
			-output could be 0.0f or < 0.0f
			-From the looks of it, original writer assumed carry weight
			 would be no more than 100.0f, so this struct just asserts
			 that constraint
			-explicit ctor to avoid implicit conversion issues
			-asserting right is not the most elegant way to handle this,
			but the constraint is in place and people can handle it
*/
struct CarryWeight
{
	float weight;
	float maxSpeed;

	explicit CarryWeight(const float w)
		:weight(w)
		{
			assert((100.0f - weight) > 0.0f);
			maxSpeed = (100.0f - weight);
		}

	void AddWeight(const float w)
	{
		assert(w > 0.0f && maxSpeed - w > 0.0f);

		weight += w;
		maxSpeed -= w;
	}
};

class HumanPlayer : public Player
{
private:
	HumanPlayer(const HumanPlayer&);
	HumanPlayer& operator=(const HumanPlayer&);

	// This is the total weight carried by this player in his inventory
	CarryWeight carryWeight;
public:
	//now we know that the calculate MaxSpeed must be valid
	HumanPlayer(const CarryWeight carryWgt) 
		: Player(carryWgt.maxSpeed, false), carryWeight(carryWgt) 
		{}

	~HumanPlayer()
	{
		printf("A Human player has left the match!\n");
	}

	void VOnTick()
	{
		//..Update Joystick info or something, blah blah
		//CurrDirection = InputManager::GetJoystrick()->Direction()..

		//if picked up new item
			//update carryWeight and update MaxSpeed//
	}
};

/*
	Changes Made
	-HumanPlayer must now call Player's ctor explicitly
	-CurrentTarget was originally public, now private
	-Based on original author, it looked like CurrentTarget should never be NULL
	-VOnTick allows us to update AILogic, update CurrentTarget is need be, before
	-dereferencing to calculate the CurrDirection
*/
class AIPlayer : public Player
{
private:
	AIPlayer(const AIPlayer&);
	AIPlayer& operator=(const AIPlayer&);


	Player* CurrentTarget;

public:
	AIPlayer(Player* pCurrTrgt) : Player(100.0f, true), CurrentTarget(pCurrTrgt) 
	{
		//let's make sure the Player passed in is valid
		//asserts go away in release build
		assert(CurrentTarget);
	}

	~AIPlayer()
	{
		printf("A AI player has left the match!\n");
	}

	void VOnTick()
	{
		//if time to switch targets || target is NULL
			//CurrentTarget = PlayerFinder::FindClosestPlayer(Location)...

		//..update AI Logic, so on

		CurrDirection = Vec3D(Location.x - CurrentTarget->GetLocation().x,
					 		  Location.y - CurrentTarget->GetLocation().y,
					 		  Location.z - CurrentTarget->GetLocation().z);
	}

};

//////////////////////////////////////////////////////////////////////////////////////////
//
// You may implement test cases for the functions here if you like.
//
//////////////////////////////////////////////////////////////////////////////////////////
void Problem_1();
void Problem_2();
void Problem_3();

//change this constant to whatever you like
//I originally used 10 to test since I can actually see what happened :D
#define NUM_ASSETS 100
void Problem_4();


int main(int argc, char* argv[])
{
	//seed random generator
	//based on system time
	srand((unsigned int) time(0));

	Problem_1();
	Problem_2();
	Problem_3();
	Problem_4();
	
	printf("Text Files with the results of the tests have been successfully created for Problems 1 through 4.\n\n");

	printf("Below is the result of the refactoring of the Player base/derived classes\n");
	printf("Unfortunately, it's difficult to set up a test bed without some sort of game loop.\n");
	printf("Please read the comments I made for my changes to the original Player classes of OriginalMain.h\n\n");

	//note this next section just screws around with the Player classes I "refactored."
	//there isn't much a problem associated with this, you can just see how the structure
	//works compared to the original in OriginalMain.h

	//see my description above for the CarryWeight struct
	//and why it does what it does based on the idea of the carry weight in OriginalMain.h's HumanPlayer class

	//create a base class pointer to HumanPlayer
	Player* chris = new HumanPlayer(CarryWeight(50.0f));

	//create a base class pointer to AIPlayer
	Player* ai = new AIPlayer(chris);
	
	delete chris;
	delete ai;

	return 0;
}

void Problem_1()
{
	pFileProblem_1 = fopen("Problem_1.txt", "w");
	assert(pFileProblem_1);

	fprintf(pFileProblem_1, "Obvious Opposite Directions:\n");
	const Vec3D bulletVel = Vec3D(1.0f, 0.0f, 0.0f/*static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000)*/);
	const Vec3D charFacing = Vec3D(-1.0f, 0.0f, 0.0f/*static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000)*/);

	fprintf(pFileProblem_1, "BulletVelocity = < %f, %f, %f>\n", bulletVel.x, bulletVel.y, bulletVel.z);
	fprintf(pFileProblem_1, "CharacterFacing = < %f, %f, %f>\n", charFacing.x, charFacing.y, charFacing.z);

	if(WasShotFromBehind(bulletVel, charFacing))
		fprintf(pFileProblem_1, "You were shot in the back!\n");
	else
		fprintf(pFileProblem_1, "You were shot in the face!\n");

	fprintf(pFileProblem_1, "\nNot So Obvious Opposite Directions:\n");
	const Vec3D _bulletVel = Vec3D(static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000));
	const Vec3D _charFacing = Vec3D(static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000));

	fprintf(pFileProblem_1, "BulletVelocity = < %f, %f, %f>\n", _bulletVel.x, _bulletVel.y, _bulletVel.z);
	fprintf(pFileProblem_1, "CharacterFacing = < %f, %f, %f>\n", _charFacing.x, _charFacing.y, _charFacing.z);

	if(WasShotFromBehind(_bulletVel, _charFacing))
		fprintf(pFileProblem_1, "You were shot in the back!\n");
	else
		fprintf(pFileProblem_1, "You were shot in the face!\n");

	fclose(pFileProblem_1);
	pFileProblem_1 = 0;
}

void Problem_2()
{
	pFileProblem_2 = fopen("Problem_2.txt", "w");
	assert(pFileProblem_2);

	//type is of Vec3D, 3 floats, 12 bytes,
	//change this to whatever you like to test it out
	Vec3D vec;
	fprintf(pFileProblem_2, "sizeof(Vec3D) returned: %u\nMySizeOf(vec3D) returned: %u\n", sizeof(Vec3D), MySizeOf(vec));
	fclose(pFileProblem_2);
	pFileProblem_2 = 0;
}

void Problem_3()
{
	pFileProblem_3 = fopen("Problem_3.txt", "w");
	assert(pFileProblem_3);

	ListNode* head = new ListNode;
	ListNode* pCurrNode = head;
	ListNode* pTest = 0;

	for(int i = 0; i < 9; ++i)
	{
		pCurrNode->Next = new ListNode;

		if(i == 3)
			pTest = pCurrNode;

		pCurrNode = pCurrNode->Next;
	}

	pCurrNode = head;
	fprintf(pFileProblem_3, "Original List:\n");

	for(int i = 0; pCurrNode; ++i)
	{
		fprintf(pFileProblem_3, "List[%d]: %p\n", i, pCurrNode);
		pCurrNode = pCurrNode->Next;
	}

	fprintf(pFileProblem_3, "\nReversal at %p\n", pTest);

	ReverseAfterTestElement(head, pTest);

	pCurrNode = head;

	for(int i = 0; pCurrNode; ++i)
	{
		fprintf(pFileProblem_3, "List[%d]: %p\n", i, pCurrNode);
		pCurrNode = pCurrNode->Next;
	}

	fclose(pFileProblem_3);
	pFileProblem_3 = 0;
}

void Problem_4()
{
	pFileProblem_4 = fopen("Problem_4.txt", "w");
	assert(pFileProblem_4);

	Asset* pAssetField = new Asset[NUM_ASSETS];

	std::vector<Asset*> levelOneAssets;
	std::vector<Asset*> levelTwoAssets;
	std::vector<Asset*> out;

	const int levelOneSize = rand() % NUM_ASSETS + 1;
	const int levelTwoSize = rand() % NUM_ASSETS + 1;

	levelOneAssets.reserve(levelOneSize);
	levelTwoAssets.reserve(levelTwoSize);

	Asset* pAsset;
	for(int i = 0; i < levelOneSize; ++i)
	{
		pAsset = &pAssetField[rand() % NUM_ASSETS];
		levelOneAssets.push_back(pAsset);
		fprintf(pFileProblem_4, "levelOneAssets[%d] = %p\n", i, pAsset);
	}

	fprintf(pFileProblem_4, "\n");

	for(int i = 0; i < levelTwoSize; ++i)
	{
		pAsset = &pAssetField[rand() % NUM_ASSETS];
		levelTwoAssets.push_back(pAsset);
		fprintf(pFileProblem_4, "levelTwoAssets[%d] = %p\n", i, pAsset);
	}

	fprintf(pFileProblem_4, "\n");

	//see my Timer.h class.
	Timer algoTimer;
	algoTimer.Start();
		FindCommonAssets(levelOneAssets, levelTwoAssets, out);
	algoTimer.Stop();

	fprintf(pFileProblem_4,"%d Common Asset(s):\n", out.size());

	for(size_t i = 0; i < out.size(); ++i)
	{
		fprintf(pFileProblem_4, "out[%d] = %p\n", i, out[i]);
	}

	fprintf(pFileProblem_4, "FindCommonAssets on vectors of size %d and %d took %f seconds.\n", levelOneAssets.size(), 
			levelTwoAssets.size(), algoTimer.GetSeconds());
	fclose(pFileProblem_4);
}

#pragma warning(pop)
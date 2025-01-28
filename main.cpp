///**  8 Puzzle Problem  **///

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <set>

using namespace std;

int goal[3][3] = { {1, 2, 3},{4, 5, 6},{7, 8, 0} };            // Required Goal State
int current[3][3] = { {0, 0, 0},{0, 0, 0},{0, 0, 0} };         // Current Board State
int heuFunc = 1;                                               // Initializing heuristic function variable as 1
int algo = 1;                                                  // Intializing Algorithm variable as 1
char actions[4] = { 'u', 'd', 'l', 'r' };                      // 4 possible actions: Up, Down, Left and Right
typedef struct Node											   // Defining structure datatype 
{
	int g;													   // Real cost from parent to node
	int h;													   // Strainght path cost from node to goal
	int f;													   // Heuristic function 
	int board[3][3];                                           // Initializing 3*3 size board 
	char action;                                               // Declaring action as character datatype
	Node* ch1;                                                 // Linked list for child1
	Node* ch2;                                                 // Linked list for child2
	Node* ch3;                                                 // Linked list for child3
	Node* ch4;                                                 // Linked list for child4
	Node* children[4];                                         // Linked list for children array of size 4
	Node* parent;                                              // Linked list for parent
	Node* next;                                                // Linked list for next node
	Node* nextOpenSet;                                         // Linked list for the set to be opened
	Node* nextExploredSet;                                     // Linked list for set next to be explored
}* node;

node headOpen = NULL;                                          // Initializing opened node as Null
node tempOpen = NULL;                                          // Initializing temporeraly opened node as Null
node currOpen = NULL;                                          // Initializing cuurently opened node as Null

node headExplored = NULL;                                      // Initializing explored node as Null
node tempExplored = NULL;                                      // Initializing temporeraly explored node as Null     
node currExplored = NULL;                                      // Initializing cuurently explored node as Null  

void manhattanDist(node n);                                    // Declaring void function for Manhattan Distance Algorithm
void missingTiles(node n);                                     // Declaring void function for Missing Tiles Algorithm
void printNodeBoard(node n);                                   // Declaring void function for printing Node Board
node makeNode(int board[][3], node parent);                    // Declaring function for generating nodes
void addToExplored(node n);									   // Declaring void function for explored nodes	
void addToOpen(node n);                                        // Declaring void function for nodes to be opened
node lowestCostFromOpen();                                     // Declaring function for determing lowest cost
node getChild(node parent, char action);                       // Declaring function for fetching child nodes
bool inExplored(node n);                                       // Declaring bool function for nodes explored
void removeFromOpen(node n);                                   // Declaring void function for explored nodes to be removed from open
void traceSolution(node n);                                    // Declaring void function for determining solution
void printBoard(int board[][3]);                               // Declaring void function for printing the 8 Puzzle Board
void randomizeBoard(int a);                                    // Declaring void function for randomly initialing values for 8 Puzzle
int getInvCount(int arr[]);                                    // Declaring a utility function to count inversions in given array 'arr[]'

int main() {
	cout << "Goal Board\n";                                    
	printBoard(goal);                                          // Calling the printBoard function to print goal board
	cout << "\nDo you wish to enter a new Goal Board or do you want to select the goal board displayed above?"; 
	cout << "\n1. Enter New Goal Board\n2. Select the above Goal Board\n";
	cout << "Enter 1 or 2: ";
	int choiceGoal;
	set<int> numbers;
	cin >> choiceGoal;
		if (choiceGoal == 1) {
		cout << "Enter the Goal Board you want to achieve using Search Algorithms, enter 0 for the blank space.\n";
		int newGoal[3][3] = { {0, 0, 0},{0, 0, 0},{0, 0, 0} };
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				printBoard(newGoal);
				cout << "Row " << i << " Col " << j << ": ";
				cin >> newGoal[i][j];                                          // Asks user to enter values for each tile
				while (newGoal[i][j] < 0 || newGoal[i][j] > 8) {               // Iterate until value entered is less than 0 or greater than 8
					cout << "Values should be in range 0-8. Try again.\n";     // Then return to ask for limit 0-8
					cout << "Row " << i << " Col " << j << ": ";
					cin >> newGoal[i][j];                                      // Take input from the user for the same tile again
			 	}
				while(!numbers.insert(newGoal[i][j]).second){                  // Iterate to avoid similar values
    				cout << "Duplicate number. Try again \n";
    				cout << "Row " << i << " Col " << j << ": ";
					cin >> newGoal[i][j];
				while (newGoal[i][j] < 0 || newGoal[i][j] > 8) {               // Iterate until value entered is less than 0 or greater than 8
					cout << "Values should be in range 0-8. Try again.\n";     // Then return to ask for limit 0-8
					cout << "Row " << i << " Col " << j << ": ";
					cin >> newGoal[i][j];                                      // Take input from the user for the same tile again
			 	}
        		}                                         

			 	goal[i][j] = newGoal[i][j];
			}
		}
	}
	else {
		cout << "You have selected the above Goal Board\n";                    // If user enters '2' then same goal board is selected
	}
	cout << "\nGoal Board\n";
	printBoard(goal);
	
	cout << "\nDo you wish to enter the player board or do you want to randomize the goal state to make the player board?";
	cout << "\n1. Enter Board\n2. Randomize\n";
	cout << "Enter 1 or 2: ";
	int choice;
	set<int> numbers1;
	cin >> choice;
	if (choice == 1) {
		cout << "Enter the player board that you want to solve, enter 0 for the blank space.\n";
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				printBoard(current);
				cout << "Row " << i << " Col " << j << ": ";
				cin >> current[i][j];                                       // Asks user to enter values for each tile
				while (current[i][j] < 0 || current[i][j] > 8) {            // Iterate until value entered is less than 0 or greater than 8
					cout << "Values should be in range 0-8. Try again.\n";  // Then return to ask for limit 0-8
					cout << "Row " << i << " Col " << j << ": ";
					cin >> current[i][j];                                   // Take input from the user for the same tile again
			 	}
			 	while(!numbers1.insert(current[i][j]).second){              // Iterate to avoid similar values
    				cout << "Duplicate number. Try again \n";
    				cout << "Row " << i << " Col " << j << ": ";
					cin >> current[i][j];
				while (current[i][j] < 0 || current[i][j] > 8) {            // Iterate until value entered is less than 0 or greater than 8
					cout << "Values should be in range 0-8. Try again.\n";  // Then return to ask for limit 0-8
					cout << "Row " << i << " Col " << j << ": ";
					cin >> current[i][j];                                   // Take input from the user for the same tile again
			 	}
        		}
			}
		}
	}
	else {
		randomizeBoard(5);                                          // If user enters '2' then randomly initialize board
	}
	cout << "\nPlayer Board\n";
	printBoard(current);
	
	int solvable = getInvCount((int *)current);                     // Count inversions in given 8 puzzle
  	if (solvable%2 == 0){                                           // If inversion count is even
  		cout<<"\nThe given set of arrangement is Solvable\n";       // Then the arrangment is solvable
	  }
	else if(solvable%2 == 1){                                       // If inversion count is odd
		cout << "\nThe given set of arrangement is Not Solvable\n"; // Then the arrangment is not solvable
		cout << "Try with a different set of arrangment whose inversion count is even.\n";
		return 0;
	}
	
	cout << "\n1. A* Search\n2. Greedy Search\nChoose a Search Algorithm: ";
	cin >> algo;                                               // Asks user for choice of algorithm: A* or Greedy Search
                      	
	cout << "\n1. Manhattan Distance\n2. Misplaced Tiles\nChoose a heuristic function: ";
	cin >> heuFunc;                                            // Asks user for choice of algorithm: Manhattan Distance or Missing Tiles

	node currentBoard = new Node;                              // Initializing current state of the board
	node child = new Node;                                     // Child node is set to be new node 
	currentBoard = makeNode(current, NULL);                    // Calling the function for current board
	addToOpen(currentBoard);                                   // Add current board to open set
	addToExplored(currentBoard);                               // Add current board to explored set
	int ctr = 0;
	
	while (currentBoard->h != 0) {                             // Access value of h and apply loop for condition until its not equal to 0 
		currentBoard = lowestCostFromOpen();				   // Determining lowest cost or direct straing line cost forcurrent state
		removeFromOpen(currentBoard);                          // Remove current board state from open set
		for (int i = 0; i < 4; i++) {
			child = getChild(currentBoard, actions[i]);        // Getting child from current board and required action (up, down, etc)
			if (child != NULL && !inExplored(child)) {         // If child is not Null and not in explored
				addToOpen(child);                              // Add current child to open list
				addToExplored(child);                          // Add current child to explored list
			}
		}
	}
	traceSolution(currentBoard);                               // Determine solution for the current board state
}

int getInvCount(int arr[])                                     // A utility function to count inversions in given array 'arr[]'
{
    int inv_count = 0;
    for (int i = 0; i < 9 - 1; i++)
        for (int j = i+1; j < 9; j++)
             if (arr[j] && arr[i] &&  arr[i] > arr[j])         // Value 0 is used for empty space
                  inv_count++;
    return inv_count;
}
 

void randomizeBoard(int numRand) {
	int newBoard[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			newBoard[i][j] = goal[i][j];                       // New board initialized as goal board states
		}
	}
	int ctr = 0;
	while (ctr < numRand) {
		ctr++;
		int blankR, blankC;                                    // Declaring blank row and column for blank board
		int newR, newC;                                        // Declaring new row and column
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (newBoard[i][j] == 0) {                     // If current new board tile value equals 0
					blankR = i;                                // Initialize blank row tile to be equal to i
					blankC = j;                                // Initialize blank column tile to be equal to j
				}
			}
		}

		int temp = rand()%100;                                 // Randomly initializing temp variable from 0 to 100 
		if (temp < 50) { //Up Down                             // If temp is less than 50
			if (blankR == 0) {                                 // And if blank row equals to 0
				newR = blankR + 1;                             // Then equate new row equal to blankR + 1
				newC = blankC;                                 // and equate new column to be the same value as blankC
			}
			if (blankR == 1) {                                 // If blankR equals 1
				newC = blankC;                                 // Equate new column to be as same value as blankC
				int temp1 = rand() % 100;                      // Randomly initializing temp1 variable from 0 to 100 
				if (temp1 < 50) {                              // If temp1 is less than 50
					newR = blankR + 1;                         // Then equate new row equal to blankR + 1
				}
				else {
					newR = blankR - 1;                         // Else, equate new row equal to blankR - 1 
				}
			}
			if (blankR == 2) {                                 // If blankR equals 2
				newR = blankR - 1;                             // Then equate new row equal to blankR - 1 
				newC = blankC;                                 // and equate new column to be the same value as blankC
			}
			int temp2 = newBoard[newR][newC];                  // Initialize temp2 as matrix of newBoard of values new row and new column
			newBoard[newR][newC] = newBoard[blankR][blankC];   // Set values of board of blank row and column to new row and column for new board
			newBoard[blankR][blankC] = temp2;                  // Set board of blank row and column equal to temp2
		}
		else { // Left Right
			if (blankC == 0) {                                 // If blank row equals to 0
				newC = blankC + 1;                             // Then equate new column equal to blankC + 1
				newR = blankR;                                 // and equate new row to be the same value as blankR
			}
			if (blankC == 1) {                                 // If blank row equals to 1
				newR = blankR;                                 // then equate new row to be the same value as blankR                      
				int temp1 = rand() % 100;                      // Randomly initializing temp1 variable from 0 to 100 
				if (temp1 < 50) {                              // If temp1 is less than 50
					newC = blankC + 1;                         // Then equate new column equal to blankC + 1
				}
				else {
					newC = blankC - 1;                         // Else, equate new column equal to blankC - 1
				}
			}
			if (blankC == 2) {                                 // If blankR equals 2
				newC = blankC - 1;                             // Then equate new column equal to blankC - 1
				newR = blankR;                                 // and equate new row to be the same value as blankR  
			}
			int temp2 = newBoard[newR][newC];                  // Initialize temp2 as matrix of newBoard of values new row and new column
			newBoard[newR][newC] = newBoard[blankR][blankC];   // Set values of board of blank row and column to new row and column for new board
			newBoard[blankR][blankC] = temp2;                  // Set board of blank row and column equal to temp2
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			current[i][j] = newBoard[i][j];                    // Generate current board tile value as newBoard tile value from above 
		}
	}
}

void manhattanDist(node n) {                                  // Manhattan Distance Algorithm 
	int dist = 0;                                              
	for (int r1 = 0; r1 < 3; r1++)                            
	{
		for (int c1 = 0; c1 < 3; c1++)
		{
			if (n->board[r1][c1] != 0) {                      // Access the current value of board and apply condition if it is not equal to 0 
				for (int r2 = 0; r2 < 3; r2++)
				{
					for (int c2 = 0; c2 < 3; c2++)
					{
						if (n->board[r1][c1] == goal[r2][c2]) {      // Check if board state is equal to goal state
							dist += abs(r1 - r2) + abs(c1 - c2);     // Then determine cost from actions taken upon rows and columns
						}
					}
				}
			}
		}
	}
	n->h = dist;                                             // Heuristic value will be the distance (path) calculated
}

void missingTiles(node n) {                                  // Missing Tiles Algorithm
	int missingTiles = 0;                                      
	for (int r1 = 0; r1 < 3; r1++)
	{
		for (int c1 = 0; c1 < 3; c1++)
		{
			if (n->board[r1][c1] != goal[r1][c1]) {          // Check if board current value is not equal to goal corresponding tile value
				missingTiles++;                              // Increment missingTiles by 1
			}
			
		}
	}
	n->h = missingTiles;                                     // Heuristic value will be the total number of ties that were misplaced
}

node getChild(node parent, char action) {                    // Get child from parent node and action
	int blankR, blankC;                                      // Initializing blankR and blankC
	node n = new Node;                                       // structure node n equas to new node
	int childBoard[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (parent->board[i][j] == 0) {                  // Check if parent in board for curren value equals to 0
				blankR = i;                                  
				blankC = j;
			}
		}
	}
	if (action == 'u') {                                     // If action is "Up"
		if (blankR == 1 || blankR == 2) {                    // and if blankR equals 1 or 2
  			for (int i = 0; i < 3; i++) {                   
				for (int j = 0; j < 3; j++) {
					childBoard[i][j] = parent->board[i][j];  // Current child board value equals to the board value from parent action
				}
			}
			childBoard[blankR][blankC] = parent->board[blankR - 1][blankC];  // Current child board value will be equal to parent previous row of same column
			childBoard[blankR - 1][blankC] = parent->board[blankR][blankC];  // Previous row of child board value will be equal to the current parent board value
			n = makeNode(childBoard, parent);                                // n equals to the child board fetched from parent
		}
		else {
			n = NULL;                                        // Else n equals Null
		}
	}
	if (action == 'd') {                                     // If action is "down"
		if (blankR == 1 || blankR == 0) {                    // and if blankR equals 1 or 0
			for (int i = 0; i < 3; i++) { 
				for (int j = 0; j < 3; j++) {
					childBoard[i][j] = parent->board[i][j];  // Current child board value equals to the board value from parent action
				}
			}
			childBoard[blankR][blankC] = parent->board[blankR + 1][blankC]; // Current child board value will be equal to parent future row of same column
			childBoard[blankR + 1][blankC] = parent->board[blankR][blankC]; // Future row of child board value will be equal to the current parent board value
			n = makeNode(childBoard, parent);                               // n equals to the child board fetched from parent
		} 
		else {
			n = NULL;                                        // Else n equals Null
		}
	}
	if (action == 'l') {                                     // If action is "Down"
		if (blankC == 1 || blankC == 2) {                    // and if blankC equals 1 or 2
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					childBoard[i][j] = parent->board[i][j];  // Current child board value equals to the board value from parent action
				}
			}
			childBoard[blankR][blankC] = parent->board[blankR][blankC - 1]; // Current child board value will be equal to parent previous column of same row
			childBoard[blankR][blankC - 1] = parent->board[blankR][blankC]; // Previous column of child board value will be equal to current parent board value
			n = makeNode(childBoard, parent);                               // n equals to the child board fetched from parent
		}
		else {
			n = NULL;                                        // Else n equals Null
		}
	}
	if (action == 'r') {                                     // If action is "Right"
		if (blankC == 1 || blankC == 0) {                    // and if blankC equals 1 or 0
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					childBoard[i][j] = parent->board[i][j];  // Current child board value equals to the board value from parent action
				}
			}
			childBoard[blankR][blankC] = parent->board[blankR][blankC + 1]; // Current child board value will be equal to parent future column of same row
			childBoard[blankR][blankC + 1] = parent->board[blankR][blankC]; // Future column of child board value will be equal to current parent board value
			n = makeNode(childBoard, parent);                               // n equals to the child board fetched from parent
		}
		else {
			n = NULL;                                        // Else n equals Null
		}
	}
	if (n != NULL) {                                         // If n remains Null 
		n->action = action;                                  // Then access actions for structure n
	}
	return n;
}

void printBoard(int board[][3]) {                            // Funtion to print the board of size 3*3
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

void printNodeBoard(node n) {                                // Function to print board state with heuristic function and cost for every state (Child Board)
	if (n == NULL) {
		cout << "\Empty Board\n";
	}
	else {
		cout << "\n";
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cout << n->board[i][j] << " ";
			}
			cout << endl;
		}
		if (algo == 1){
			cout << ">> f = " << n->f << ", g = " << n->g << ", h = " << n->h;
			cout << "\n";
		}
		else if (algo == 2){
			cout << ">> f = " << n->f << ", h = " << n->h;
			cout << "\n";
		}
		
	}
}

node makeNode(int board[][3], node parent) {                 // Function to solve the given board, takes input as board and parent node
	node n = new Node;                                       // Structure node n is taken as new node
	n->parent = parent;                                      // Access the value of parent and initialize it as parent for n
	n->nextOpenSet = NULL;                                   // Access the value of next open set and initialize it to Null for n
	n->nextExploredSet = NULL;                               // Access the value of next explored set and initialize it to Null for n
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			n->board[i][j] = board[i][j];                    // Access the value of the current board state and initialize it as board state value for n
		}
	}
	if (heuFunc == 1) {                                      // If heuristic function equals to 1 
		manhattanDist(n);                                    // Then solve the 8 Puzzle uszing Manhattan Distance
	}
	if (heuFunc == 2) {                                      // If heuristic function equals to 2
		missingTiles(n);                                     // Then solve the 8 Puzzle uszing Missing Tiles
	}
	if (n->parent == NULL) {                                 // If parent value for n is Null
		n->g = 0;                                            // Then initialize real cost g to b 0
	}
	else {
		n->g = n->parent->g + 1;                             // Else, calculate the real cost from parent to node
	}
	
	if (algo == 1){
		n->f = n->g + n->h;                                  // Calculate the function using real cost and heuristic cost (A* Search)
	}
	else if(algo == 2){
		n->f = n->h;                                         // Calculate the function using heuristic cost only (Greedy Search)
	} 
	return n;
}

void addToExplored(node n) {                                 // Function to add visited nodes to explored set
	if (headExplored == NULL) {                              // If node explored is Null
		headExplored = n;                                    // Then head explored equals to the structure node n
	}
	else {
		currExplored = headExplored;                         // Else, current explored node equals to the head explored 
		while (currExplored->nextExploredSet != NULL) {      // If next node to explore is not Null
			currExplored = currExplored->nextExploredSet;    // Then assign next explored node to be current explored
		}
		currExplored->nextExploredSet = n;                   // structure node n equals to the value next explored accessed by current explored node
	}
}

bool inExplored(node n) {                                    // bool function to determine all explored nodes
	currExplored = headExplored;                             // Initial explored node equals to the current explored node
	bool nodeFound = false;                                  // varialbe nodeFound is declared as False
	while (currExplored != NULL && !nodeFound) {             // Iterate until current explored is not Null and it is not found as well
		int ctr = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (currExplored->board[i][j] == n->board[i][j]) {    // If current value for structure node n equals board value for current explored
					ctr += 1;                                         // Then increment counter by 1
				}
			}
		}
		if (ctr == 9) {                                               // If counter equals 9
			return true;                                              // then return true
		}
		currExplored = currExplored->nextExploredSet;                 // Access the value of next explored set and assign it to current explored
	}
	return false;
}

void addToOpen(node n) {                                              // Function to visit node
	if (headOpen == NULL) {                                           // If initial node to open is Null 
		headOpen = n;                                                 // Then assign the value of node n to variable headOpen
	}
	else {
		currOpen = headOpen;                                          // Else, node to currently open equals the node to be visited
		while (currOpen->nextOpenSet != NULL) {                       // Iterate until next nodes to open is not Null
			currOpen = currOpen->nextOpenSet;                         // Access all the next nodes to be visited
		}
		currOpen->nextOpenSet = n;                                    // structure node n equals to the value to next open set accessed by currently opened node
	}
}

node lowestCostFromOpen() {                                           // Function to calculate lowest cost from parent to goal
	node n = new Node;                                                // Structure node n is taken as new node
	currOpen = headOpen;                                              // Node to currently open assigned to the node to be visited
	n = headOpen;                                                     // Structure node n equals to the list of nodes to be visited
	while (currOpen != NULL) {                                        // Iterate until the nodes to currently visit is not Null
		if (currOpen->f < n->f) {                                     // If function of node n is greater than the function accessed by currently visiting nodes
			n = currOpen;                                             // Then current nodes to visit are assigned to n
		}
		currOpen = currOpen->nextOpenSet;                             // Access all the next nodes to be visited
	}
	return n;
}

void removeFromOpen(node n) {                                         // Remove node that is visited from the open list
	currOpen = headOpen;                                              // Node to currently open assigned to the node to be visited
	tempOpen = headOpen;                                              // Node to currently open assigned to temporarily visit it
	bool nodeFound = false;                                           // Varialbe nodeFound is declared as False
	int ctr = 0;                                                      // Counter initialzed as 0
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (currOpen->board[i][j] == n->board[i][j]) {            // If current value for structure node n equals board value for currently opened board  
				ctr += 1;                                             // Increment counter by 1
			}
		}
	}
	if (ctr == 9) {                                                   // If counter equals 9
		nodeFound = true;                                             // Then return true
	}
	while (currOpen != NULL && !nodeFound) {                          // Iterate until current opened node is not Null and it is not found as well 
		tempOpen = currOpen;                                           
		currOpen = currOpen->nextOpenSet;                             // Access the value of next visiting nodes to currently open 
		int ctr = 0;                                                      
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (currOpen->board[i][j] == n->board[i][j]) {        // If current value for structure node n equals board value for currently opened board 
					ctr += 1;
				}
			}
		}
		if (ctr == 9) {
			nodeFound = true;
		}
	}
	if (currOpen != NULL) {                                           // If node to open currently is not Null
		n = currOpen;                                                 // Then assign the current opened node to n
		currOpen = currOpen->nextOpenSet;                             // Access all the next nodes to be visited
		tempOpen->nextOpenSet = currOpen;                             // Assign the currnet opened node to temporary opened node in next visiting node set
		if (n == headOpen) {                                          // If n equals initial opened lsit 
			headOpen = headOpen->nextOpenSet;                         // Assign next open set to visit the nodes
			tempOpen = NULL;                                          // Assign the temporary opened node as Null
		}
	}
}

void traceSolution(node n) {                                          // Function to trace the solution 
	node currSol = n;                                                 // Structure node n assigned to the structure current solution 
	int steps = 0;                                                    // Initializing steps as 0

	while (currSol != NULL)                                           // Iterate until current solution is not Null
	{
		printNodeBoard(currSol);                                      // Print board after each step 
		cout << ">> Step: " << steps << " Action: " << currSol->action << endl;   
		currSol = currSol->parent;                                    // Parent state is accessed and assigned as current solution
		steps++;                                                      // Steps increment by 1
	}
	cout << "\n\nThe puzzle was solved in " << steps << " steps\n";  
	cout << "Scroll up to see the individual steps\n";
	cout << "Steps are in order bottom to top for ease. Consider step numbering in descending order.\n";
}

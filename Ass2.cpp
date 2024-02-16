/*
File:Ass2.cpp
Author: Jason Waid | 400424500
McMaster University | Numerical Linear Algebra and Numerical Optimization | SFWRTECH 4MA3
Professor: SESHASAI SRINIVASAN 
Description: Implementation of the Inverse Iteration and Rayleigh-Quotient Iteration with Shifts to find eigenvalues
*/

#include <iostream>
#include <ios>
#include <iomanip>  
#include <limits>
#include <string>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;

void displayGreeting();
void displayMenu();
bool checkInputInt(string inputBuffer);
bool checkInputDouble(string inputBuffer);
int getMatrixSize(string inputBuffer);
void getMatrixDisplay(string inputBuffer, int matrixSize, double matrix[][11]);
double getMatrixValue(string inputBuffer, int matrixSize);
double getInitialGuess(string inputBuffer);
void clearInputBuffer();
void printMatrix(double matrix[][11], int matrixSize);
void printInitialGuess(double matrix[][11], int matrixSize);
void performRayleighQuotientIterationWithShifts(double matrix[][11], int matrixSize);
void performInverseIteration(double matrix[][11], int matrixSize);
void performGaussianElimination(double matrix[][11], int numUnknowns);
void displayInitialMatrixMessage();
void displayInitialGuessMessage();
void displayInverseIterationData(int iteration, double matrix[][11], double infiniteNorm, int matrixSize);
void performBackwardsSubstitution(double matrix[][11], int matrixSize);
void displayRayleighQuotientIterationWithShiftsTableHeaders();
void displayRayleighQuotientIterationWithShiftsTableData(int iteration, double matrix[][11], double infiniteNorm, int matrixSize, double rayQuotient);
double calcRayleighQuotient(double matrix[][11], int matrixSize);

/*
Function: displayGreeting
Purpose: Greeting message intended for app launch
Return: none
Author: Jason Waid
Date Modified: 03/12/2022
*/
void displayGreeting() {
	cout << "Welcome to Assignment 2!\n";
	cout << "Developed by Jason Waid\n";
}

/*
Function: displayMenu
Purpose: Menu for Eigenvalue calculation method
Return: none
Author: Jason Waid
Date Modified: 03/12/2022
*/
void displayMenu() {
	cout << "Choose a program:\n";
	cout << "1: Inverse Iteration\n";
	cout << "2: Rayleigh-Quotient Iteration with Shifts\n";
}

/*
Function: checkInputInt
Purpose: Validates Input String by checking if it is a Number
Param: a input buffer of type string
Return: Boolean depending on success or failure
Author: Jason Waid
Date Modified: 02/05/2022
*/
bool checkInputInt(string inputBuffer) {

	//check for negative val
	if (!strcmp(inputBuffer.substr(0, 1).c_str(), "-")) {
		cout << inputBuffer.c_str() << " is not valid!\n";
		clearInputBuffer();
		return false;
	}

	for (char const& c : inputBuffer) {

		if (isdigit(c) == 0) {
			cout << inputBuffer.c_str() << " is not valid!\n";
			clearInputBuffer();
			return false;
		}	

	}
	if (stoi(inputBuffer) > 0 && stoi(inputBuffer) <= 10) {
		return true;
	}
	else {
		cout << inputBuffer.c_str() << " is not valid!\n";
		clearInputBuffer();
		return false;
	}
}

/*
Function: checkInputMenu
Purpose: Validates Input String by checking if it is a valid Number
Param: a input buffer of type string
Return: Boolean depending on success or failure
Author: Jason Waid
Date Modified: 02/05/2022
*/
bool checkInputMenu(string inputBuffer) {

	//check for negative val
	if (!strcmp(inputBuffer.substr(0, 1).c_str(), "-")) {
		cout << inputBuffer.c_str() << " is not valid!\n";
		clearInputBuffer();
		return false;
	}
	for (char const& c : inputBuffer) {
		if (isdigit(c) == 0) {
			cout << inputBuffer.c_str() << " is not valid!\n";
			clearInputBuffer();
			return false;
		}
	}
	if (stoi(inputBuffer) == 1 || stoi(inputBuffer) == 2) {
		return true;
	}
	else {
		cout << inputBuffer.c_str() << " is not valid!\n";
		clearInputBuffer();
		return false;
	}
}

/*
Function: checkInputDouble
Purpose: Validates Input String by checking if it is a Number
Param: a input buffer of type string
Return: Boolean depending on success or failure
Author: Jason Waid
Date Modified: 02/05/2022
*/
bool checkInputDouble(string inputBuffer) {
	bool negativeCheck = false;
	if (!strcmp(inputBuffer.substr(0, 1).c_str(), "-")) {
		inputBuffer.erase(0);
		negativeCheck = true;

	}

	for (char const& c : inputBuffer) {
		if (isdigit(c) == 0) {
			cout << inputBuffer.c_str() << " is not valid!\n";
			clearInputBuffer();
			return false;
		}
	}
	if (negativeCheck == true) {
		inputBuffer.insert(0, "-");
	}
	return true;
}

/*
Function: getMenuOption
Purpose: Gets input from user for number of desired unknowns
Param: a input buffer of type string
Return: Integer value entered
Author: Jason Waid
Date Modified: 02/05/2022
*/
int getMenuOption(string inputBuffer) {
	do {
		cin >> inputBuffer;
	} while (!checkInputMenu(inputBuffer));
	return atoi(inputBuffer.c_str());
}

/*
Function: getMatrixSize
Purpose: Gets input from user for size of square matrix
Param: a input buffer of type string
Return: Integer value entered
Author: Jason Waid
Date Modified: 02/05/2022
*/
int getMatrixSize(string inputBuffer) {
	do {
		cout << "Enter size of square matrix (max is 10): ";
		cin >> inputBuffer;
	} while (!checkInputInt(inputBuffer));
	return atoi(inputBuffer.c_str());
}

/*
Function: getMatrixDisplay
Purpose: Prompts user for input for matrix values
Param: a input buffer of type string
Return: Integer value entered
Author: Jason Waid
Date Modified: 03/12/2022
*/
void getMatrixDisplay(string inputBuffer, int matrixSize, double matrix[][11]) {
	for (int i = 0; i < matrixSize; i++) {
		cout << "Now entering Matrix Row " << i + 1 << "\n";
		for (int k = 1; k <= matrixSize; k++) {

			if (k == 1) {
				cout << "[ ?" << k;
			}
			else
				cout << " ?" << k;
		}
		cout << " ]\n";

		for (int j = 0; j < matrixSize; j++) {
			matrix[i][j] = getMatrixValue(inputBuffer, j);
			clearInputBuffer();
		}
		//matrix[i][matrixSize] = getFactorB(inputBuffer);
		matrix[i][matrixSize] = getInitialGuess(inputBuffer);
		//clearInputBuffer();
	}
}

/*
Function: getMatrixValue
Purpose: Gets the value matrix xi
Param: inputBuffer which is users input, matrixSize which is size of square matrix
Return: Double conversion of inputBuffer after validation is passed
Author: Jason Waid
Date Modified: 03/12/2022
*/
double getMatrixValue(string inputBuffer, int matrixSize) {
	do {
		cout << "Enter value for ?" << matrixSize + 1 << ": ";
		cin >> inputBuffer;
	} while (!checkInputDouble(inputBuffer));
	return stod(inputBuffer);
}

double getInitialGuess(string inputBuffer) {

	do {
		cout << "Enter value for initial guess value b: ";
		cin >> inputBuffer;
	} while (!checkInputDouble(inputBuffer));

	clearInputBuffer();
	return stod(inputBuffer);
}


/*
Function: clearInputBuffer
Purpose: clears the input buffer.
Param: NONE
Return: NONE
Author: Jason Waid
Date Modified: 02/05/2022
*/
void clearInputBuffer() {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/*
Function: printMatrix
Purpose: prints the matrix
Param: double matrix[][11] which is the matrix in question, int matrixSize which is size of square matrix
Return: NONE
Author: Jason Waid
Date Modified: 03/12/2022
*/
void printMatrix(double matrix[][11], int matrixSize) {

	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			cout << matrix[i][j] << "\t";
		}
		cout << "\n";
	}
	
}

/*
Function: printInitialGuess
Purpose: prints the matrix
Param: double initialGuess[2] the initial guess
Return: NONE
Author: Jason Waid
Date Modified: 03/12/2022
*/
void printInitialGuess(double matrix[][11], int matrixSize) {

	cout << "x0 =  [ ";

	for (int i = 0; i < matrixSize; i++) {

		if (i != matrixSize-1) {
			cout << matrix[i][matrixSize] << ", ";
		}
		else {
			cout << matrix[i][matrixSize];
		}
	}
	cout << " ]^T\n";
}

/*
Function: displayInverseIterationTableHeaders
Purpose: prints table headers for inverse iteration
Param: NONE
Return: NONE
Author: Jason Waid
Date Modified: 03/14/2022
*/
void displayInverseIterationTableHeaders() {
	cout << "k\t\t\t\tx[k]^T\t\t\t||y[k]||inf.norm\n";
	cout << "-----------------------------------------------------------------------------------------------------------\n";
}



/*
Function: displayRayleighQuotientIterationWithShiftsTableHeaders
Purpose: prints table headers for Rayleigh Quotient Iteration With Shifts
Param: NONE
Return: NONE
Author: Jason Waid
Date Modified: 03/14/2022
*/
void displayRayleighQuotientIterationWithShiftsTableHeaders() {
	cout << "k\t\t\t\tx[k]^T\t\t\t||y[k]||inf.norm x[k]^T*Ax[k]/x[k]^T*x[k]\n";
	cout << "-----------------------------------------------------------------------------------------------------------\n";
}

/*
Function: calcInfiniteNorm
Purpose: calculated infinity norm of given vector
Param: double matrix: the given matrix, b values are given vector, matrixSize: size of matrix, also the column number of the b vals
Return: NONE
Author: Jason Waid
Date Modified: 03/14/2022
*/
double calcInfiniteNorm(double matrix[][11] , int matrixSize) {

	double max = 0;

	for (int i = 0; i < matrixSize; i++) {
		if (max < fabs(matrix[i][matrixSize])) {
			//cout << "new max is " << matrix[i][matrixSize] << "\n";
			max = fabs(matrix[i][matrixSize]);
		}
	}

	return max;
}

/*
Function: calcTwoNorm
Purpose: calculated two norm of given vector
Param: double matrix: the given matrix, b values are given vector, matrixSize: size of matrix, also the column number of the b vals
Return: NONE
Author: Jason Waid
Date Modified: 03/14/2022
*/
double calcTwoNorm(double matrix[][11], int matrixSize) {

	double norm = 0;

	//sumation of all vals squared
	for (int i = 0; i < matrixSize; i++) {
		
		norm = norm + (fabs(matrix[i][matrixSize]) * fabs(matrix[i][matrixSize]));

	}

	//power of 1/2
	norm = sqrt(norm);


	return norm;
}

/*
Function: performInverseIteration
Purpose: performs inverse iteration
Param: double matrix: the given matrix, matrixSize: size of matrix, also the column number of the b vals aka initial guess
Return: NONE
Author: Jason Waid
Date Modified: 03/14/2022
*/
void performInverseIteration(double matrix[][11], int matrixSize) {

	double errorTolerance = 0.001; //0.1% tollerance
	double currentError = errorTolerance * 10;
	double infiniteNorm_old = 100; //% initial guess %
	double infiniteNorm = 0;
	int iteration = 0;
	double origonalMatrix[10][11] = {};



	//keep copy of origonal matrix in memory
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {

			origonalMatrix[i][j] = matrix[i][j];

		}

	}
	displayInverseIterationTableHeaders();
	displayInverseIterationData(iteration, matrix, infiniteNorm, matrixSize);
	iteration++;

	while (currentError > errorTolerance) {

		performGaussianElimination(matrix, matrixSize);
		performBackwardsSubstitution(matrix, matrixSize);
		//the above is our new initial guess matrix

		//calc inf norm before manipulating array
		infiniteNorm = calcInfiniteNorm(matrix, matrixSize);
			
		//calc new xk
		//where [matrix vals....] | b <-- where b is xk
		for (int i = 0; i < matrixSize; i++) {

			matrix[i][matrixSize] = matrix[i][matrixSize] / infiniteNorm;

		}

		displayInverseIterationData(iteration, matrix, infiniteNorm, matrixSize);
		currentError = (fabs(infiniteNorm - infiniteNorm_old) / infiniteNorm_old) * 100;
		iteration++;
		infiniteNorm_old = infiniteNorm;
		
		//replace matrix with origonal one witrh updated guesses before moving to next iteraton
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {

				matrix[i][j] = origonalMatrix[i][j];

			}

		}
	}
}

/*
Function: calcRayleighQuotient
Purpose: calculated Rayleigh Quotient
Param: double matrix: the given matrix, matrixSize: size of matrix, also the column number of the b vals aka initial guess
Return: NONE
Author: Jason Waid
Date Modified: 03/15/2022
*/
double calcRayleighQuotient(double matrix[][11], int matrixSize) {

	double rayleighQuotient = 0;
	double x[10] = {0};


	//top and bottom of calculation
	double nominatorVector[10] = {0};

	double topVal = 0;
	double bottomVal = 0;

	//get x & xt
	for (int i = 0; i < matrixSize; i++) {

		x[i] = matrix[i][matrixSize];

	}
	
	//calc nominator
	//x^t * A
	//[?, ?,...,?] * A  <- ? x ? size matrix = [?, ?,...,?]
	//cout << "calc nominator\n";
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {

			nominatorVector[i] += x[j] * matrix[j][i];

		}
	}
	//calc nominator cont..
	// * x
	// [?, ?,...,?] * [?, ?,...,?]^t = ?
	//cout << "calc nominator cont..\n";
	for (int i = 0; i < matrixSize; i++) {

		topVal += nominatorVector[i] * x[i];
	}

	//calc denominator
	for (int i = 0; i < matrixSize; i++) {

		bottomVal += x[i] * x[i];

	}

	//calc Rayleigh Quotient
	rayleighQuotient = topVal / bottomVal;

	return rayleighQuotient;

}

/*
Function: performRayleighQuotientIterationWithShifts
Purpose: performs Rayleigh Quotient Iteration
Param: double matrix: the given matrix, matrixSize: size of matrix, also the column number of the b vals aka initial guess
Return: NONE
Author: Jason Waid
Date Modified: 03/15/2022
*/
void performRayleighQuotientIterationWithShifts(double matrix[][11], int matrixSize) {


	double errorTolerance = 0.7; //70% tollerance
	double currentError = errorTolerance * 10;
	double infiniteNorm_old = 100; //% initial guess %
	double infiniteNorm = 0;
	int iteration = 0;
	double rayQuotient = 0;
	double origonalMatrix[10][11] = {};


	//keep copy of origonal matrix in memory
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {

			origonalMatrix[i][j] = matrix[i][j];

		}

	}
	displayRayleighQuotientIterationWithShiftsTableHeaders();

	//initialize RayQuotient
	rayQuotient = calcRayleighQuotient(matrix, matrixSize);
	displayRayleighQuotientIterationWithShiftsTableData(iteration, matrix, infiniteNorm, matrixSize, rayQuotient);

	while (currentError >= errorTolerance) {
		iteration++;

		//shift array
		for (int i = 0; i < matrixSize; i++) {

			/* No need to actually multiply rayQuotient by 1
			[3 1] * [1*rayQuotient 0] = [1 3] * [rayQuotient 0]
			[1 3]	[0 1*rayQuotient]	[3 1]	[0 rayQuotient]
			*/
			matrix[i][i] = matrix[i][i] - rayQuotient;

		}

		//(A - rayleighQuotient*I)y[k] = x[k-1] <- solve for y[k]
		//pass into Gauss elimination to solve for y[k]
		performGaussianElimination(matrix, matrixSize);
		performBackwardsSubstitution(matrix, matrixSize);

		//calc inf norm before manipulating array
		infiniteNorm = calcInfiniteNorm(matrix, matrixSize);

		//calc new xk
		//where [matrix vals....] | b <-- where b is xk
		for (int i = 0; i < matrixSize; i++) {

			matrix[i][matrixSize] = matrix[i][matrixSize] / infiniteNorm;

		}

		rayQuotient = calcRayleighQuotient(matrix, matrixSize);

		displayRayleighQuotientIterationWithShiftsTableData(iteration, matrix, infiniteNorm, matrixSize, rayQuotient);
		currentError = infiniteNorm - infiniteNorm_old;


		infiniteNorm_old = infiniteNorm;

		//replace matrix with origonal one with updated guesses before moving to next iteraton
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {

				matrix[i][j] = origonalMatrix[i][j];

			}

		}
	}
}

/*
Function: performGaussianElimination
Purpose: Performs Gaussian Elimination on the matrix
Param: double matrix[][11] which is the matrix in question, int numRows which are the number of rows, int numColumns which are the number of columns
Return: NONE
Author: Jason Waid
Date Modified: 03/13/2022
*/
void performGaussianElimination(double matrix[][11], int numUnknowns) {
	double ratio = 0;

	for (int k = 0; k < numUnknowns - 1; k++) {
		if (matrix[k][k] == 0) {
			//stop
			exit(0);
		}
		for (int j = k + 1; j < numUnknowns; j++) {
			ratio = matrix[j][k] / matrix[k][k];

			for (int i = k; i < numUnknowns + 1; i++) {
				matrix[j][i] = matrix[j][i] - (ratio * matrix[k][i]);
			}

		}
	}
}

/*
Function: displayInitialMatrixMessage
Purpose: Displays header for initial matrix
Param: NONE
Return: NONE
Author: Jason Waid
Date Modified: 02/06/2022
*/
void displayInitialMatrixMessage() {
	cout << "Initial Matrix\n";
	cout << "-----------------------------------------------------------------------------------------------------------\n";
}

/*
Function: displayInitialGuessMessage
Purpose: Displays header for initial matrix
Param: NONE
Return: NONE
Author: Jason Waid
Date Modified: 03/12/2022
*/
void displayInitialGuessMessage() {
	cout << "Initial Guess\n";
	cout << "-----------------------------------------------------------------------------------------------------------\n";
}


/*
Function: performBackwardsSubstitution
Purpose: Performs backwards substituion on the matrix
Param: double matrix[][] which is the matrix in question, int numUnknowns is the number of unknowns
Return: NONE
Author: Jason Waid
Date Modified: 03/16/2022
*/
void performBackwardsSubstitution(double matrix[][11], int matrixSize) {

	double temp = 0;
	double unknownVals[10] = {0};

	for (int i = 0; i < matrixSize; i++) {
		unknownVals[i] = matrix[i][matrixSize];
	}

	for (int i = matrixSize - 1; i >= 0; i--)
	{

		temp = 0;
		for (int j = i + 1; j < matrixSize; j++) {
			
			temp += matrix[i][j] * unknownVals[j];

		}

		if (matrix[i][i] == 0) {
			unknownVals[i] = 0;
		}
		else {
			unknownVals[i] = (matrix[i][matrixSize] - temp) / matrix[i][i];
		}
		
	}

	for (int i = 0; i < matrixSize; i++) {
		matrix[i][matrixSize] = unknownVals[i];
	}
}

/*
Function: displayInverseIterationData
Purpose: displays inverse iteration row in table
Param: int iteration, double matrix[][]: the matrix in question, int infiniteNorm: the norm val, int matrixSize: size of square matrix
Return: NONE
Author: Jason Waid
Date Modified: 03/15/2022
*/
void displayInverseIterationData(int iteration, double matrix[][11], double infiniteNorm, int matrixSize) {

	cout << iteration << "\t|";

	cout << setprecision(5);

		for (int i = 0; i < matrixSize; i++) {
			cout << setw(10) << setprecision(5) << matrix[i][matrixSize] << "\t";
		}

		cout << "| " << setw(10) << infiniteNorm << "\n";

}



/*
Function: displayRayleighQuotientIterationWithShiftsTableData
Purpose: displays inverse iteration row in table
Param: int iteration, double matrix[][]: the matrix in question, int infiniteNorm: the norm val, int matrixSize: size of square matrix
Return: NONE
Author: Jason Waid
Date Modified: 03/15/2022
*/
void displayRayleighQuotientIterationWithShiftsTableData(int iteration, double matrix[][11], double infiniteNorm, int matrixSize, double rayQuotient) {

	cout << iteration << "\t | ";

	cout  << setprecision(5);

	for (int i = 0; i < matrixSize; i++) {
		cout << setw(10) << matrix[i][matrixSize] << "\t";
	}

	cout << "|\t" << infiniteNorm << "\t|\t" << rayQuotient << "\n";

}
int main()
{
	string inputBuffer;
	int matrixSize = 0;
	int menu = 0;
	//double matrix[10][10 + 1] = {};
	double matrix[10][10 + 1] = { {1,-1,0,1} ,{0,-4,2,3}, {0,0,-2,5} };
	//double matrix[10][10 + 1] = { {3,1,0} ,{1,3,1}};
	matrixSize = 3;
	menu = 2;

	/*QUICKER TESTING COMMENTED OUT
	displayGreeting();
	displayMenu();
	menu = getMenuOption(inputBuffer);
	clearInputBuffer();
	matrixSize = getMatrixSize(inputBuffer);
	clearInputBuffer();
	getMatrixDisplay(inputBuffer, matrixSize, matrix);
	*/
	
	displayInitialMatrixMessage();
	printMatrix(matrix, matrixSize);
	displayInitialGuessMessage();
	printInitialGuess(matrix, matrixSize);

	if (menu == 1) {

		performInverseIteration(matrix, matrixSize);
	}
	else if (menu == 2) {

		performRayleighQuotientIterationWithShifts(matrix, matrixSize);
	}

	return 0;
}
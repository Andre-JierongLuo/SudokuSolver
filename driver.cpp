#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "sudoku.h"
using namespace std;

// Backtracking Algorithm.
Sudoku* solve( Sudoku* puzzle){
	// Method 2:
	puzzle->horSolve();
	// Method 3:
	puzzle->verSolve();
	// Method 4:
	puzzle->boxSolve();
	// Method 1:
	int work = puzzle->finalize();
	// work is -1 means failure.
	// free the puzzle and return NULL.
	if(work == -1){delete puzzle; return NULL;}
	// if puzzle is solved, then just return puzzle.
	else if(puzzle->isSolved()) return puzzle;
	// work is 0 means no more solvable cells.
	else if(work == 0){
		// Create a new puzzle with a cell finailzed to the 
		// guessed candidate and eliminate the guessed
		// candidate from the original puzzle.
		Sudoku * newpuzzle = puzzle->guess();
		// Create a pointer guess point to the result of solve(newpuzzle)
		Sudoku * guess = solve(newpuzzle);
		// if guess is NULL, that means newpuzzle can't be finalized.
		// Then we make another guess for puzzle.
		if(guess == NULL) return solve(puzzle);
		// if newpuzzle is solved, then return newpuzzle.
		if(newpuzzle->isSolved()) return newpuzzle;
		// otherwise, free newpuzzle and return guess.
		delete newpuzzle; return guess;
	}
	// work is 1 means puzzle still need to be finalized until work is 0.
	else return solve(puzzle);
}

// input is the input character.
char input;
// trans_string is the input string for Sudoku Class.
string trans_string;

int main(){
	// We need to collect 81 characters to be the 9 * 9 numbers in Sudoku puzzle.
	// i is the number of character.
	int i = 0;
	while(cin >> input){
		// add input character to tran_string.
		trans_string += input;
		i++;
		// whlie loop break when tran_string has 81 characters.
		if(i == 81) break;
	}
	// Once we finish the collection of characters, we need to print the trans_string 
	// as a Sudoku puzzle.
	// p is the position of the character in trans_string.
	int p = 0;
	while(p < 81){
		// Since there are 9 characters per line, so add '\n' at the end of the 9th character.
		if((p+1) % 9 == 0) cout << trans_string.at(p) << '\n';
		// otherwise, just print that character following by a blank.
		else cout << trans_string.at(p) << " ";
		p++;
	}
	// print a blank line.
	cout << '\n';
	// Create a puzzle with the trans_string.
	Sudoku * puzzle = new Sudoku(trans_string);
	// finalize the puzzle.
	int work = puzzle->finalize();
	// if work is -1, then the puzzle has no solution.
	if(work == -1){
		// print error message to standard error.
		cerr << "ERROR" << endl;
		// free the memory
		delete puzzle;
		// exit the program with a value of 0.
		exit(0);
	}
	// Otherwise, check if the puzzle is solved or not.
	// if puzzle is solved, then print puzzle and free memory.
	if(puzzle->isSolved()){
		cout << *puzzle;
		delete puzzle;
	} else {
		// if puzzle is not solved yet, then use the solve funciton to find
		// out the solution for the puzzle, and print the solution.
		Sudoku * solution = solve(puzzle);
		cout << *solution;
		// Once the program print the solution, then free all the allocated memory.
		delete solution;
		delete puzzle;
	}
	return 0;
}

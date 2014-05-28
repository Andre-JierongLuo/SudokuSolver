#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "sudoku.h"

using namespace std;

Sudoku* solve( Sudoku* puzzle){
	puzzle->horSolve();
	puzzle->verSolve();
	puzzle->boxSolve();
	int work = puzzle->finalize();
	if(work == -1){delete puzzle; return NULL;}
	else if(puzzle->isSolved()) return puzzle;
	else if(work == 0){
		Sudoku * newpuzzle = puzzle->guess();
		Sudoku * guess = solve(newpuzzle);
		if(guess == NULL) return solve(puzzle);
		if(newpuzzle->isSolved()) return newpuzzle;
		delete newpuzzle; return guess;
	}
	else return solve(puzzle);
}



char a;
string b;

int main(){
	int i = 0;
	while(cin >> a){
		b += a;
		i++;
		if(i == 81) break;
	}
	i = 0;
	while(i < 81){
		if((i+1) % 9 == 0) cout << b.at(i) << '\n';
		else cout << b.at(i) << " ";
		i++;
	}
	cout << '\n';
	Sudoku * A = new Sudoku(b);
	int s = A->finalize();
	if(s == -1){
		cerr << "ERROR" << endl;
		delete A;
		exit(0);
	}
	if(A->isSolved()){
		cout << *A;
		delete A;
	} else {
		Sudoku * k = solve(A);
		cout << *k;
		delete k;
		delete A;
	}

	return 0;
}


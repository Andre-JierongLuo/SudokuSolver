#include <iostream>
#include <string>
#include "sudoku.h"
using namespace std;

bool Sudoku::eliminate(int row, int col, int val){
    // check row:
    for(int c = 0; c < 9; c++){
        // excluding the cell at the given coordinates.
        if(c == col-1) continue;
        // if the cell doesn't has two candidates,
        if(puzzle[row-1][c]->possibilities() != 2){
            // fail to remove the val from the cell, then return false;
            if(!puzzle[row-1][c]->removeCandidate(val)) return false;
        } else {
        // if the cell has two candidates, check if the val can be removed from the cell successfully.
            if(puzzle[row-1][c]->removeCandidate(val)){
                // after removing the val from the cell, the cell has only one candidate now, then add it to finalStack.
                if(puzzle[row-1][c]->possibilities() == 1){
                    SolvedCell * tempCell = new SolvedCell(row,c+1,puzzle[row-1][c]->first());
                    finalStack.push(tempCell);
                }
            } else {
                // fail to remove the val, then return false;
                return false;
            }
        }
    }
    
    // check column:
    for(int r = 0; r < 9; r++){
        // excluding the cell at the given coordinates.
        if(r == row-1) continue;
        // if the cell doesn't has two candidates.
        if(puzzle[r][col-1]->possibilities() != 2){
            // fail to remove the val from the cell, then return false;
            if(!puzzle[r][col-1]->removeCandidate(val)) return false;
        } else {
        // if the cell has two candidates, check if the val can be removed from the cell successfully.
            if(puzzle[r][col-1]->removeCandidate(val)){
                // after removing the val from the cell, the cell has only one candidate now, then add it to finalStack.
                if(puzzle[r][col-1]->possibilities() == 1){
                    SolvedCell * tempCell = new SolvedCell(r+1,col,puzzle[r][col-1]->first());
                    finalStack.push(tempCell);
                }
            } else {
                // fail to remove the val, then return false;
                return false;
            }
        }
    }
    // check box:
    int rr = (row-1) % 3, cc = (col-1) % 3;
    for(int pr = (row-rr-1); pr < (row+2-rr); pr++){
    	for(int pc = (col-cc-1); pc < (col+(2-cc)); pc++){
		// Since the row check, and col check already check the cells that in the same row and col. So, we don't need to check those cells in the same row and col again.
            if(pr == row-1 || pc == col-1) continue;
			// if the cell doesn't has two candidates.
            if(puzzle[pr][pc]->possibilities() != 2){
		// fail to remove the val from the cell, then return false;
                if(!puzzle[pr][pc]->removeCandidate(val)) return false;
            } else {
			// if the cell has two candidates, check if the val can be removed from the cell successfully.
                if(puzzle[pr][pc]->removeCandidate(val)){
					// after removing the val from the cell, the cell has only one candidate now, then add it to finalStack.
                    if(puzzle[pr][pc]->possibilities() == 1){
                        SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,puzzle[pr][pc]->first());
                        finalStack.push(tempCell);
                    }
                } else {
				// fail to remove the val, then return false;
                    return false;
                }
            }
        }
    }
    return true;
}


Sudoku::Sudoku(){
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){
            puzzle[r][c] = new Cell;
        }
    }
}

Sudoku::Sudoku(string input){
    int i = 0;
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){
			// a char - '0' will will give a number.
            int num = input.at(i) - '0';
			// if the given number is in [1,9], then that cell is a finalized cell.
            if(1 <= num && num <= 9){
                puzzle[r][c] = new Cell;
                SolvedCell * tempCell = new SolvedCell(r+1,c+1,num);
                finalStack.push(tempCell);
            } else {
			// otherwise, it is a default cell.
                puzzle[r][c] = new Cell;
            }
            i++;
        }
    }
}

Sudoku::Sudoku(const Sudoku& other){
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){
            int count = other.puzzle[r][c]->possibilities();
			// if that cell in other only contain one candidate, then that is a finalized cell.
            if(count == 1){
                int v = other.puzzle[r][c]->first();
		puzzle[r][c] = new Cell(v);
            } else {
			// otherwise, remove all candidates that the cell in other doesn't have.
                puzzle[r][c] = new Cell;
                for(int can = 1; can < 10; can++){
                    if(!other.puzzle[r][c]->at(can)) puzzle[r][c]->removeCandidate(can);
                }
            }
        }
    }
}

Sudoku::~Sudoku(){
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			delete puzzle[r][c];
		}
	}
	while(!finalStack.empty()){
		delete finalStack.top();
		finalStack.pop();
	}
}

Sudoku* Sudoku::guess(){
	Sudoku* temp = new Sudoku(*this);
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			int count = puzzle[r][c]->possibilities();
			if(count == 2){
				int v = puzzle[r][c]->first();
				SolvedCell * tempCell1 = new SolvedCell(r+1,c+1,v);
				temp->finalStack.push(tempCell1);
				puzzle[r][c]->removeCandidate(v);
				int v2 = puzzle[r][c]->first();
				SolvedCell * tempCell2 = new SolvedCell(r+1,c+1,v2);
				finalStack.push(tempCell2);
				return temp;
			}
			if(count > 2){
				int v = puzzle[r][c]->first();
				SolvedCell * tempCell1 = new SolvedCell(r+1,c+1,v);
				temp->finalStack.push(tempCell1);
				puzzle[r][c]->removeCandidate(v);
				return temp;
			}
		}
	}
}

void Sudoku::horSolve(){
	// use for loop to check every row.
	for(int r = 0; r < 9; r++){
		// v is the value;
		for(int v = 1; v < 10; v++){
			// p is the col position.
			int p = -1;
			// count is the # of times that v appear in the same row of cols.
			int count = 0;
			// use for loop to check every col in the same row.
			for(int c = 0; c < 9; c++){
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1) break;
				if(puzzle[r][c]->at(v) && count == 0){count++; p = c; continue;}
				if(puzzle[r][c]->at(v) && count != 0) count++;
				if(count == 2) break;
			}
			if(count == 1){
				SolvedCell * tempCell = new SolvedCell(r+1,p+1,v);
				finalStack.push(tempCell);
			}
		}
	}
}

void Sudoku::verSolve(){
	// use for loop to check every col.
	for(int c = 0; c < 9; c++){
		// v is the value;
		for(int v = 1; v < 10; v++){
			// p is the row position.
			int p = -1;
			// count is the # of times that v appear in the same col of rows.
			int count = 0;
			// use for loop to check every col in the same row.
			for(int r = 0; r < 9; r++){
				// if puzzle[r][c] is a finalized cell that contain v, then just break the loop.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1) break;
				// if puzzle[r][c] has v, and count is zero.
				// count increase by 1, let p be r, and continue.
				if(puzzle[r][c]->at(v) && count == 0){
					count++;
					p = r;
					continue;
				}
				// if puzzle[r][c] has v and count is not zero, count just add 1.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is 2 then we break the for loop.
				if(count == 2) break;
			}
			// Once the for loop is done, check count.
			// if count is 1, that means v only appear in 1 cell in the same row.
			if(count == 1){
				SolvedCell * tempCell = new SolvedCell(p+1,c+1,v);
				finalStack.push(tempCell);
			}
		}
	}
}

void Sudoku::boxSolve(){
	//box1:
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 0, pc = 0;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 2 && pc == 2){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 2 ){ 
								pc++;
								continue;
							}
							if( pc == 2 ){
								pc = 0;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box2:
	for(int r = 0; r < 3; r++){
		for(int c = 3; c < 6; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 0, pc = 3;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 2 && pc == 5){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 5 ){ 
								pc++;
								continue;
							}
							if( pc == 5 ){
								pc = 3;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box3:
	for(int r = 0; r < 3; r++){
		for(int c = 6; c < 9; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 0, pc = 6;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 2 && pc == 8){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 8 ){ 
								pc++;
								continue;
							}
							if( pc == 8 ){
								pc = 6;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box4:
	for(int r = 3; r < 6; r++){
		for(int c = 0; c < 3; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 3, pc = 0;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 5 && pc == 2){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 2 ){ 
								pc++;
								continue;
							}
							if( pc == 2 ){
								pc = 0;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box5:
	for(int r = 3; r < 6; r++){
		for(int c = 3; c < 6; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 3, pc = 3;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 5 && pc == 5){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 5 ){ 
								pc++;
								continue;
							}
							if( pc == 5 ){
								pc = 3;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box6:
	for(int r = 3; r < 6; r++){
		for(int c = 6; c < 9; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 3, pc = 6;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 5 && pc == 8){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 8 ){ 
								pc++;
								continue;
							}
							if( pc == 8 ){
								pc = 6;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box7:
	for(int r = 6; r < 9; r++){
		for(int c = 0; c < 3; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 6, pc = 0;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 8 && pc == 2){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 2 ){ 
								pc++;
								continue;
							}
							if( pc == 2 ){
								pc = 0;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box8:
	for(int r = 6; r < 8; r++){
		for(int c = 3; c < 6; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 6, pc = 3;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 8 && pc == 5){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 5 ){ 
								pc++;
								continue;
							}
							if( pc == 5 ){
								pc = 3;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
	//box9:
	for(int r = 6; r < 9; r++){
		for(int c = 6; c < 9; c++){
			if(puzzle[r][c]->possibilities() == 1) continue;
			for(int can = 1; can < 10; can++){
				if(puzzle[r][c]->at(can)){
					int a = finalStack.size();
					int pr = 6, pc = 6;
					while(true){
						if(!puzzle[pr][pc]->at(can) || (puzzle[pr][pc]->at(can) && pr == r && pc == c)){
							if( pr == 8 && pc == 8){
								SolvedCell * temp = new SolvedCell(r + 1, c + 1, can);
								finalStack.push(temp);
								break;
							}
							if( pc < 8 ){ 
								pc++;
								continue;
							}
							if( pc == 8 ){
								pc = 6;
								pr++;
							}
						} else {
							break;
						}
					}
					if(a + 1 == finalStack.size()) break;
				}
			}
		}
	}
}


int Sudoku::finalize(){
	if(finalStack.empty()) return 0;
	while(!finalStack.empty()){
		int r = finalStack.top()->row, c = finalStack.top()->col, v = finalStack.top()->val;
		delete puzzle[r-1][c-1];
		puzzle[r-1][c-1] =  new Cell(v);
		delete finalStack.top();
		finalStack.pop();
		if(!eliminate(r,c,v)){
			return -1;
		}
	}
	return 1;
}

bool Sudoku::isSolved(){
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			if(puzzle[r][c]->possibilities() != 1) return false;
		}
	}
	for(int v = 1; v < 10; v++){
		for(int r = 0; r < 9; r++){
			int count1 = 0;
			for(int c = 0; c < 9; c++){
				if(puzzle[r][c]->at(v)) count1++;
				if(count1 > 1) return false;
			}
		}
		for(int c = 0; c < 9; c++){
			int count2 = 0;
			for(int r = 0; r < 9; r++){
				if(puzzle[r][c]->at(v)) count2++;
				if(count2 > 1) return false;
			}
		}
	}
	return true;
}

ostream & operator<<(ostream & stream, const Sudoku & puzzleState){
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			if(c+1 == 9) stream << puzzleState.puzzle[r][c]->first() << endl;
			else stream << puzzleState.puzzle[r][c]->first() << " ";
		}
	}
}


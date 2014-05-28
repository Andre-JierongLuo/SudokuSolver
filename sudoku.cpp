#include <iostream>
#include <string>
#include "sudoku.h"
using namespace std;

// eliminate:
bool Sudoku::eliminate(int row, int col, int val){
    // Check row:
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
                // after removing the val from the cell, if the cell has only one candidate now, then add it to finalStack.
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
    
    // Check column:
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
                // after removing the val from the cell, if the cell has only one candidate now, then add it to finalStack.
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
    
    // Check box:
    // rr is the position of row of given coordinate in its box.
    // cc is the position of col of given coordinate in its box.
	// for example, if  the given coordinate is row 4, col 5 in the puzzle.
	// then it belongs to box 5. rr = ( 4 - 1 ) % 3 = 0 , cc = ( 5 - 1 ) % 3 = 1
	// so, its position in box 5 is (0,1).
    int rr = (row-1) % 3, cc = (col-1) % 3;
    // for the example above, the position of box 5 in the puzzle should be row 3 ~ 5, col 3 ~ 5.
    // then pr and pc are the started point of box 5.
    // pr = row - rr - 1 = 4 - 0 - 1 = 3   ;    pc = col - cc - 1 = 5 - 1 - 1 = 3
    // also, pr should be stop at 5, so pr < ( row - rr + 2 ) = 4 - 0 + 2 = 6
    //       pc should be stop at 5, so pc < ( col - cc + 2 ) = 5 - 1 + 2 = 6
    
    // therefore, the for loop for rows should be " for(int pr = 3; pr < 6; pr++) "
    for(int pr = (row-rr-1); pr < (row-rr+2); pr++){
    	// the for loop for cols should be " for(int pc = 3; pc < 6; pc++) "
    	for(int pc = (col-cc-1); pc < (col-cc+2); pc++){
			// Since the row check, and col check already check the cells that in the same row and col. 
			// So, we don't need to check those cells in the same row and col again.
            if(pr == row-1 || pc == col-1) continue;
			// if the cell doesn't has two candidates.
            if(puzzle[pr][pc]->possibilities() != 2){
				// fail to remove the val from the cell, then return false;
                if(!puzzle[pr][pc]->removeCandidate(val)) return false;
            } else {
				// if the cell has two candidates, check if the val can be removed from the cell successfully.
                if(puzzle[pr][pc]->removeCandidate(val)){
					// after removing the val from the cell, if the cell has only one candidate now, then add it to finalStack.
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
    // if there is no failure during the row check, col check, and box check, then return true.
    return true;
}

// default constructor:
Sudoku::Sudoku(){
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){
        	// just create a new cell for each position.
            puzzle[r][c] = new Cell;
        }
    }
}

// constructor:
Sudoku::Sudoku(string input){
	// i is the position of the character in input.
    int i = 0;
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){
			// a character - '0' will give a number.
            int num = input.at(i) - '0';
			// if the given number is in [1,9], then that cell is a finalized cell.
            if(1 <= num && num <= 9){
            	// create a new cell
                puzzle[r][c] = new Cell;
                // create a new SolvedCell for this cell, and add it to finalStack.
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

// copy constructor:
Sudoku::Sudoku(const Sudoku& other){
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){
            int count = other.puzzle[r][c]->possibilities();
			// if that cell in other only contain one candidate, then that is a solved cell.
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

// destructor:
Sudoku::~Sudoku(){
	// delete each cell in the puzzle
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			delete puzzle[r][c];
		}
	}
	// delete all of the remaining SolvedCell in finalStack.
	while(!finalStack.empty()){
		delete finalStack.top();
		finalStack.pop();
	}
}

// guess:
Sudoku* Sudoku::guess(){
	// Create a new temporary puzzle that is same as this->puzzle.
	Sudoku* temp = new Sudoku(*this);
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			// find the first Cell which has two or more candidates.
			int count = puzzle[r][c]->possibilities();
			// if that cell has two candidates:
			if(count == 2){
				// finalize that cell in temp with the smallest candidate, and remove that candidate from the cell in this.
				int v1 = puzzle[r][c]->first();
				SolvedCell * tempCell1 = new SolvedCell(r+1,c+1,v1);
				temp->finalStack.push(tempCell1);
				puzzle[r][c]->removeCandidate(v1);
				// after removing the smallest value, the cell in this only has one candidate now.
				// So it is a solvable cell now. We add that to finalStack.
				int v2 = puzzle[r][c]->first();
				SolvedCell * tempCell2 = new SolvedCell(r+1,c+1,v2);
				finalStack.push(tempCell2);
				return temp;
			}
			// if that cell has more than two candidates:
			if(count > 2){
				// finalize that cell in temp with the smallest candidate, and remove that candidate from the cell in this.
				int v = puzzle[r][c]->first();
				SolvedCell * tempCell1 = new SolvedCell(r+1,c+1,v);
				temp->finalStack.push(tempCell1);
				puzzle[r][c]->removeCandidate(v);
				return temp;
			}
		}
	}
}

// horSolve: A cell with a uniquie candidate, in a row, is solvable.
//           horSolve finds such cells and adds them to finalStack
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
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next row.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1) break;
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){count++; p = c; continue;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// After checking all cells in the same row, if count is one, that means
			// an unsolved cell has v in a row, and the other cells in the same row don't have v.
			// As a result, we finalize that cell with candidate v.
			if(count == 1){
				SolvedCell * tempCell = new SolvedCell(r+1,p+1,v);
				finalStack.push(tempCell);
			}
		}
	}
}

// verSolve: A cell with a uniquie candidate, in a colomn, is solvable.
//	         verSolve finds such cells and adds them to finalStack
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
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next row.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1) break;
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){count++; p = r; continue;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// After checking all cells in the same col, if count is one, that means
			// an unsolved cell has v in a col, and the other cells in the same col don't have v.
			// As a result, we finalize that cell with candidate v.
			if(count == 1){
				SolvedCell * tempCell = new SolvedCell(p+1,c+1,v);
				finalStack.push(tempCell);
			}
		}
	}
}

// boxSolve: A cell with a uniquie candidate, in a box, is solvable.
//	         boxSolve finds such cells and adds them to finalStack
void Sudoku::boxSolve(){
	for(int v = 1; v < 10; v++){
		// count is the # of times that v appear in then same box.
		int count = 0;
		// pr and pc is the position of row and col.
		int pr = -1, pc = -1;
		// when finded = 1, that means we find a solved cell with v as its candidate.
		int finded = 0;
		// Box 1:
		for(int r = 0; r < 3; r++){
			for(int c = 0; c < 3; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 2:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 0; r < 3; r++){
			for(int c = 3; c < 6; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 3:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 0; r < 3; r++){
			for(int c = 6; c < 9; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 4:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 3; r < 6; r++){
			for(int c = 0; c < 3; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 5:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 3; r < 6; r++){
			for(int c = 3; c < 6; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 6:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 3; r < 6; r++){
			for(int c = 6; c < 9; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 7:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 6; r < 9; r++){
			for(int c = 0; c < 3; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 8:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 6; r < 9; r++){
			for(int c = 3; c < 6; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
		
		// Box 9:
		count = 0, pr = -1, pc = -1, finded = 0; 
		for(int r = 6; r < 9; r++){
			for(int c = 6; c < 9; c++){
				// if a cell has v, and it is a solved cell, then we just break the for loop and check the next box.
				if(puzzle[r][c]->at(v) && puzzle[r][c]->possibilities() == 1){finded = 1; break;}
				// if a cell has v, and v doesn't appear in the other checked cells yet,
				// then count increases one, record the position of that cell and check the next cell.
				if(puzzle[r][c]->at(v) && count == 0){pr = r; pc = c; count++;}
				// if a cell has v, and v already appear in the other checked cells,
				// then count increases one.
				if(puzzle[r][c]->at(v) && count != 0) count++;
				// if count is two, then break the loop.
				if(count == 2) break;
			}
			// if finded = 1, then check the next box.
			if(finded == 1) break;
			// if count is two, then break the loop.
			if(count == 2) break;
		}
		// After checking all cells in the same box, if count is one, that means
		// an unsolved cell has v in a col, and the other cells in the same box don't have v.
		// As a result, we finalize that cell with candidate v.
		if(count == 1){
			SolvedCell * tempCell = new SolvedCell(pr+1,pc+1,v);
			finalStack.push(tempCell);
		}
	}
}

// finalize:
int Sudoku::finalize(){
	// if there is no solvable cell in finalStack, then return 0.
	if(finalStack.empty()) return 0;
	// if there are some solvable cells in finalStack, use while loop to finalize those cells.
	while(!finalStack.empty()){
		// r is the row #, c is the col #, and v is the candidate.
		int r = finalStack.top()->row, c = finalStack.top()->col, v = finalStack.top()->val;
		// delete the memory at that position in the puzzle first.
		delete puzzle[r-1][c-1];
		// and create a new solved cell at that position.
		puzzle[r-1][c-1] =  new Cell(v);
		// since we already have the information of the solvable cell, we can delete the memory
		// of that solvabe cell, and pop it from the finalStack.
		delete finalStack.top();
		finalStack.pop();
		// call eliminate, if eliminate fails, then return -1.
		if(!eliminate(r,c,v)){
			return -1;
		}
	}
	// if all solvable cells in finalStack were finalized, then return 1.
	return 1;
}

// isSolved:
bool Sudoku::isSolved(){
	// check every cell in puzzle, if there is a cell with more than one candidate,
	// then the puzzle is not solved.
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			if(puzzle[r][c]->possibilities() != 1) return false;
		}
	}
	// if all cells in the puzzle have only one candidate, then the puzzle is solved.
	return true;
}

// operator << :
ostream & operator<<(ostream & stream, const Sudoku & puzzleState){
	for(int r = 0; r < 9; r++){
		for(int c = 0; c < 9; c++){
			// just print the first candidate of every cell, and start a new line
			// when there are nine number in a row.
			if(c+1 == 9) stream << puzzleState.puzzle[r][c]->first() << endl;
			else stream << puzzleState.puzzle[r][c]->first() << " ";
		}
	}
}

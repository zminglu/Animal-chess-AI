#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <stdlib.h> 
#include <time.h>
using namespace std;

#define row 6
#define column 5

/*
	blue: 10 + piece.num	//pc
	red: piece.num		//player
	hiyoko 1
	niwatori 2
	zou 3
	kirin 4
	lion 5

*/


	int board[row][column] = {
		{ -1,-1,-1,-1,-1 },
		{ -1,14,15,13,-1 },	//blue: zou, lion, kirin
		{ -1, 0,11,0 ,-1 },	//blue: none, hiyoko, none
		{ -1, 0, 1,0 ,-1 },
		{ -1, 3, 5,4 ,-1 },
		{ -1,-1,-1,-1,-1 }
	};

	int hiyoko[3][3] = { { 0,1,0 },{ 0,0,0 },{ 0,0,0 } };
	int niwatori[3][3] = { { 1,1,1 },{ 1,0,1 },{ 0,1,0 } };
	int zou[3][3] = { { 1,0,1 },{ 0,0,0 },{ 1,0,1 } };
	int kirin[3][3] = { { 0,1,0 },{ 1,0,1 },{ 0,1,0 } };
	int lion[3][3] = { { 1,1,1 },{ 1,0,1 },{ 1,1,1 } };	
	//moving rule of each piece

	int red_stocks[6] = { 0,1,0,1,1,1 };
	int blue_stocks[6] = { 0,1,0,1,1,1 };

	int from[2], to[2], type; //type including whether red or blue side 1-5 or 11-15

	struct piece {//the struct for calculating movable pieces
		int stype;
		int sfrom[2];
		int sto[2];
		int pt;
	};

	
int move_judge(int *from, int *to, int type) {
	int row_move = from[0] - to[0];	//>0 means move upward
	int column_move = from[1] - to[1];	//>0 means move to left
	int piecemove[6][3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			piecemove[1][i][j] = hiyoko[i][j];
			piecemove[2][i][j] = niwatori[i][j];
			piecemove[3][i][j] = zou[i][j];
			piecemove[4][i][j] = kirin[i][j];
			piecemove[5][i][j] = lion[i][j];
		}

	if (to[0] == 0 || to[0] == 5 || to[1] == 0 || to[1] == 4)	return 0;	//wall is illegal
	else if (from[0] == -1)		return 1;//outside the board
	else if (board[from[0]][from[1]] != type)	return 0;	//wrong input
	else if (board[to[0]][to[1]] > 0 && -4 <= type - board[to[0]][to[1]] && type - board[to[0]][to[1]] <= 4)	return 0;   //your own piece
	else if (type <= 5)			return piecemove[type][1 - row_move][1 - column_move];
	else						return piecemove[type - 10][1 + row_move][1 + column_move];

	/*else {
		switch (type) {
		case 1:
			return hiyoko[1 - row_move][1 - column_move];
			break;
		case 11:
			return hiyoko[1 + row_move][1 + column_move];
			break;
		case 2:
			return niwatori[1 - row_move][1 - column_move];
			break;
		case 12:
			return niwatori[1 + row_move][1 + column_move];
			break;
		case 3:
			return zou[1 - row_move][1 - column_move];
			break;
		case 13:
			return zou[1 + row_move][1 + column_move];
			break;
		case 4:
			return kirin[1 - row_move][1 - column_move];
			break;
		case 14:
			return kirin[1 + row_move][1 + column_move];
			break;
		case 5:
			return lion[1 - row_move][1 - column_move];
			break;
		case 15:
			return lion[1 + row_move][1 + column_move];
			break;
		}
	
	}*/

}	//to judge if this moving is legal

int check() {
	if (blue_stocks[5] == 0)			return 1;
	else if (red_stocks[5] == 0)		return 11;
	else if (board[1][1] == 5 || board[1][2] == 5 || board[1][3] == 5)	return 2;
	else if (board[4][1] == 15 || board[4][2] == 15 || board[4][3] == 15)	return 12;
	else return 0;
}


//江上くんの部分、今は適度に書きました。
int get_score(int *stock) {
	int score = 0;
	score = stock[1] + stock[2] * 8 + stock[3] * 5 + stock[4] * 5 + stock[5] * 50;
	return score;
}


void print() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 3; j++)
			cout << board[i][j] << " ";
		cout << endl;
		cout << endl;
	}
	//cout << "All left pieces including both on board and out board:" << endl;
	cout << "Player pieces:" << endl;
	cout << "  hiyoko:" << red_stocks[1] << "  niwatori:" << red_stocks[2] << "  zou:" << red_stocks[3] << "  kirin:" << red_stocks[4] << "  lion:" << red_stocks[5] << endl;
	cout << "PC pieces:" << endl;
	cout << "  hiyoko:" << blue_stocks[1] << "  niwatori:" << blue_stocks[2] << "  zou:" << blue_stocks[3] << "  kirin:" << blue_stocks[4] << "  lion:" << blue_stocks[5] << endl;
	cout << endl;
}



piece* rpiece_calc() {//get the data of the current board and stocked piece 
	
	int stock[6];
	for (int i = 1; i <= 5; i++)	stock[i] = red_stocks[i];
	piece* temp = new piece[61];
	for (int i = 0; i <= 60; i++) {
		temp[i].stype = 0;	temp[i].sfrom[0] = 0;	temp[i].sfrom[1] = 0;	temp[i].sto[0] = 0;		temp[i].sto[1] = 0;	
	}
	int num = 0;


	//Firstly calculating the red piece on the board
	for (int i = 1; i <= 4; i++)
	for (int j = 1; j <= 3; j++) {
		if (board[i][j] >= 1 && board[i][j] <= 5) {
			for (int x = i - 1; x <= i + 1; x++)
			for (int y = j - 1; y <= j + 1; y++) {
				from[0] = i;	from[1] = j;	to[0] = x;	to[1] = y;
				if (move_judge(from, to, board[i][j]) == 1) {
					temp[num].stype = board[i][j];	
					temp[num].sfrom[0] = i;	temp[num].sfrom[1] = j;
					temp[num].sto[0] = x; temp[num].sto[1] = y;
					num++;	
				}
			}
			stock[board[i][j]]--;
		}		
	}
	
	//Red piece outside board
	for (int i = 1; i <= 5; i++) {
		if (stock[i] != 0) {
			for (int x = 1; x <= 4; x++)
			for (int y = 1; y <= 3; y++) {
				if (board[x][y] == 0) {
					temp[num].stype = i;
					temp[num].sfrom[0] = -1;	temp[num].sfrom[1] = -1;	//from outside of the board
					temp[num].sto[0] = x; temp[num].sto[1] = y;
					num++;
				}
			}
		}
	}//no matter how many pieces left for each type, the case is the same

	return temp;
}

piece* bpiece_calc() {//get the data of the current board and stocked piece 

	int stock[6];
	for (int i = 1; i <= 5; i++)	stock[i] = blue_stocks[i];
	piece* temp = new piece[61];
	for (int i = 0; i <= 60; i++) {
		temp[i].stype = 0;	temp[i].sfrom[0] = 0;	temp[i].sfrom[1] = 0;	temp[i].sto[0] = 0;		temp[i].sto[1] = 0;	
	}
	int num = 0;


	//Firstly calculating the blue piece on the board
	for (int i = 1; i <= 4; i++)
	for (int j = 1; j <= 3; j++) {
		if (board[i][j] >= 11 && board[i][j] <= 15) {
			for (int x = i - 1; x <= i + 1; x++)
			for (int y = j - 1; y <= j + 1; y++) {
				from[0] = i;	from[1] = j;	to[0] = x;	to[1] = y;
				if (move_judge(from, to, board[i][j]) == 1) {
					temp[num].stype = board[i][j];
					temp[num].sfrom[0] = i;	temp[num].sfrom[1] = j;
					temp[num].sto[0] = x; temp[num].sto[1] = y;
					num++;
				}
			}
		stock[board[i][j]-10]--;
		}
	}

	//Blue piece outside board
	for (int i = 1; i <= 5; i++) {
		if (stock[i] != 0) {
			for (int x = 1; x <= 4; x++)
			for (int y = 1; y <= 3; y++) {
				if (board[x][y] == 0) {
					temp[num].stype = i + 10;
					temp[num].sfrom[0] = -1;	temp[num].sfrom[1] = -1;	//from outside of the board
					temp[num].sto[0] = x; temp[num].sto[1] = y;
					num++;
				}
			}
		}
	}//no matter how many pieces left for each type, the case is the same

	return temp;
}



int bestnum;
int dp3_pt[61];
int backup[3] = { 0,0,0 };


void domove(int type, int xfrom, int yfrom, int xto, int yto) {

	//red piece on the board
	if ((type <= 5 && type >= 1) && (xfrom > 0 && yfrom > 0)) {
		if (board[xto][yto] != 0) {
			blue_stocks[board[xto][yto] - 10]--;
			red_stocks[board[xto][yto] - 10]++;
		}
		board[xto][yto] = type;
		board[xfrom][yfrom] = 0;
	}

	//red piece outside the board
	if ((type <= 5 && type >= 1) && (xfrom < 0 && yfrom < 0))
		board[xto][yto] = type;

	if (type == 1 && xto == 1 && xfrom != -1) {//niwatori level up
		board[xto][yto] = 2;
		red_stocks[1]--;	red_stocks[2]++;
	}

	//blue
	if ((type <= 15 && type >= 11) && (xfrom > 0 && yfrom > 0)) {
		if (board[xto][yto] != 0) {
			red_stocks[board[xto][yto]]--;
			blue_stocks[board[xto][yto]]++;
		}
		board[xto][yto] = type;
		board[xfrom][yfrom] = 0;
	}

	if ((type <= 15 && type >= 11) && (xfrom < 0 && yfrom < 0))
		board[xto][yto] = type;

	if (type == 11 && xto == 4 && xfrom != -1) {//niwatori level up
		board[xto][yto] = 12;
		blue_stocks[1]--;	blue_stocks[2]++;
	}
}

void undomove(int type, int undotype, int xfrom, int yfrom, int xto, int yto) {
	//undo red on the board
	if (type == 1 && xto == 1 && xfrom != -1) {
		board[xto][yto] = 1;
		red_stocks[1]++;	red_stocks[2]--;
	}

	if ((type >= 1 && type <= 5) && (xfrom > 0 && yfrom > 0)) {
		if (undotype != 0) {
			red_stocks[undotype - 10]--;
			blue_stocks[undotype - 10]++;
		}
		board[xfrom][yfrom] = type;
		board[xto][yto] = undotype;
	}

	if ((type >= 1 && type <= 5) && (xfrom < 0 && yfrom < 0))
		board[xto][yto] = 0;

	//blue
	if (type == 11 && xto == 4 && xfrom != -1) {
		board[xto][yto] = 11;
		blue_stocks[1]++;	blue_stocks[2]--;
	}

	if ((type >= 11 && type <= 15) && (xfrom > 0 && yfrom > 0)) {
		if (undotype != 0) {
			red_stocks[undotype]++;
			blue_stocks[undotype]--;
		}
		board[xfrom][yfrom] = type;
		board[xto][yto] = undotype;
	}

	if ((type >= 11 && type <= 15) && (xfrom < 0 && yfrom < 0))
		board[xto][yto] = 0;

}


int minsearch(int depth);

int maxsearch(int depth) {
	if (depth == 0)		return get_score(blue_stocks);

	int maxvalue = -1000000, value = 0;
	piece *moves;
	moves = (piece*)bpiece_calc();

	int num = 0;
	while (moves[num].stype != 0) {
		int undotype = board[moves[num].sto[0]][moves[num].sto[1]];
		domove(moves[num].stype, moves[num].sfrom[0], moves[num].sfrom[1], moves[num].sto[0], moves[num].sto[1]);	
		value = minsearch(depth - 1);
		if (depth == 4)	dp3_pt[num] = value;
		undomove(moves[num].stype, undotype, moves[num].sfrom[0], moves[num].sfrom[1], moves[num].sto[0], moves[num].sto[1]);	
		if (value > maxvalue) {
			maxvalue = value;
			if (depth == 4)	bestnum = num;
		}
		num++;
	}
	return maxvalue;
}

int minsearch(int depth) {
	if (depth == 0)		return get_score(red_stocks);

	int minvalue = 1000000, value = 0;
	piece *moves;
	moves = (piece*)rpiece_calc();

	int num = 0;
	while (moves[num].stype != 0) {
		int undotype = board[moves[num].sto[0]][moves[num].sto[1]];
		domove(moves[num].stype, moves[num].sfrom[0], moves[num].sfrom[1], moves[num].sto[0], moves[num].sto[1]);
		value = maxsearch(depth - 1);
		if (depth == 4)	dp3_pt[num] = value;
		undomove(moves[num].stype, undotype, moves[num].sfrom[0], moves[num].sfrom[1], moves[num].sto[0], moves[num].sto[1]);
		if (value < minvalue) {
			minvalue = value;
			if (depth == 4)	bestnum = num;
		}
		num++;
	}
	return minvalue;
}


int main()
{
	int flag, inputflag = 1, first;
	cout << "Player-first, input 1" << endl;
	cout << "PC-first, input 2" << endl;
	cin >> first;	

	print();

	if (first == 2) {//there are only two possible moves on the first step
		srand((unsigned)time(NULL));
		int pos = rand();
		if (pos % 2 == 1) domove(14, 1, 1, 2, 1);
		else domove(11, 2, 2, 3, 2);
		print();
	}

	while (1) {
		while (1) {//make sure the input is legal
			cout << "input your next move, types are from 1 to 5" << endl;
			cout << "type:";	cin >> type;
			cout << "from:";	cin >> from[0] >> from[1];
			cout << "to:";	cin >> to[0] >> to[1];	cout << endl;
			
			inputflag = move_judge(from, to, type);
			if (inputflag == 0)		cout << "This move is illegal, please input the move again" << endl;
			else break;
		}
		//change the board
		domove(type, from[0], from[1], to[0], to[1]);
		print();	

		//check
		flag = check();
		if (flag == 1 || flag == 2) { cout << "Player won" << endl;	break; }
		else if (flag == 11 || flag == 12) { cout << "PC won" << endl;	break; }

		else {//pc operator
			int score = maxsearch(4);

			piece *moves;
			moves = (piece*)bpiece_calc();

			backup[0] = bestnum;	backup[1] = bestnum;	backup[2] = bestnum;
			int k = 1, i = 0;
			while (moves[i].stype != 0 && k <= 2) {
				if (dp3_pt[i] >= (dp3_pt[bestnum] - 5) && i != bestnum) {
					backup[k] = i;		k++;
				}
				i++;
			}

			/*i = 0;
			while (moves[i].stype != 0) {
				cout << i << " " << moves[i].stype << " " << moves[i].sfrom[0] << " " << dp3_pt[i] << endl;
				i++;
			}
			cout << backup[0] << backup[1] << backup[2] << endl;*/

			srand((unsigned)time(NULL));
			int pos = rand();	
			//cout << "pos:" <<pos << endl;
			pos = pos % 3;
			int finalnum = backup[pos];
			//cout << "finalnum:" << finalnum << endl;

			domove(moves[finalnum].stype, moves[finalnum].sfrom[0], moves[finalnum].sfrom[1], moves[finalnum].sto[0], moves[finalnum].sto[1]);
			print();

			flag = check();
			if (flag == 1 || flag == 2) { cout << "Player won" << endl;	break; }
			else if (flag == 11 || flag == 12) { cout << "PC won" << endl;	break; }
			
		}
	}

	system("pause");
    return 0;
}


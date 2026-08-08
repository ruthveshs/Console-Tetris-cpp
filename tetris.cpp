#include<iostream>
#include<windows.h>
#include<conio.h>
using namespace std;

const int ROWS = 20;
const int COLS = 10;

int board[ROWS][COLS] = {0};

int I_piece[4][4] = {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
};
int O_piece[4][4] = {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
int T_piece[4][4] = {
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};
int S_piece[4][4] = {
    {0,0,0,0},
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0}
};
int Z_piece[4][4] = {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};
int J_piece[4][4] = {
    {0,0,0,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0}
};
int L_piece[4][4] = {
    {0,0,0,0},
    {0,1,0,0},
    {0,1,1,1},
    {0,0,0,0}
};

// pointer array so we can pick a random shape easily
int (*allPieces[7])[4] = { I_piece, O_piece, T_piece, S_piece, Z_piece, J_piece, L_piece };

int currentPiece[4][4];
int pieceRow, pieceCol;
int score = 0;
int linesCleared = 0;
bool gameOver = false;

void copyPiece(int source[4][4]){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            currentPiece[i][j] = source[i][j];
}

void spawnPiece(){
    int idx = rand() % 7;
    copyPiece(allPieces[idx]);
    pieceRow = 0;
    pieceCol = 3;
}

bool isValidPosition(int row, int col){
    if(row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return false;
    if(board[row][col] != 0)
        return false;
    return true;
}

bool canPlacePiece(int piece[4][4], int row, int col){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(piece[i][j] != 0){
                int r = row + i;
                int c = col + j;
                // allow cells above the board (still spawning in)
                if(r < 0) continue;
                if(!isValidPosition(r, c))
                    return false;
            }
        }
    }
    return true;
}

void lockPiece(){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(currentPiece[i][j] != 0){
                int r = pieceRow + i;
                int c = pieceCol + j;
                if(r >= 0 && r < ROWS && c >= 0 && c < COLS)
                    board[r][c] = 1;
            }
}

void clearLines(){
    int cleared = 0;
    for(int i=ROWS-1;i>=0;i--){
        bool full = true;
        for(int j=0;j<COLS;j++){
            if(board[i][j] == 0){
                full = false;
                break;
            }
        }
        if(full){
            cleared++;
            // shift every row above down by one
            for(int r=i;r>0;r--){
                for(int c=0;c<COLS;c++){
                    board[r][c] = board[r-1][c];
                }
            }
            for(int c=0;c<COLS;c++) board[0][c] = 0;
            i++; // recheck this row index since rows shifted down into it
        }
    }
    if(cleared > 0){
        linesCleared += cleared;
        score += cleared * 100;
    }
}

void rotatePiece(){
    int rotated[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            rotated[i][j] = currentPiece[3-j][i];

    if(canPlacePiece(rotated, pieceRow, pieceCol)){
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                currentPiece[i][j] = rotated[i][j];
    }
}

void moveLeft(){
    if(canPlacePiece(currentPiece, pieceRow, pieceCol-1))
        pieceCol -= 1;
}

void moveRight(){
    if(canPlacePiece(currentPiece, pieceRow, pieceCol+1))
        pieceCol += 1;
}

void moveDown(){
    if(canPlacePiece(currentPiece, pieceRow+1, pieceCol)){
        pieceRow += 1;
    }
    else{
        lockPiece();
        clearLines();
        spawnPiece();
        if(!canPlacePiece(currentPiece, pieceRow, pieceCol)){
            gameOver = true;
        }
    }
}

void hardDrop(){
    while(canPlacePiece(currentPiece, pieceRow+1, pieceCol)){
        pieceRow += 1;
    }
    moveDown(); // this call will fail the down-check and lock it
}

void gotoTop(){
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor(){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void draw(){
    // build a temporary display grid = board + current falling piece overlaid
    int display[ROWS][COLS];
    for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
            display[i][j] = board[i][j];

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(currentPiece[i][j] != 0){
                int r = pieceRow + i;
                int c = pieceCol + j;
                if(r >= 0 && r < ROWS && c >= 0 && c < COLS)
                    display[r][c] = 1;
            }
        }
    }

    gotoTop();
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++){
            if(display[i][j] != 0)
                cout << "██";
            else
                cout << "░░";
        }
        cout << endl;
    }
    cout << "Score: " << score << "   Lines: " << linesCleared << endl;
    cout << "Controls: A=left  D=right  S=down  W=rotate  Space=hard drop  Q=quit" << endl;
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();
    srand((unsigned)time(0));
    spawnPiece();

    DWORD lastFallTime = GetTickCount();
    const DWORD fallDelay = 700; // ms between automatic drops

    draw();

    while(!gameOver){
        // non-blocking input check
        if(_kbhit()){
            char key = _getch();
            if(key == 'a' || key == 'A') moveLeft();
            else if(key == 'd' || key == 'D') moveRight();
            else if(key == 's' || key == 'S') moveDown();
            else if(key == 'w' || key == 'W') rotatePiece();
            else if(key == ' ') hardDrop();
            else if(key == 'q' || key == 'Q'){ gameOver = true; break; }
            draw();
        }

        // gravity: auto drop every fallDelay ms, independent of key presses
        DWORD now = GetTickCount();
        if(now - lastFallTime >= fallDelay){
            moveDown();
            draw();
            lastFallTime = now;
        }

        Sleep(10); // small sleep to avoid pegging the CPU at 100%
    }

    cout << "GAME OVER. Final score: " << score << endl;
    return 0;
}

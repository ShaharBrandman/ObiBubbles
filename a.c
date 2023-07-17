#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 10
#define COLUMNS 17

struct Bubble {
    int x;
    int y;
    char color;
    bool exists;
    bool sticked;
    struct Bubble* stickedTo;
};

struct Bubble bubbles[ROWS][COLUMNS];

//array of char
char colors[] = {'R', 'G', 'B', 'Y', 'P'};

void findNearestBubble(struct Bubble (*mat)[COLUMNS], int i, int j) {
    //find the nearest bubble to stick to
    //not the first bubble
    if(i != 0) {
        struct Bubble* stickedTo;
        bool doesStick = false;

        //above bubble 
        if (mat[i][j].color == mat[i - 1][j].color) {
            doesStick = true;
            stickedTo = &mat[i - 1][j];  
        }
        //not the first and not the last bubble in a column
        else if(j != COLUMNS - 1 && j != 0){
            //right
            if(mat[i][j].color == mat[i][j+1].color) {
                doesStick = true;
                stickedTo = &mat[i][j + 1];
            }
            //left
            else if(mat[i][j].color == mat[i][j-1].color) {
                doesStick = true;
                stickedTo = &mat[i][j - 1];
            }
        }

        mat[i][j].sticked = doesStick;
        mat[i][j].stickedTo = stickedTo;
    }
    else {
        mat[i][j].sticked = false;
    }
}

void genBubbles(struct Bubble (*mat)[COLUMNS]) {
    srand(time(NULL));
    
    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLUMNS; j++) {
            mat[i][j].x = i;
            mat[i][j].y = j;
            mat[i][j].color = colors[rand() % sizeof(colors) / sizeof(char)];
            mat[i][j].exists = true;

            findNearestBubble(mat, i, j);
        }
    }
}

void displayBubblesIndex(struct Bubble (*mat)[COLUMNS]) {
    for(int i=0; i<ROWS; i++) {
        printf("=%4d -", i);
        for(int j=0; j<COLUMNS; j++) {
            printf("%4d", j);
        }
        printf("=\n");
    }
}

void displayBubblesValues(struct Bubble (*mat)[COLUMNS]) {
    for(int i=0; i<ROWS; i++) {
        printf("=%4d -", i);
        for(int j=0; j<COLUMNS; j++) {
            if(mat[i][j].exists) {
                if(mat[i][j].sticked) {
                    struct Bubble* stickedTo = mat[i][j].stickedTo;
                    if(stickedTo->y == i + 1) {
                        printf("%4c", mat[i][j].color);
                    }
                    else if (stickedTo->y == i - 1) {
                        printf("%4c", mat[i][j].color);
                    }
                    else if(stickedTo->x == j + 1) {
                        printf("%4c", mat[i][j].color);
                    }
                    else if (stickedTo->x == j - 1) {
                        printf("%4c", mat[i][j].color);
                    }
                }
                else {
                    printf("%4c", mat[i][j].color);
                }
            }
            else {
                printf("%4c", "X");
            }
        }
        printf("=\n");
    }
}

void checkForPop(struct Bubble (*mat)[COLUMNS], struct Bubble newBubble) {
    
}

int main() {
    genBubbles(bubbles);
    displayBubblesIndex(bubbles);
    printf("\n");
    displayBubblesValues(bubbles);
}
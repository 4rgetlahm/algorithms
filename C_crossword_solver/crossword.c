#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_X 100
#define MAX_Y 100
#define MAX_PLACEHOLDERS 100
#define MAX_WORD_SIZE 100
#define MAX_WORD_COUNT 100



typedef enum Positioning{
    VERTICAL,
    HORIZONTAL
} Positioning;

typedef struct Placeholder{
    int x, y;
    int size;
    //int possibleWordCount;
    //char possibleWords[MAX_WORD_COUNT][MAX_WORD_SIZE];
    Positioning positioning;
} Placeholder;

void readMatrix(char fileName[], char crosswordMatrix[MAX_Y][MAX_X], int* crosswordX, int* crosswordY){
    FILE* readFile = fopen(fileName, "r");
    if(readFile != NULL){
        for( (*crosswordY) = 0; fscanf(readFile, "%s", crosswordMatrix[(*crosswordY)]) != EOF; ++*(crosswordY) ); // read and find matrix Y
        if(*(crosswordY) > 0){
            (*crosswordX) = 0; //get matrix x;
            for(int i = 0; i != MAX_X; i++){
                if(crosswordMatrix[0][i] == '0' || crosswordMatrix[0][i] == '1'){
                    ++(*crosswordX);
                }
            }
        }
    }
}


void getHorizontalMatrix(char crosswordMatrix[MAX_Y][MAX_X], int crosswordX, int crosswordY){
    for(int y = 0; y != crosswordY; ++y){
        for(int x = 0; x != crosswordX; ++x){
            if((crosswordMatrix[y][x] == '0' && crosswordMatrix[y][x+1] == '1' && crosswordMatrix[y][x-1] == '1')
               || (crosswordMatrix[y][x] == '0' && x+1 >= crosswordX && crosswordMatrix[y][x-1] == '1')
               || (crosswordMatrix[y][x] == '0' && x-1 < 0 && crosswordMatrix[y][x+1] == '1')){
                crosswordMatrix[y][x] = '1';
            }
        }
    }
}

void getVerticalMatrix(char crosswordMatrix[MAX_Y][MAX_X], int crosswordX, int crosswordY){
    for(int y = 0; y != crosswordY; ++y){
        for(int x = 0; x != crosswordX; ++x){
            if(crosswordMatrix[y][x] == '0' && !(crosswordMatrix[y+1][x] == '0' || crosswordMatrix[y-1][x] == '0')){
                crosswordMatrix[y][x] = '1';
            }
        }
    }
}

int getHorizontalPlaceholders(char horizontalMatrix[MAX_Y][MAX_X], Placeholder* horizontalPlaceholders[MAX_PLACEHOLDERS], int crosswordX, int crosswordY){
    int placeholderCount = 0;
    for(int y = 0; y != crosswordY; ++y){
        short foundPlaceholder = 0, placeholderSize = 0;
        for(int x = 0; x != crosswordX; ++x){
            if(horizontalMatrix[y][x] == '0' && !foundPlaceholder){
                foundPlaceholder = 1;
                horizontalPlaceholders[placeholderCount] = (Placeholder*) malloc(sizeof(Placeholder));
                horizontalPlaceholders[placeholderCount]->x = x;
                horizontalPlaceholders[placeholderCount]->y = y;
                horizontalPlaceholders[placeholderCount]->positioning = HORIZONTAL;
            } else if(horizontalMatrix[y][x] == '0' && foundPlaceholder){
                ++placeholderSize;
            } else if(horizontalMatrix[y][x] == '1' && foundPlaceholder){
                horizontalPlaceholders[placeholderCount]->size = ++placeholderSize;
                foundPlaceholder = 0;
                placeholderSize = 0;
                ++placeholderCount;
            }
        }
        if(foundPlaceholder){
            horizontalPlaceholders[placeholderCount]->size = ++placeholderSize;
            foundPlaceholder = 0;
            placeholderSize = 0;
            ++placeholderCount;
        }
    }
    return placeholderCount;
}


int getVerticalPlaceholders(char verticalMatrix[MAX_Y][MAX_X], Placeholder* verticalPlaceholders[MAX_PLACEHOLDERS], int crosswordX, int crosswordY){
    int placeholderCount = 0;
    for(int x = 0; x != crosswordX; ++x){
        short foundPlaceholder = 0, placeholderSize = 0;
        for(int y = 0; y != crosswordY; ++y){
            if(verticalMatrix[y][x] == '0' && !foundPlaceholder){
                foundPlaceholder = 1;
                verticalPlaceholders[placeholderCount] = (Placeholder*) malloc(sizeof(Placeholder));
                verticalPlaceholders[placeholderCount]->x = x;
                verticalPlaceholders[placeholderCount]->y = y;
                verticalPlaceholders[placeholderCount]->positioning = VERTICAL;
            } else if(verticalMatrix[y][x] == '0' && foundPlaceholder){
                ++placeholderSize;
            } else if(verticalMatrix[y][x] == '1' && foundPlaceholder){
                verticalPlaceholders[placeholderCount]->size = ++placeholderSize;
                foundPlaceholder = 0;
                placeholderSize = 0;
                ++placeholderCount;
            }
        }
        if(foundPlaceholder){
            verticalPlaceholders[placeholderCount]->size = ++placeholderSize;
            foundPlaceholder = 0;
            placeholderSize = 0;
            ++placeholderCount;
        }
    }
    return placeholderCount;
}

/*void getPossibleWordListForPlaceholder(Placeholder* placeholder, char wordList[MAX_WORD_COUNT][MAX_WORD_SIZE], int wordCount){
    placeholder->possibleWordCount = 0;
    for(int i = 0; i != wordCount; ++i){
        if(strlen(wordList[i]) == placeholder->size){
            strcpy(placeholder->possibleWords[placeholder->possibleWordCount], wordList[i]);
            ++(placeholder->possibleWordCount);
        }
    }
}*/

short fillWordIntoMatrix(char crosswordMatrix[MAX_Y][MAX_X], Placeholder* placeholder, char word[]){
    if(placeholder->positioning == HORIZONTAL){
        for(int i = 0; i != placeholder->size; ++i){
            if(crosswordMatrix[placeholder->y][(placeholder->x) + i] == '0' || crosswordMatrix[placeholder->y][(placeholder->x) + i] == word[i]){
                crosswordMatrix[placeholder->y][(placeholder->x) + i] = word[i];
            } else {
                return 0;
            }
        }
    } else if(placeholder->positioning == VERTICAL){
        for(int i = 0; i != placeholder->size; ++i){
            if(crosswordMatrix[(placeholder->y) + i][placeholder->x] == '0' || crosswordMatrix[(placeholder->y) + i][placeholder->x] == word[i]){
                crosswordMatrix[(placeholder->y) + i][placeholder->x] = word[i];
            } else {
                return 0;
            }
        }
    }
    return 1;
}

void writeMatrixToFile(char fileName[], char matrix[MAX_Y][MAX_X], int crosswordY){
    FILE* file = fopen(fileName, "w");
    if(file != NULL){
        for(int i = 0; i != crosswordY; ++i){
            fprintf(file, "%s\n", matrix[i]);
        }
    }
    fclose(file);
}

void writeError(char fileName[]){
    FILE* file = fopen(fileName, "w");
    if(file != NULL){
        fprintf(file, "Nepavyko isspresti kryziazodzio");
    }
}


short solve(char currentMap[MAX_Y][MAX_X], Placeholder* allPlaceholders[2 * MAX_PLACEHOLDERS],
            char wordList[MAX_WORD_COUNT][MAX_WORD_SIZE], int wordCount, int currentPlaceholder, int allPlaceholderCount, int crosswordY){

    if(currentPlaceholder == allPlaceholderCount){
        writeMatrixToFile("solvedcrossword.txt", currentMap, crosswordY);
        return 1; // found solution!!!
    }

    char matrixCopy[MAX_Y][MAX_X];
    for(int i = 0; i != wordCount; ++i){
        memcpy(matrixCopy, currentMap, MAX_X * MAX_Y);
        if(strlen(wordList[i]) != allPlaceholders[currentPlaceholder]->size){
            continue;
        }
        if(fillWordIntoMatrix(matrixCopy, allPlaceholders[currentPlaceholder], wordList[i]) &&
            solve(matrixCopy, allPlaceholders, wordList, wordCount, ++currentPlaceholder, allPlaceholderCount, crosswordY)){
                return 1;
        }
    }
    return 0;
}


int readWordList(char fileName[], char wordList[MAX_WORD_COUNT][MAX_WORD_SIZE]){
    FILE* readFile = fopen(fileName, "r");
    int wordCount = 0;
    if(readFile != NULL){
        for(; fscanf(readFile, "%s", wordList[wordCount]) != EOF; ++wordCount );
    }
    fclose(readFile);
    return wordCount;
}


int main(){
    int crosswordX, crosswordY;

    char crosswordMatrix[MAX_Y][MAX_X];
    char horizontalMatrix[MAX_Y][MAX_X];
    char verticalMatrix[MAX_Y][MAX_X];
    char wordList[MAX_WORD_COUNT][MAX_WORD_SIZE];

    readMatrix("crossword_matrix.txt", crosswordMatrix, &crosswordX, &crosswordY);
    int wordCount = readWordList("wordlist.txt", wordList);
    memcpy(horizontalMatrix, crosswordMatrix, sizeof(crosswordMatrix));
    memcpy(verticalMatrix, crosswordMatrix, sizeof(crosswordMatrix));

    getHorizontalMatrix(horizontalMatrix, crosswordX, crosswordY);
    getVerticalMatrix(verticalMatrix, crosswordX, crosswordY);

    Placeholder* horizontalPlaceholders[MAX_PLACEHOLDERS];
    int horizontalPlaceholderCount = getHorizontalPlaceholders(horizontalMatrix, horizontalPlaceholders, crosswordX, crosswordY);

    Placeholder* verticalPlaceholders[MAX_PLACEHOLDERS];
    int verticalPlaceholderCount = getVerticalPlaceholders(verticalMatrix, verticalPlaceholders, crosswordX, crosswordY);

    Placeholder* allPlaceholders[2 * MAX_PLACEHOLDERS];

    for(int i = 0; i != horizontalPlaceholderCount; ++i){
        allPlaceholders[i] = horizontalPlaceholders[i];
    }
    for(int i = 0; i != verticalPlaceholderCount; ++i){
        allPlaceholders[i + horizontalPlaceholderCount] = verticalPlaceholders[i];
    }

    int allPlaceholderCount = horizontalPlaceholderCount + verticalPlaceholderCount;

    if(!solve(crosswordMatrix, allPlaceholders, wordList, wordCount, 0, allPlaceholderCount, crosswordY)){
        writeError("solvedcrossword.txt");
    }


    /*free(horizontalPlaceholders);
    free(verticalPlaceholders);
    free(allPlaceholders);
    free(horizontalMatrix);
    free(verticalMatrix);
    free(crosswordMatrix);*/



    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct {
  char *word; // word and corresponding hint
  char *clue;
  int x; // Starting x and y positions
  int y;
  char direction; // H for horizontal, V for vertical
  int f;          // solved or not
} Word_t;

Word_t *loadTextFile(FILE *myFile, int nrWords);
void displayBoard(int rows, int cols, char **myBoard);
int isBoardFilled(int rows, int cols, char **myBoard);
char **createArray(int rows, int cols);
int findIndex(int arr[], int size, int val);
void playGame(char **myBoard, int wordCount, Word_t *words, int x, int y,
              int countToFinish);
char **updateBoard(char **myBoard, Word_t *words, int solve);

int main(int argc, char const *argv[]) {
  char **board;
  FILE *file;
  int row,column,words;
  Word_t *wordList;
  char fileName[100];

  printf("CMPE252 HW1 PUZZLE GAME\n\n");
  printf("Enter name of text file: " );
  scanf("%s",fileName);
  file= fopen(fileName,"r");

  while(file==NULL){
      printf("Wrong file name\nEnter name of text file: " );
      scanf("%s",fileName);
      file= fopen(fileName,"r");
  }
  fscanf(file,"%d %d %d\n",&row,&column,&words);
  printf("\nGame is %d rows x %d cols with %d words\n",row,column,words );
  wordList=loadTextFile(file,words);
  board=createArray(row,column);
  printf("Words and clues are read!\n" );
  playGame(board,words, wordList, row, column,100);

  //FINISH
  free(board);
  free(wordList);
  return 0;
}

Word_t* loadTextFile(FILE *myFile, int nrWords){
    int size=nrWords*sizeof(Word_t);
  Word_t* words =(Word_t *) malloc(size);
  for(int i=0; i<nrWords;i++){
    char hv;
    int x,y;
    char *kelime = malloc(sizeof(char)*20);
    char *clue = malloc(sizeof(char)*200);
    fscanf(myFile,"%c %d %d %s %[^\n\t]\n",&hv,&x,&y,kelime,clue);
    Word_t w = {kelime,clue,x,y,hv,0};
    words[i]=w;
  }
  return words;
}

  char ** createArray(int rows, int cols){
    char **arr =(char **)malloc(rows * sizeof(char*));
    for (size_t i = 0; i < rows; i++) {
      arr[i]=(char *)malloc(cols * sizeof(char));
    }
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        arr[i][j]='#';
      }
    }
    return arr;
  }
void displayBoard(int rows, int cols, char **myBoard){
  printf("\t ");
  for (int i = 0; i < cols; i++) {
    printf(" %d ",i+1 );
  }
  printf("\n" );
  printf("\t" );
  for (size_t i = 0; i < cols; i++) {
    printf("---" );
  }
  printf("\n");
  for (int i = 0; i < rows; i++) {
    printf("%d\t| ",i+1 );
    for (size_t j = 0; j < cols; j++) {
      printf("%c  ",myBoard[i][j] );
    }
    printf("\n");
  }
  printf("\t" );
  for (size_t i = 0; i < cols; i++) {
    printf("***" );
  }
  printf("\n" );
}
int isBoardFilled(int rows, int cols, char **myBoard){
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if(myBoard[i][j]=='_'){
        return FALSE;
      }
    }
  }
  return TRUE;
}

int findIndex(int arr[], int size, int val){
  for (size_t i = 0; i < size; i++) {
    if(arr[i]==val){
      return TRUE;
    }
  }
  return FALSE;
}

char **updateBoard(char **myBoard, Word_t *words, int solve){
    if(words[solve].f==TRUE){
      if(words[solve].direction=='H'){
        int j=0;
        for (size_t i = words[solve].y-1; i < words[solve].y-1+strlen(words[solve].word); i++) {
          myBoard[words[solve].x-1][i]=words[solve].word[j++];
        }
      }
      else if(words[solve].direction=='V'){
        int j=0;
        for (size_t i = words[solve].x-1; i < words[solve].x-1+strlen(words[solve].word); i++) {
          myBoard[i][words[solve].y-1]=words[solve].word[j++];
        }
      }
    }
    else{
      if(words[solve].direction=='H'){
        for (size_t i = words[solve].y-1; i < words[solve].y-1+strlen(words[solve].word); i++) {
          myBoard[words[solve].x-1][i]='_';
        }
      }
      else if(words[solve].direction=='V'){
        for (size_t i = words[solve].x-1; i < words[solve].x-1+strlen(words[solve].word); i++) {
          myBoard[i][words[solve].y-1]='_';
        }
      }
    }
}
void playGame(char **myBoard, int wordCount, Word_t *words, int x, int y, int countToFinish){
    int count=0;
    for (size_t i = 0; i < wordCount; i++) {
        updateBoard(myBoard,words,i);
    }
    while(count<=countToFinish && isBoardFilled(y,x,myBoard)==FALSE){
        int toSolve;
        char ans[20];
        printf("Current puzzle:\n" );
        displayBoard(y,x,myBoard);

        printf("Ask for hint:\n" );
        printf("#  Direction\trow\tcol\n" );
        printf("---------------------------\n" );
        for (size_t i = 0; i < wordCount; i++) {
            if(words[i].f==FALSE){
                printf("%d: ",i+1 );
                if(words[i].direction=='H'){
                    printf("Horizontal" );
                }
                else{
                    printf("Vertical" );
                }
                printf("\t%d\t%d",words[i].y,words[i].x);
            }
            printf("\n" );
        }
        //displayBoard(y,x,myBoard);
        printf("\nEnter -1 to exit\nWhich word to solve next?: " );
        scanf("%d", &toSolve);
        if(toSolve<0){
            printf("Good Bye!\n" );
            break;
        }
        if(toSolve>wordCount){
            printf("WRONG input\n" );
            continue;
        }
        while(words[toSolve-1].f==TRUE){
            printf("That word has been solved before\n" );
            printf("\nEnter -1 to exit\nWhich word to solve next?: " );
            scanf("%d", &toSolve);
        }
        printf("Current hint: %s\n", words[toSolve-1].clue);
        printf("Enter your solution: " );
        scanf("%s",ans );
        printf("\n\n");
        if(strcasecmp(ans,words[toSolve-1].word)==0){
            words[toSolve-1].f=TRUE;
            updateBoard(myBoard,words,toSolve-1);
            count++;
            if(isBoardFilled(x,y,myBoard)==TRUE){
                displayBoard(y,x,myBoard);
                printf("Congrations! You beat the puzzle!\n");
                break;
            }
        }
        else{
            printf("WRONG ANSWER\n" );
        }

    }

}

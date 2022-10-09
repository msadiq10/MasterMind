/*
 * MasterMind: a cut down version with just the master-mind game logic (purely C) and no external devices

Sample run:
Contents of the sequence (of length 3):  2 1 1
Input seq (len 3): 1 2 3
0 2
Input seq (len 3): 3 2 1
1 1
Input seq (len 3): 2 1 1
3 0
SUCCESS after 3 iterations; secret sequence is  2 1 1

 * Compile:    gcc -o mmt  master-mind-terminal.c
 * Run:        ./mmt

 */

/* --------------------------------------------------------------------------- */

/* Library headers from libc functions */
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/* Constants */
#define  COL  3
#define  LEN  3

/* Global variables */

int colors = COL;
int seqlen = LEN;

static char* color_names[] = { "red", "green", "blue" };

static int* theSeq = NULL;

/* Aux functions */

/* initialise the secret sequence; by default it should be a random sequence, with -s use that sequence */
void initSeq() {
  /* COMPLETE the code here */
  printf("Enter size of sequence (less than 10): ");  /* asking user to set the length of the sequence */
  fscanf(stdin, "%d", &seqlen);                       /* and storing in seqlen */
  printf("Enter number of colours (less than 10): "); /* asking user to set the number of colors for the sequence */
  fscanf(stdin, "%d", &colors);                       /* and storing in colors */

  if(seqlen>=10){                                     /* if sequence length is more than 10, */
    printf("Oops, sequence length is too huge\n");    /* throw error message and exit program */
    exit(0);
  }
  if(colors>=10){                                     /* if number of colors is more than 10, */
    printf("Oops, too many colors\n");                /* throw error message and exit program */
    exit(0);
  }
  theSeq = (int *)malloc(seqlen*sizeof(int));         /* allocating memory for the secret sequence */
  int i = 0;                                          /* counter for iterating purpose */
  srand(time(0));                                     /* seed for random function */
  while(i<seqlen){                                    /* looping through the secret sequence */
    theSeq[i]=rand() % colors + 1;                    /* generating a random number from 1-3 and */
    i+=1;                                             /* storing them in the 'i'th index of the secret sequence */
  }
}

/* display the sequence on the terminal window, using the format from the sample run above */
void showSeq(int *seq) {
  /* COMPLETE the code here */
  printf("Contents of the sequence (of length %d): ", seqlen);
  int i = 0;
  while(i<seqlen){                                    /* iterating through the secret sequence */
    printf(" %d",seq[i]);                             /* and printing out each number in the seqeunce */
    i+=1;
  }
  printf("\n");
}

/* counts how many entries in seq2 match entries in seq1 */
/* returns exact and approximate matches  */
int countMatches(int *seq1, int *seq2) {
  /* COMPLETE the code here */
  int *tempseq1 = NULL;
  tempseq1 = (int *)malloc(seqlen*sizeof(int));       /* allocating memory for temporary array of seq1 */
  int *tempseq2 = NULL;     
  tempseq2 = (int *)malloc(seqlen*sizeof(int));       /* allocating memory for temporary array of seq2 */
  int countMatches=0;                                 /* variable to store the matches */
  int i = 0;      
  while(i<seqlen){      
    tempseq1[i]=seq1[i];                              /* storing elements of seq1 into its temporary array */
  i+=1;     
  }     
  i = 0;      
  while(i<seqlen){      
    tempseq2[i]=seq2[i];                              /* storing elements of seq2 into its temporary array */
  i+=1;     
  }     
  i = 0;      
  while(i<seqlen){                                    /* looking for an exact match */
    if(tempseq1[i]==tempseq2[i]){                     /* if we find an exact match */
      countMatches+=10;                               /* increment the matches by 10 (all the exact matches will be stored in the tens place) */
      tempseq1[i] = 0;                                /* then set the current elements in the temporary sequences to 0 to avoid multiple checks */
      tempseq2[i] = 0;      
    }     
    i+=1;     
  }     
  i = 0;      
  int j;      
  while(i<seqlen){                                    /* looking for an approximate match */
    j = 0;      
    if ( tempseq2[i] != 0 )                           /* if the element in the temporary guess sequence is 0, skip it */
    while (j<seqlen){     
      if(tempseq2[i]==tempseq1[j]){                   /* if we find an approximate match */
        countMatches+=1;                              /* increment the matches by 1 (all the approximate matches will be stored in the ones place) */
        tempseq1[j] = 0;                              /* then set the current elements in the temporary secret sequence to 0 to avoid multiple checks */
        break;
      }
      j+=1;
    }
    i+=1;
  }
  return countMatches;                          /* return a two digit number- exact matches in tens place and approximate matches in ones place */
}

/* show the results from calling countMatches on seq1 and seq1 */
void showMatches(int code, /* only for debugging */ int * seq1, int *seq2) {
  /* COMPLETE the code here */
    int countM= 0;                              /* variable to store the matches */
    countM = countMatches(seq1, seq2);          /* storing the matches after comparing seq1 and seq2 */
    // if in debugging(unittesting) mode, 
    // print the exact and approximate matches in sentences
    if(code){
      printf("%d exact matches\n", countM/10);  /* printing exact matches (tens place of the matches variable) */
      printf("%d approximate matches\n", countM%10); /* printing approximate matches (one place of the matches variable) */
    }
    else                                        /* else print the exact and approximate matches in one line seperated by a space */
    {
      printf("%d %d", countM/10,  countM%10);
      printf("\n");
    }
}

/* read a guess sequence fron stdin and store the values in arr */
void readString(int *arr) {
  /* COMPLETE the code here */
  printf("Input seq (len %d): ", seqlen);       /* reading guess sequence digit by digit and storing in arr */
  int i = 0;
  while(i<seqlen){
    fscanf(stdin, "%d", &arr[i]);
    i++;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int main(int argc, char **argv){
  /* DEFINE your variables here */
  int found = 0, attempts = 0, i = 0;
  /* for getopts() command line processing */
  int verbose = 0, help = 0, unittest = 0, debug = 0;
  char *sseq = NULL;
  int * guess=NULL;
  int maxAttempts= 10; // maximum number of attempts
  // see: man 3 getopt for docu and an example of command line parsing
  // Use this template to process command line options and to store the input
  {
    int opt;
    while ((opt = getopt(argc, argv, "hvuds:")) != -1) {
      switch (opt) {
        case 'h':
          help = 1;
          break;
        case 'v':
          verbose = 1;
          break;
        case 'u':
          unittest = 1;
          break;
        case 'd':
          debug = 1;
          break;
        case 's':
          seqlen= strlen(optarg);
          sseq = (char *)malloc(seqlen*sizeof(char));
          strcpy(sseq,optarg);
          break;
      default: /* '?' */
	fprintf(stderr, "Usage: %s [-h] [-v] [-d] [-u] [-s] <secret sequence>\n", argv[0]);
	exit(EXIT_FAILURE);
      }
    }
    if (unittest && optind >= argc) {
      fprintf(stderr, "Expected argument after options\n");
      exit(EXIT_FAILURE);
    }

    if (verbose && unittest) {
      printf("1st argument = %s\n", argv[optind]);
      printf("2nd argument = %s\n", argv[optind+1]);
    }
  }

  if(help){ // print out help message
    fprintf(stdout, "Help -- Run the program in the following manner: ./cw3 [-h] [-v] [-d] [-u] sequence1 sequence2 [-s] sequence\n");
    exit(0);
  }
  if (verbose) {
    fprintf(stdout, "Settings for running the program\n");
    fprintf(stdout, "Verbose is %s\n", (verbose ? "ON" : "OFF"));
    fprintf(stdout, "Debug is %s\n", (debug ? "ON" : "OFF"));
    fprintf(stdout, "Unittest is %s\n", (unittest ? "ON" : "OFF"));
    if (sseq)  fprintf(stdout, "Secret sequence set to %s\n", sseq);
  }

  if (sseq) { // explicitly setting secret sequence
    /* SET the secret sequence here */
    theSeq = (int *)malloc(seqlen*sizeof(int)); // allocating memory for secret sequence
    i = 0;
    while(i<seqlen){
      theSeq[i]=sseq[i] - '0';                  // converting each character of secret sequence to integer and storing in theSeq
      i+=1;
    }
  }    
  if (unittest) {
    /* SET secret and guess sequence here */
    /* then run the countMatches function and show the result */
    seqlen = strlen(argv[optind]);
    sseq = (char *)malloc(seqlen*sizeof(char));  // allocating memory for first sequence
    sseq = argv[optind];                         // storing the first sequence in sseq
    theSeq = (int *)malloc(seqlen*sizeof(int));  // allocating memory for second sequence
    i = 0;
    while(i<seqlen){
      theSeq[i]=sseq[i] - '0'; // converting each character of the first sequence to integer and storing in theSeq
      i+=1;
    }
    sseq = argv[optind+1];
    seqlen = strlen(argv[optind+1]);
    guess = (int *)malloc(seqlen*sizeof(int));
    i = 0;
    while(i<seqlen){
      guess[i]=sseq[i] - '0'; // converting each character of the second sequence to integer and storing in guess
      i+=1;
    }
    showMatches(unittest, theSeq, guess); // comparing the two sequence and then printing the matches
    exit(0);
  }

  // -----------------------------------------------------------------------------
  if(!theSeq)
    initSeq(); // initializing secret sequence if not done before
  if(debug)
  showSeq(theSeq); // show the secret sequence if in debug mode
  // +++++ main loop
  while (!(found) && attempts<maxAttempts) {
    attempts++;
    /* IMPLEMENT the main game logic here */
    guess = (int *)malloc(seqlen*sizeof(int));     // allocating memory for guess sequence
    readString(guess);                             // reading guess sequence from the user and storing in guessS
    showMatches(unittest, theSeq, guess);          // compare the guess sequence and secret sequence and display the matches
    if(countMatches(theSeq, guess)==seqlen*10)     // if there are n (n being the length of sequence) exact matches and 0 approximate matches, 
      found+=1;                                    // we have found the correct sequence
  }

  if (found) {
    /* print SUCCESS and the number of iterations */
    printf("SUCCESS!! after %d iteration(s); secret sequence is",attempts);
    i = 0;
    while(i<seqlen){
      printf(" %d",theSeq[i]); // printing the secret sequence
      i+=1;
    }
    printf("\n");
  } else {
    // if we could not find the sequence, print failure message
    printf("Sorry, maximum number of attempts (%d) reached; the secret sequence was",maxAttempts);
    i = 0;
    while(i<seqlen){
      printf(" %d",theSeq[i]); // printing the secret sequence
      i+=1;
    }
    printf("\n");
    printf("Better luck next time!! :)");
    printf("\n");
  }
  return EXIT_SUCCESS;
}
  

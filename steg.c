#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct node {
  char comm[70];
  struct node * nxt;
  
};
/*
  Stores the RGB values of a pixel
*/
typedef struct Pixel{
  int red;
  int green;
  int blue;
} Pixel;


/*
  Stores the contents of a PPM file
*/
typedef struct PPM{
  /* TODO: Question 1 */
  char P3[3];
  struct node * root; //comments read using linked list
  int width ,height,max;
  struct Pixel * Pixel;
} PPM;


//initialize the functions for use in main
struct PPM * getPPM(FILE * fd);
void showPPM(struct PPM * ppm);
void writePPMtoFile(struct PPM * ppm,FILE * fout);
PPM* encode(char* text, PPM* i);
char * decode(struct PPM* i1,struct  PPM* i2);


/* TODO: Question 3 */
int main (int argc, char ** argv){
	FILE * fin;
	FILE * fout;
	
  struct PPM * ppm1;
	struct PPM * ppm2;
	
	char message[500];
	char * choice;
	int lim;

	 
	choice = (char *)malloc(sizeof(char));
	choice = argv[1];
	
	if (choice[0] == 'e'){
		
		fin = fopen(argv[2], "r");
    	ppm1 = getPPM(fin);
		
		lim = (ppm1->height * ppm1->width) / 2;

		printf("Enter a message:\n");
		fgets(message, sizeof(message), stdin);
		
		if (strlen(message) > lim){
			
			printf("message too long, please try again.\n");
			exit(0);
			
		} else {
			
			ppm1 = encode(message,ppm1);
    			
			fout = fopen(argv[3], "w");
			writePPMtoFile(ppm1, fout);
			fclose(fin);
			fclose(fout);
			// showPPM(ppm1);
			printf("Voila!\n");
			
		}
		
	} else if(choice[0] == 'd'){
		
		fin = fopen(argv[2], "r");
		fout = fopen(argv[3], "r");
		ppm1 = getPPM(fin);
		ppm2 = getPPM(fout);
		printf("DecodedMessage: %s",decode(ppm1,ppm2));
		fclose(fin);
		fclose(fout);
		
	}else {
		printf("invalid amount of argumments, must include two arguments");
	}
}


/*
  Returns the PPM struct from the file fd
*/

struct PPM * getPPM(FILE * fd) {
	char com[70]; //array of comments
	int ctr = 0; //counter initialization
	int  width, height, max;
	char ch;
	int n = 0;
	struct node * start; //linked list
   // ppm initialization

	struct PPM * ppm = (struct PPM*)malloc(sizeof(struct PPM));
	ppm->root = malloc(sizeof(struct node)); //allocate memory to hold for the linked list
	strcpy(ppm->root->comm,"comments");
  	char P3[3]; // to read the chars P3, create an array of the chars
	fscanf(fd," %s",&P3);

	if (P3[0] != 'P' || P3[1] != '3'){
		
		printf("Should be in PPM Format\n"); //if P3 is not read then not in PPM format
		exit(1);
		
	}

	ch = fgetc(fd);
	start = ppm->root;
	ch = fgetc(fd); // get characters to read comments
	
	if (ch != '#'){
		
		ungetc(ch,fd); //if # not found then not comment so unget
		
	}
	
	if(ch == '#'){ //if it is a comment
		
		while(ch != '\n'){ //while last character not new line
			com[n++] = ch; 
			ch = fgetc(fd); // keep loop going
			
		}
		
	}
	


	fscanf(fd, " %d %d",&width, &height);	//scan file width and height
	fscanf(fd, " %d", &max); //scan file max

	strcpy(ppm->P3, P3); //copy P3 from PPM file
	ppm->width = width; //intializing variable names
	ppm->height = height;
	ppm->max = max;

	ppm->Pixel = (struct Pixel*)malloc((ppm->height * ppm->width) * sizeof(struct Pixel));
  int r,g,b;
  int i;
	for(i = 0; i < (ppm->height*ppm->width); i++){
			fscanf(fd," %d %d %d ",&r,&g,&b); // read red green and blue values of ppm
			ppm->Pixel[i].red = r; //adding variables for red green and blue values for i(size of ppm)
			ppm->Pixel[i].green = g;
			ppm->Pixel[i].blue = b; 
			
	}

	return  ppm;
	free(ppm);
	fclose(fd);
}

/*
  Prints a PPM struct in the format of a PPM file
*/
void showPPM(struct PPM * ppm){
  struct node * start;
  /* TODO: Question 2b */
  printf("%s\n", ppm->P3);
  printf("%d ",ppm -> width);
  printf("%d\n",ppm->height);
  printf("%d\n", ppm->max);
  
  int i;
  for(i=0; i < (ppm->height * ppm-> width); i++){
		
			printf(" %d %d %d  \n",ppm->Pixel[i].red,ppm->Pixel[i].green,ppm->Pixel[i].blue);
			
	}
  start = ppm->root;
	start = start->nxt;
	
	if ( start != 0) {
		
		while ( start->nxt != 0){
			
			printf("%s\n",start->comm); // print comments from the beginning of the linked list 
			start = start->nxt; //keep going
		}
		printf("%s\n", start->comm); //until it reaches the end at new line
		
	}
}


/*
  Write a PPM to a file
*/
void writePPMtoFile(struct PPM * ppm,FILE * fout){
  struct node * start; //initialize linked list
  
  fprintf(fout,"%s\n", ppm->P3); //writing P3 of ppm to fout file
  start = ppm->root; //initialize start of linked list
	start = start->nxt; //iterate over comments

  if (start != 0) {
    while ( start->nxt != 0){
      fprintf(fout,"%s\n",start->comm); //continue printing comments to file if nxt doesnt point to 0
      start = start->nxt;
    }
  }
  fprintf(fout,"%d %d\n%d\n",ppm->width,ppm->height,ppm->max); //print width height and max to file

	for(int a = 0; a < (ppm->height*ppm->width); a++){
		
			fprintf(fout," %d %d %d  \n",ppm->Pixel[a].red,ppm->Pixel[a].green,ppm->Pixel[a].blue); 
		
	}
}

/*
  Encodes text into red field of PPM
  Returns the encoded PPM
*/
PPM* encode(char* text, PPM* i){	
  time_t t;
	srand((unsigned) time(&t)); //random number gen initialization
  int ctr = 0; //initialize counter
  int start;
  start = rand() % 100; //find random number by div half the size of the ppm file
  int a;
  for(a=start; a < ( i->width*i->height); a++){
    if( ctr < strlen(text)){ //keep counter going till no more chars are left to be read
      i->Pixel[a].red = text[ctr]; //add all that the message entails using ctr to the red pixel in the ppm
      ctr = ctr + 1; //keep going 
    }
  }
  return i;
}


/*
  Compares 2 PPM structs and decodes the message hidden within
  Returns the decoded message if the images have the same dimensions
  Returns NULL otherwise
*/
char * decode(struct PPM* i1,struct  PPM* i2){
  char * decodedMsg = (char *)malloc(1 * sizeof(char *)); //allocate memory for decodedmessage {initializing}
  int i;
  int l = 0;
  int spaces = 1;
  for(i = 0; i < (i1->width*i1->height); i++){
    if(i1->Pixel[i].red != i2->Pixel[i].red){ //if discrepancy found in red pixels start
     decodedMsg = realloc(decodedMsg,spaces * sizeof(char *)); //reallocate memory for the decoded message
     decodedMsg[l]= i2->Pixel[i].red; //read decoded message from red pixels
     l = l+1; //ctr for chars in message
     spaces = spaces + 1; //for whitespaces
    }
  }
  
  return decodedMsg;
  free(decodedMsg);
}

#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define WORDCHUNK 20
#define n 999999

struct binaryTree {
  struct binaryTree *left ; /* Sanat, jotka ovat vasemmalla puolella puun solmua*/
  struct binaryTree *right; /* Sanat, jotka ovat oikealla puolella puun solmua */                 
  char *word;
  int used;
};

struct Words{
	char *pword;
	long times;
}; 

int i = 0;
struct Words newArray[n];


/*compare funktio qsortia varten*/
int compare (const void *a, const void *b){
	return (((struct Words*)b)->times - ((struct Words*)a)->times); 
}

char *getWord(FILE *input){
  char *temp = NULL;
  int counter; /* laskee merkkien lukumäärän, joita muistivarauksessa on*/
  int byte;  
  int wordCount; /*muistivarauksen merkkien määrä*/ 
  
  /* Hypätään yli ensimmäiset ei-aakkoselliset merkit, jotka tulevat vastaan */
  for (byte = fgetc(input); !isalpha(byte); byte = fgetc(input)){
    if (byte == EOF)
      return NULL;
  }
  /* Varataan sanalle vaadittava tila */
  if ((temp = malloc(wordCount = WORDCHUNK)) == NULL)
    return NULL;

  /* aloitetaan tavujen kerääminen */
  for (counter = 0; isalpha(byte) ; byte = fgetc(input)){
    if (counter == wordCount-1){
	
	char *temp2;
	
	if ((temp2 = realloc(temp,wordCount+=WORDCHUNK)) == NULL)
	
	return NULL;
	
	else
	temp = temp2;
    }
    byte = tolower (byte);
    *(temp+counter++) = byte;
  }

  *(temp+counter) = 0;
  
  ungetc(byte,input);
  return temp;
}

/*
**Char pointer stringiin, joka aiotaan lisätä.
**Struct binaryTree ** osoittaa juurisolmuun.
**lisaaSana palauttaa intin, joka osoittaa, kuinka monta sanaa lisätään. 
**/

int addWord(char *teksti, struct binaryTree **base){

	struct binaryTree *temp;
	int direction;
	
	/* Varataan tilaa uudelle solmulle*/
	if((temp = malloc(sizeof *temp)) == NULL)
		return 0;
	
	/*Asetetaan solmulle arvot*/
	temp->left  = temp->right = NULL;
	temp->word = teksti;
	temp->used = 1;
	
	/*Laitetaan sana oikealle paikalleen binääripuussa*/
	if (*base == NULL)
	*base = temp; /*Jos sanoja ei ole vielä asetettu, tehdään uusi solmu*/
	else{
		struct binaryTree *prev = NULL, *this = *base;
		int direction;
		
		/* etsitään solmu, jonka tulisi osoittaa tähän solmuun*/
		while (this != NULL){
			prev = this;
			
			if ((direction = strcmp(this->word, temp->word)) == 0){
				
				/*Sana on jo puussa, palautetaan 0*/
				this->used += 1;
				free(temp);
				return 0;
			}
			else
				if (direction <= 0) /*Uusi sana on entisen sanan rightlla*/
					this = this->right;
				else /*Uusi sana on entisen sanan vasemmalla*/
					this = this->left ;
		}
		if (direction < 0)
			prev->right = temp;
		else
			prev->left  = temp;
	}
	return 1;
	
}

void deleteWord(struct binaryTree *root, int trace) {
	
	if (root == NULL) return;

	deleteWord(root->left ,trace);
	if (trace){
	newArray[i].pword = malloc(sizeof(root->word));
	newArray[i].pword = root->word;
	newArray[i].times = root->used;
	i++;
	deleteWord(root->right,trace);
	}
}
/*
**kaikkiSanat yhdistää kolme aikaisempaa funktiota keskenään.
**hankiSana funktio hakee yksittäisiä sanoja tiedostosta, lisaaSana lisää sanan puuhun. 
**poistaSana poistaa sanan sekä ilmoittaa sanan esiintymiskerrat annetussa tiedostossa.
*/
unsigned long allWords (FILE *input1, FILE *input2, int trace){

	char buffer[n];
	
	int l = 0;
	int c = 0;
    int i = 0;

	struct binaryTree *root = NULL;
	char *characterPointer2;
	char *characterPointer1;
	unsigned long diffWordCount = 0;
	unsigned long allWordCount = 0;

	while ((characterPointer2 = getWord(input2)) !=NULL)
	diffWordCount += addWord(characterPointer2, &root);
	deleteWord(root,trace);
	qsort((void *) newArray, diffWordCount, sizeof(newArray[0]), compare);

	while (newArray[l].pword != '\0'){
		allWordCount += newArray[l].times;
		l++;
	}

	while (c < n - 1 && (characterPointer1 = fgetc(input1))!= EOF) {
		buffer[c++] = tolower(characterPointer1);
		}
	buffer[c] = '\0';

    int init_size = strlen(buffer);
    char delim[] = "' '1234567890.,:;\\//-";
    char *ptr = strtok(buffer, delim);
    
    while (ptr != NULL){
        ptr = strtok(NULL, delim);
        printf("%s", ptr);
        printf("\n");
    }
	printf ("Tiedostossa oli kaiken kaikkiaan %lu sanaa\n", allWordCount);
	return diffWordCount;
}

int usage (char *name, char *text){
	fprintf(stderr, "Ohjelman kaytto: %s [[-v] filename]\n", name);
	if (text != NULL) fprintf(stderr, "		%s\n" ,text);
	return EXIT_FAILURE;
} 

int main(int argc, char *argv[]){
  
  FILE *input1;
  FILE *input2;

  unsigned trace = 0;
  
  switch (argc){
    case 1:
		trace = 1;
		printf ("Anna tiedoston 1 nimi: ");
		char tiedosto1 [50];
		scanf ("%s", tiedosto1);

		printf ("Anna tiedoston 2 nimi: ");
		char tiedosto2 [50];
		scanf ("%s", tiedosto2);

		input1 = fopen(tiedosto1, "r");
		input2 = fopen(tiedosto2, "r");

		printf("Tiedostossa oli %lu erilaista sanaa\n", allWords(input1, input2, trace));
	
		if (input2 != stdin) fclose(input2);

		if (input2 == NULL){
		printf("Tiedostoa ei loydy");
		}
    	break;

    case 2:
      if  (strcmp("-v",argv[1]) == 0){
        trace = 1;
        input2 = stdin;
        printf("Tiedostossa oli %lu erilaista sanaa\n", allWords(input1, input2,trace)); 
		if (input2 != stdin) fclose(input2);
      }
      else
      {
        trace = 0;
        if ((input2 = fopen(argv[1],"r")) == NULL) 
          return usage(argv[0],"Tiedostoa ei ole");
      }
      break;

    default:
      return usage(argv[0],NULL);
      break;
  }
  return EXIT_SUCCESS;
}


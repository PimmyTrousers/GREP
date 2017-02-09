#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
     This global variable is recommended to contain the NUMBER of times each 
     ascii symbol appears in the text file (i.e. I suggest using it as a
     frequency table).
  
     Note that you only need to pay attention to the symbols 'A-Z' and  'a-z' 
     and ' ' and ',' and '.' as the program only parses out words made up of 
     english letters and the only allowable punctuation is space, comma, 
     and period. 
 */
int alphabet[128] ;
int number_of_states;
int number_of_symbols;

/*
------ THE SUBROUTINES BELOW ARE RECOMMENDED ------

int get_word_length() 

int get_num_sym() 
  
void print_trans_table()
  
void construct_trans_table() 
  
*/

int get_word_length(); 

int get_num_sym();
  
void print_trans_table();
  
void construct_trans_table();

int create_freq_table(char *word);

int main(int argc, char *argv[]){

  /////////////////////////////////////////////////////////////////////////
  /* 
     Read in a file by it's name and put contents into an array called:
     "file_contents" that is null terminated and length: "input_file_size"
     I highly recommend you use the variable input_file_size
  */

  // Logic to exit when the user doenst supply the neccesary args
  if(argc != 3){
  	printf("Need to supply a test file and a special word\n");
  	printf("example: ./DFA test_file this\n");
  	exit(0);
  }

  char *file_contents;
  int word_count; // contains the amount of time a word has occured in the string. 
  long input_file_size;
  FILE *input_file = fopen(argv[1], "rb");
  fseek(input_file, 0, SEEK_END); // what is this?
  input_file_size = ftell(input_file);
  rewind(input_file); // what is this?
  file_contents = malloc(input_file_size * (sizeof(char)));
  fread(file_contents, sizeof(char), input_file_size, input_file);
  fclose(input_file);
  file_contents[input_file_size] = '\0';

  /////////////////////////////////////////////////////////////////////////
  
  char *word = argv[2]; // this is the special word
  number_of_states = get_word_length(word) + 4;
  number_of_symbols = create_freq_table(word) + 3;

  char trans_table[100][128]; // this is the array that will eventually turn into the transition table 

  int flag = 0;
  int k ;
  printf("\"") ;

  // work needs to be done in here. 
  for(k=0;k<input_file_size;k++) {
    if(file_contents[k] == '\n') { printf("\"") ; flag = 1 ; }
    printf("%c",file_contents[k]) ;
  }
  if(flag == 0) { printf("\"") ; } 
  printf("\n") ;


}

int get_word_length(char *word){
	int i = 0;
	while(word[i] != '\0'){
		i++;
	}

	return i;
}

// void print_trans_table(char *trans_table){
// 	int i;
// 	int j;

// 	for(i = 0; i < number_of_states; i++){
// 		printf("%d", i);
// 		for(j = 0; j < number_of_symbols; j++){
// 			printf("%c", trans_table[i][j]);
// 		}
// 		printf("\n");
// 	}
// }

// int get_unique_symbol_length(char *word){
// 	int i;
// }

void construct_trans_table(char *word, char *trans_table){

}

int create_freq_table(char *word){
	int i;
	for(i=0; i<128; i++){
		alphabet[i] = 0;
	}

	i = 0;
	while(word[i] != '\0'){
		if( 65 <= word[i] && word[i] <= 90 ){
			alphabet[word[i] + 32]++;
		}
		else{
			alphabet[word[i]]++;
		}
		i++;
	}

	int length = 0;

	for(i=0; i<128; i++){
		if(alphabet[i] != 0){
			length++;
		}
		
	}

	return length;


}






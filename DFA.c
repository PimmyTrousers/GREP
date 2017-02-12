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
  printf("states: %d\n", number_of_states);
  printf("symbols: %d\n", number_of_symbols);

  char *trans_table[100][128]; // this is the array that will eventually turn into the transition table
  construct_trans_table(word, trans_table);

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

void print_trans_table(char trans_table[100][128]){
	int i;
	int j;

	for(i = 0; i < number_of_states; i++){
		printf("%d| ", i);
		for(j = 0; j < number_of_symbols; j++){
			printf("%d ", trans_table[j][i]);
		}
		printf("\n");
	}
}

// int get_unique_symbol_length(char *word){
// 	int i;
// }

void construct_trans_table(char *word, char trans_table[100][128]){
  int i, j, k;
  int word_length = get_word_length(word);
  int ascii_symbols[number_of_symbols-3], ascii_word[word_length];
  int counter = 0;

  // makes our ascii symbols array which organizes our table intp lexible order
  for(i = 0; i < 128; i++){
    if(alphabet[i] != 0){
      ascii_symbols[counter] = i;
      counter++;
    }
  }

  for(i = 0; i < word_length; i++){
    ascii_word[i] = word[i];
  }


  // inits array to all zeros
  for(i = 0; i < number_of_symbols; i++){
    for(j = 0; j < number_of_states; j++){
      trans_table[i][j] = 0;
    }
  }
  // hard codes the stuff that we know
  trans_table[0][0] = 2;
  trans_table[1][0] = 2;
  trans_table[2][0] = 2;
  trans_table[0][1] = 1;
  trans_table[0][2] = 1;
  trans_table[1][1] = 1;
  trans_table[1][2] = 1;
  trans_table[2][1] = 1;
  trans_table[2][2] = 1;

  for(j = 0; j < number_of_states - 1 ; j++){
    trans_table[0][j] = 2;
    trans_table[1][j] = 1;
    trans_table[2][j] = 1;
  }

  // setting second last state row to last state
  trans_table[0][number_of_states - 2] = number_of_states - 1;
  trans_table[1][number_of_states - 2] = number_of_states - 1;
  trans_table[2][number_of_states - 2] = number_of_states - 1;

  //setting second to last state's row where column are not space, comma or period, putting one
  for(i = 3; i < number_of_symbols; i++){
    trans_table[i][number_of_states - 2] = 1;
    trans_table[i][1] = 1;
  }

  // puts the final state in (the force state)
  for(i = 0; i < number_of_symbols; i++){
    trans_table[i][number_of_states - 1] = 2;
  }

  for (j = 0; j < word_length - 1; j++){
    for (i = 0; i < number_of_symbols - 3; i++){
      if(ascii_word[j + 1] == ascii_symbols[i]){
        trans_table[i + 3][j + 3] = j + 4;
      }
      else {
        trans_table[i + 3][j + 3] = 1;
      }
    }
  }
  //setting value of fourth till second to third row
  for (i = 0; i < number_of_symbols-3; i++){
    if (ascii_word[0] == ascii_symbols[i]){
      trans_table[i + 3][0] = 3;
      trans_table[i + 3][2] = 3;
    }
    else {
      trans_table[i + 3][0] = 1;
      trans_table[i + 3][2] = 1;
    }
  }

  print_trans_table(trans_table);

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

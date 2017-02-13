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
long input_file_size;
int trans_table[100][128];


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

int ghetto_grep(char *word, char *file_contents){

  int flag = 0;
  int k, i;
  printf("\"");
  int current_state = 0;
  int occurances = 0;
  int accepting_state = number_of_states - 1; // this is correct 
  int symbols[number_of_symbols+3];

  symbols[2] = 46; // 46 = period.
  symbols[1] = 44; // 44 = comma.
  symbols[0] = 32; // 32 = space. 

  int counter = 3;

  for(i = 0; i < 128; i++){
  	if(alphabet[i] != 0){
  		symbols[counter] = i;
  		counter++;
  	}
  }

  int word_length = get_word_length(word);

  for(k = 0;k < input_file_size; k++){
   int change_flag = 0;
   int current_char = file_contents[k];
   
   for(i = 0; i < number_of_symbols; i++){
   	if(current_char == symbols[i] || current_char == symbols[i]-32){
   		current_state = trans_table[i][current_state];
   		change_flag = 1;
   	}
   }
   if(change_flag == 0){
   	current_state = 1;
   }
  
  	// the case when we reach our accepting state where I assume that the accepting state is number of states - 1.
  	if(current_state == accepting_state){
  		occurances++;
  		current_state = 2;
  	}

  	// if we reach the end of the file (\n)
    if(file_contents[k] == '\n'){
    	printf("\""); 
    	flag = 1;
	}
    printf("%c",file_contents[k]);
   }
  	
  if(flag == 0){
   	printf("\""); 
  }
  printf("\n");
  printf("There were a total of %d occurances\n", occurances);
}

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

  construct_trans_table(word);
  print_trans_table();
  ghetto_grep(word, file_contents);



}

int get_word_length(char *word){
	int i = 0;
	while(word[i] != '\0'){
		i++;
	}

	return i;
}

void print_trans_table(){
	int i;
	int j;

  printf("   ");
  for (i = 0; i < number_of_symbols; i++){
    printf("%d ", i);
  }
  printf("\n");

	for(i = 0; i < number_of_states; i++){
		printf("%d| ", i);
		for(j = 0; j < number_of_symbols; j++){
			printf("%d ", trans_table[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

void construct_trans_table(char *word){
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
			word[i] = word[i] + 32;
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

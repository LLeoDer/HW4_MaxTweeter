#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
maxTweeter finds the top 10 tweeter given a csv file
Arguments: file_location
Ex Call: ./maxTweeter doc/folder/test.csv
*/


static char *user_array[22000];
static int count_array[22000];
static int top10[10] = {0,0,0,0,0,0,0,0,0,0};

//Gets the username
char* extract_username(char* line, int num) {
    char* token;
    for (token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n"))
        if(!--num)  return token;
    return NULL;
}

//Compares num1 and num2 to return the higher number
int max_num(int num1, int num2) {
    if (num1 > num2) return num1;
    else return num2;
}

//Finds the top 10 tweeters in order
//If less that 10, rest are NULL , 0 to ensure output is always 10 lines
void find_top10() {
    int max = 0;
    int max_position = 0;
  // roll through each place in top 10 and assign user
  for(int place = 0; place < 10; place++) {
    if (place == 0) { // first place == base case
        // check every user in user_array
        for (int i = 0; i<22000; i++) {
            if (user_array[i] != NULL) {
                max = max_num(max, count_array[i]);
                if (max == count_array[i]) max_position = i;
            }
        }
    }
    else { // find max of remaining users
        max = 0;
        // roll through entire user_array and search for highest tweet count
        for (int i = 0; i < 21000; i++) {
                    max = max_num(max, count_array[i]);
                    if (max == count_array[i]) max_position = i;
        }
    }

    // top10 array tracks position in user/count arrays for indexing
    top10[place] = max_position;
    printf("%s: %d\n", user_array[max_position], count_array[max_position]);
    count_array[max_position] = 0; // count of prev max set to 0 to avoid counting twice
  } // end for
}

//Error print statement
void throw_error() {
    printf("Invalid Input Format\n");
}

int main(int argc, char **argv) {
    // setup file stream
    //Tests if no argument is found
    if (argv[1] == NULL) {
        throw_error();
        return 0;
    }
    //Opens the file
    FILE* stream = fopen(argv[1], "r");
    char line[1024];

    // other variables
    int name_field = -1;
    int position = 0;
    int check_header = 0;
    int user_found = 0;
    int test = 1;

    //Tests for empty file
    if(NULL != stream){
        fseek(stream,0,SEEK_END);
	int sz = ftell(stream);
	//If end of seek is still at 0, then throw and quit
	if(0 == sz){
	    throw_error();
	    return 0;
	}
	//Else rewind stream
	else{
	    fseek(stream,0,SEEK_SET);	
	}
    }
    // get each line from csv
    while(fgets(line,1024,stream)) {
        char* temp = strdup(line);
      // if 1st line of csv, find name column
      if (check_header == 0) {
          // roll through columns searching for match
          for(int i = 0; i < 30; i++) {
              char* temp2 = strdup(line);
              char* header_contents = extract_username(temp2, i);
              if (header_contents != NULL)
                 test = strcmp(header_contents, "\"name\"");
              if (test == 0)  {
                  name_field = i;
              }

          } // end for
          if (name_field == -1) {
              throw_error();
              return 0;
           }

        check_header = 1;
      }
      // every other line of csv file
      else {
        char* name = extract_username(temp, name_field);
        if (name != NULL) {
            // search user_array for username matching extracted name from csv
            for (int i=0; i<21000; i++) {
                // don't check any blank arrays or array indicies past current user count (will be empty)
                if ( (user_array[i] != NULL) && (i <= position) ) {
                    int user_match = strcmp(user_array[i], name);
                    if (user_match == 0) {
                        count_array[i]++; // user's tweet count stored here -- position matches user_array
                        user_found = 1;
                    }
                }
            } // done searching through list for name
                if (user_found == 0) { // if user wasn't found, add them to array
                    user_array[position] = strdup(name); // position tracks available spot in user_array
                    count_array[position] = 1;
                    position++;
                }
            user_found = 0;
        } // end if name != null
      } // end else
        if (temp != NULL) free(temp);
    } // end while statement

    //Prints top 10
    find_top10();
    return 0;

}

                                                 

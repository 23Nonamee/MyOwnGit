/*included files*/
#include "wrapper.h"
#include "builtin.h"

/*libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Main function
int main(int argc, char *argv[]) {
  
  if (argc < 2){ // Checking if the user use an argument 
    printf("Bad Command Usage!, You need to write: ./mygit <command>\n");
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "init") == 0) { //Checking if the command is init
      init();

  }

  else if (strcmp(argv[1], "hash-object") == 0) { //Checking if the command is hash-object
    
    
    if (argc < 3 ) { //checking if the args are 3 
      printf("Usage: ./mygit hash-object <filename>\n");
      return EXIT_FAILURE;
    }
    
    if(strcmp (argv[2], "-w") == 0){
      
      if (argc < 4 ) { //checking if the args are 3 
      printf("Usage: ./mygit hash-object -w <filename>\n");
      return EXIT_FAILURE;
      }

      hash_object(argv[3], 1);
    }

    else{
      hash_object(argv[2], 0);
    }
  }

  else{
    printf("Unknow Command\n"); 
  }
  return EXIT_SUCCESS;
}

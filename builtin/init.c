
//files
#include "../wrapper.h"
#include "../builtin.h"

//libraries
#include <stdio.h>

int init(void) {

  if (create_dir(".mygit") != 0) { // Create the directory .mygit
    perror("Error creating .mygit");
    return 1;
  }

  if (create_dir(".mygit/objects") != 0){
    perror("Error creating .mygit/objects");
    return 1;
  }

  if (create_dir(".mygit/refs") != 0){
    perror("Error creating .mygit/refs");
    return 1;
  }

  if (create_dir(".mygit/refs/heads") != 0){
    perror("Error creating .mygit/refs/heads");
    return 1;
  }

  FILE *head_file = fopen(".mygit/HEAD", "w"); // create the file HEAD
  if (head_file == NULL) {
    perror("Error creating .mygit/HEAD");
    return 1;
  }
  fprintf(head_file, "ref: refs/heads/main\n"); //add a line in HEAD pointing the default branch
  fclose(head_file);
  
  printf("Initialized empty MyGit repository in .mygit/\n");
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <sys/types.h>

//Funtion to detect the OS

int create_dir(const char *path) {
  #ifdef _WIN32
    return mkdir(path);
  #else 
    return mkdir(path, 0755);
  #endif /* ifdef _WIN32 */
}


// Read bytes and create hash 
int cmd_hash_object(const char *filename, int flag) {
  
  FILE *f = fopen(filename,"rb"); //open the file in memory
  if (f == NULL) {
    perror("No se pudo abrir el archivo"); // if the file doesn't exist give an error
    return 1;
  }

  fseek(f, 0, SEEK_END); // go to the end of the file
  long size = ftell(f); // count how much is the size of the file (f) and save in (size)
  fseek(f, 0, SEEK_SET); // come back to the start
  
  char *content = malloc(size); // make a memory space of the file size 
  fread(content, 1, size, f); // copy the content of the file (f) in (content), ([size] indicate the space)
  

  char header[64]; // create a variable for the header
  int header_len = sprintf(header,"blob %ld", size) + 1; // input the blob and size in the header and count the char quantity + 1 (the \0) and save it in header_len
  
  long total_len = header_len + size; // sum the header_len + the file size and save it in total_len
  char *full_data = malloc(total_len); // create a pointer (full_data) and it assign memory of the (total_len) size
  
  memcpy(full_data,header,header_len); //copy in the top of the memory the header ("blob size")
  memcpy(full_data + header_len, content,size); // skip the header_len bytes and copy the content of the file


  unsigned char hash[20]; // create the variable in bytes to contain the hash
  SHA1((const unsigned char *)full_data, total_len, hash); // take the (full_data) content, use (total_len) to indicate the size of the file + header and save in (hash)
  
  char hex_hash[41]; // create a variable for the hash in hex + the \0

  for (int i = 0; i < 20; i++){
    sprintf(&hex_hash[i * 2],"%02x" , hash[i]); // save in memory slots the hex numbers
  } 
  printf("%s\n", hex_hash); // show the hex in the screen

  if (flag) { 
    char directory_hex[64];// create a 3 char variable 

    /* save the object directory path with the first 2 
    digits of the hex*/
    snprintf(directory_hex, sizeof(directory_hex), ".mygit/objects/%.2s",hex_hash); 
    create_dir(directory_hex); //create the two digits directory
    
    char obj_hex[128];
    snprintf(obj_hex, sizeof(obj_hex), "%s/%s", directory_hex, hex_hash + 2);
    printf("The blob file path is: %s\n", obj_hex); 
  }
  //free the memory
  free(full_data);
  fclose(f);
  free(content);
  
  return 0;

} 

int cmd_init(void) {

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

// Main function
int main(int argc, char *argv[]) {
  
  if (argc < 2){ // Checking if the user use an argument 
    printf("Bad Command Usage!, You need to write: ./mygit <command>\n");
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "init") == 0) { //Checking if the command is init
      cmd_init();

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

      cmd_hash_object(argv[3], 1);
    }

    else{
    cmd_hash_object(argv[2], 0);
    }
  }

  else{
    printf("Unknow Command\n"); 
  }
  return EXIT_SUCCESS;
}

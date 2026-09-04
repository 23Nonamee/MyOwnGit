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
int cmd_hash_object(const char *filename) {
  FILE *f = fopen(filename,"rb");
  if (f == NULL) {
    perror("No se pudo abrir el archivo");
    return 1;
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  
  char *content = malloc(size);
  fread(content, 1, size, f);
  

  char header[64];
  int header_len = sprintf(header,"blob %ld", size) + 1;
  
  long total_len = header_len + size;
  char *full_data = malloc(total_len);
  
  memcpy(full_data,header,header_len);
  memcpy(full_data + header_len, content,size);


  unsigned char hash[20];
  SHA1((const unsigned char *)full_data, total_len, hash);
  
  for (int i = 0; i < 20; i++){
    printf("%02x",hash[i]);
  } 
  printf("\n");


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
    cmd_hash_object(argv[2]);

  }

  else{
    printf("Unknow Command\n"); 
  }
  return EXIT_SUCCESS;
}

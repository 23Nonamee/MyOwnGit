//files
#include "../builtin.h"
#include "../libraries.h"
#include "../wrapper.h"

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <sys/types.h>



// Read bytes and create hash 
int hash_object(const char *filename, int flag) {
  
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
    
    uLong compression_len = compressBound(total_len); // calculate the max size of compression to reserve memory
    Bytef *compressed_data = (Bytef *)malloc(compression_len);// reserve space in memory for the compressed file
  
    compress(compressed_data, &compression_len, (const Bytef *)full_data, total_len); // bytes compression of the file
    FILE *obj_file = fopen(obj_hex, "wb"); //open a file in the path of (obj_hex)

    if (obj_file == NULL){ // check if the file in NULL and give an error
      perror("ERROR: The file don't exist!\n");
    }
    fwrite(compressed_data, 1, compression_len, obj_file); // write the (compressed_data) in (obj_file)
      
    //free memory
    free(compressed_data); 
    fclose(obj_file);
  }


  //free the memory
  free(full_data);
  fclose(f);
  free(content);

  
  return 0;

} 

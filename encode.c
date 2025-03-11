#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */


// Function defination to copy all remaining data from beautiful.bmp to stego.bmp
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
   char ch;
//    while((ch=fgetc(fptr_src_image))!=EOF)
//    {
//      fputc(ch,fptr_stego_image);
//    }
// Reading byte by byte and storing into ch
   while((fread(&ch,1,1,fptr_src_image)) > 0)
   {
    // writing ch into stego.bmp
      fwrite(&ch,1,1,fptr_stego_image);
   }
   return e_success;
}

// Function defination to encode all the secret file data 
Status encode_secret_file_data(EncodeInfo *encInfo)
{
   char image_buffer[200];
   fread(image_buffer,sizeof(char),encInfo->size_secret_file*8,encInfo->fptr_secret);
   encode_data_to_image(image_buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image);
   return e_success;
}

// Function defination to encode file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fread(image_buffer,sizeof(char),32,encInfo->fptr_src_image); // reading 32 bytes from src_image
    image_buffer[32] = encode_size_to_lsb(encInfo->size_secret_file,image_buffer);
    fwrite(image_buffer,sizeof(char),32,encInfo->fptr_stego_image); // writing encoded 32 bytes to dest_image
    return e_success;
}

// Function defination to encode secret_file_extn
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{   
    // Encode the file extension into the image
    if (encode_data_to_image(file_extn, encInfo->extn_size, encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        fprintf(stderr, "Error: Failed to encode the file extension into the image.\n");
        return e_failure;
    }

    printf("File extension encoded successfully.\n");
    return e_success;
}


// Function to encode each byte of size into stego.bmp
Status encode_size_to_lsb(long size, char *image_buffer)
{
   for(int i=0;i<32;i++) // 32 times because size is of 4 bytes i.e, int so to encode 4 bytes 32 bytes is required
   {
     int res = ((size & (1<<(31-i))) >> (31-i)); // getting lsb bit of size and moving it to lsb. Storing it in res
     image_buffer[i] = (image_buffer[i] & 0XFE | res);  // clearing lsb bit and storing the res
    //  if((res&1)==1)
    //  {
    //     image_buffer[i] = ((image_buffer[i] & 0XFE) & res);  // storing the encoded data into an array
    //  }
    //  else if((res&1)==0)
    //  {
    //     image_buffer[i] = ((image_buffer[i] & 0XFE) | res);  // storing the encoded data into an array
    //  }
   }
   return e_success;
    
}

// Function defination to encode secret_file_extn_size into stego.bmp
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fread(image_buffer,sizeof(char),32,encInfo->fptr_src_image); // reading 32 bytes from src_image
    image_buffer[32] = encode_size_to_lsb(encInfo->extn_size,image_buffer);
    fwrite(image_buffer,sizeof(char),32,encInfo->fptr_stego_image); // writing encoded 32 bytes to dest_image
    return e_success;
}

// Function to encode each byte from data into beautiful.bmp and store the result in image_buffer
Status encode_byte_to_lsb(char data, char *image_buffer)
{
   for(int i=0;i<8;i++)
   {
      int res = ((data & (1<<(7-i))) >> (7-i)); // getting particular nth bit from data and moving it to lsb. Storing it in res
      image_buffer[i] = (image_buffer[i] & 0XFE | res);  // clearing lsb bit and setting lsb bit with res
    //   if((res&1)==1)
    //   {
    //     image_buffer[i] = (image_buffer[i] & 0XFE & res);
    //   }
    //   else if((res&1)==0)
    //   {
    //     image_buffer[i] = (image_buffer[i] & 0XFE | res); 
    //   }
         // 
     // clearing the image_buffer[i] th lsb bit | res i.e, data to be encoded , setting that bit by using bitwise OR 
   }
   return e_success;
  
}

// Function which reads that much of bytes that has been passed and writes into stego.bmp after encoding 
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
     // data = #*
     char image_buffer[8]; // array to store the 8 bytes
     
     for(int i=0;i<size;i++)
     {
        if(fread(image_buffer,sizeof(char),8,fptr_src_image)!=8) // reading 8 bytes from beautiful.bmp and storing in image_buffer
        {
            printf("Reading from file falied\n");
            return e_failure;
        }

        if(encode_byte_to_lsb(data[i],image_buffer)!=e_success) // first #, and second *
        {
            printf("encode_byte_to_lsb function failed\n");
            return e_failure;
        }
        
        if(fwrite(image_buffer,sizeof(char),8,fptr_stego_image)!=8) // writing 8 bytes from image_buffer into stego.bmp)
        {
            printf("Writing into stego.bmp failed\n");
            return e_failure;
        }
     }

     return e_success;
     
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
   int size = strlen(MAGIC_STRING); // size = 2, because #*
   // magic string = #*
  if(encode_data_to_image(magic_string, size, encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success) // magic string encode successfully
     return e_success;
  else
    return e_failure;
}

// Function defination to copy 1st 54 bytes from beautiful.bmp to stego.bmp because 54 bytes contains header files 
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
   char arr[54]; // local array to store the header 54 bytes from src_image
   // fread and fwrite returns the no of items successfully read and written respectively
    if(fread(arr,sizeof(char),54,fptr_src_image)==54)// reading 54 header bytes from str_image copying into arr
    {
       if(fwrite(arr,sizeof(char),54,fptr_dest_image)==54) // writing 54 bytes copied in arr into dest_image
         return e_success; // successfully read and written
       else 
         return e_failure; // writing failed
    }
    else
     return e_failure; // reading failed
   
}

// Function defination to check capacity of the beautiful.bmp
Status check_capacity(EncodeInfo *encInfo)
{
    // checking if the beautiful.bmp file's capacity is greater than the total data to be encoded to it
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image); // beautiful.bmp file's capacity is stored in image_capacity
    if(encInfo->image_capacity==0)
      return e_failure;
    
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret); // secret.txt , secret file's size is stored in size_secret_file
    if(encInfo->fptr_secret==0)
       return e_failure;

    if(encInfo->image_capacity > (54 + strlen(MAGIC_STRING)* 8 + (4 * 8) + (4 * 8) + (4 * 8) + (encInfo->size_secret_file *8)))
    {
        // 54(header files) + 2*8 + sizeof(.bmp file) i.e, int *8 + extension .txt i.e 4bytes * 8 + sizeof(seret_file) i.e, int *8 + data inside secret_file 4 * 8
        return e_success; // sufficient capacity is there to store the data
    }
    else
       return e_failure;
   
}


// Function defination to validate all the argv[] elements
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *addr1 =strstr(argv[2],"."); // returns the staring addess of . in argv[2]
    if(strcmp(addr1,".bmp")==0)
    {
        encInfo->src_image_fname=argv[2]; // storing the source image name
    }
    else
      return e_failure;

    char *addr2 = strstr(argv[3],".");
    if(strcmp(addr2,".txt")==0)
    {
        char *file_extn = ".txt";
        int file_size = strlen(file_extn); // size=4
        
        encInfo->extn_size = file_size; // size = 4
        printf("size of extn is %d\n",encInfo->extn_size);
        encInfo->extn_of_secret_file=file_extn;
        //strcpy(encInfo->extn_of_secret_file,file_extn);
        printf("extn is %s\n",encInfo->extn_of_secret_file);
        encInfo->secret_fname=argv[3]; // storing the 3rd argument entered by user to secret_fname , i.e, secret.txt    
    }
    else if(strcmp(addr2,".c")==0)
    {
        char *file_extn = ".c";
        // int len = strlen(file_extn);
        int file_size = strlen(file_extn); // size = 2
        encInfo->extn_size = file_size; // 2
        printf("size of extn is %d\n",encInfo->extn_size);
        encInfo->extn_of_secret_file=file_extn;
        //strcpy(encInfo->extn_of_secret_file,file_extn); // .c
        printf("extn is %s\n",encInfo->extn_of_secret_file);
        //encInfo->extn_of_secret_file = file_extn; // .c
        encInfo->secret_fname=argv[3]; // storing the 3rd argument entered by user to secret_fname, i.e, secret.c
    }
    else if(strcmp(addr2,".sh")==0)
    {
        char *file_extn = ".sh";
        // int len = strlen(file_extn);
        int file_size = strlen(file_extn); // size = 3
        encInfo->extn_size = file_size; // 3
        printf("size of extn is %d\n",encInfo->extn_size);
        encInfo->extn_of_secret_file=file_extn;
        //strcpy(encInfo->extn_of_secret_file,file_extn);// .sh
        printf("extn is %s\n",encInfo->extn_of_secret_file);
        encInfo->secret_fname=argv[3]; // storing the 3rd argument entered by user to secret_fname , i.e, secret.sh

    }
    else
        return e_failure;

    if(argv[4]==NULL)
    {
        encInfo->stego_image_fname = "stego.bmp"; // output file to copy the encocded data from beautiful.bmp and secret.txt
                   
    }
    else if(argv[4]!=NULL)
    {
        char *addr3 = strstr(argv[4],".");
        if(strcmp(addr3,".bmp")==0)
        {
             encInfo->stego_image_fname = argv[4];
            //return e_success;
        }
        else
            return e_failure;
    }
    else 
       return e_failure;
   

   return e_success; // default if all the conditions meet, then return e_success
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);
    fseek(fptr_image,0,SEEK_SET); // moves fptr to starting 
    // Return image capacity
    return width * height * 3; // total size of beautiful.bmp
}

// Function calculates and returns the size of secret file
uint get_file_size(FILE *fptr)
{
    
    fseek(fptr,0,SEEK_END); // moves fptr to EOF
    uint res = ftell(fptr); // tells the size of secret file

    fseek(fptr,0,SEEK_SET); // moves fptr to starting 
    return res; // returns size of the secret file(i.e, no of bytes)

}


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

// Function Definationo to perform encoding operation, returns e_success on successfull encoding or e_failure 
Status do_encoding(EncodeInfo *encInfo)
{ 
    // Function to Open files 
    if(open_files(encInfo) == e_success)
    {
        printf("Open files completed\n");
    }
    else
      return e_failure;

    // Function to Check capacity of the beautiful.bmp file to store the data to be encoded into it
    if(check_capacity(encInfo)== e_success)
    {
       printf("Capactiy checked successfully\n");
    }
    else
     return e_failure;

     // Function to copy first 54 header bytes from beautiful.bmp to stego.bmp file
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf("Copy bmp header is done\n");
    }
    else 
      return e_failure;
     
    // Function to encode(write/copy) magic string data into stego.bmp file
   
    if(encode_magic_string(MAGIC_STRING, encInfo)== e_success)
    {
        printf("Magic string copied successfully\n");
    }
    else
     return e_failure;

    // Function to encode the size of secret file, i.e, ".txt", size=0
    if(encode_secret_file_extn_size(encInfo->extn_size,encInfo)== e_success)
    {
        printf("Secret file extension size copied successfully\n");
    }
    else
     return e_failure;
    
    // Function to encode secret file extn i.e, .txt or .sh or .c
    if(encode_secret_file_extn(encInfo->extn_of_secret_file,encInfo)== e_success)
    {
        printf("Secret file extension copied successfully\n");
    }
    else
     return e_failure;
    
    // Function to encode file size
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)== e_success)
    {
        printf("Secret file size copied successfully\n");
    }
    else
     return e_failure;
    
    // Function to encode all the data in secret file to output file
    if(encode_secret_file_data(encInfo)== e_success)
    {
        printf("Secret file data copied successfully\n");
    }
    else 
      return e_failure;
    
    // Function to copy all the remaining data into stego.bmp so that it looks exactly like beautiful.bmp
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)== e_success)
    {
        printf("All remaining data copied successfully\n");
    }
    else
     return e_failure;

   // Closing all the files after completing the encode operation
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_stego_image);
    fclose(encInfo->fptr_secret);

    return e_success;
}


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"
#include "common.h"
#include "decode.h"

/* Function Defination to Perform decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    // Function to Open files 
    if(opening_files(decInfo) == e_success)
    {
        printf("Open files completed\n");
    }
    else
      return e_failure;
    
    // Fucntion to decode magic string
    if(decode_magic_string(decInfo) == e_success)
    {
      printf("Decoded magic string successfully\n");
    }
    
    // function to decode secret file extn size
    if(decode_secret_file_extn_size(decInfo) == e_success)
    {
      printf("Decoded secert_file_extn_size successfully\n");
    }

     // Function to decode secret file extn
    if(decode_secret_file_extn(decInfo) == e_success)
    {
      printf("Dedcoded secert_file_extn successfully\n");
    }

    // Function to open output file
    if(open_output_file(decInfo) == e_success)
    {
      printf("Output file opened in write mode successfully\n");
    }

     // Function to decode file size
    if(decode_file_size(decInfo) == e_success)
    {
      printf("Decode secert_file_size Successfully\n");
    }

    // Function to decode the data
    if(decode_file_data(decInfo) == e_success)
    {
      printf("Decoded Data from stego.bmp file successfully\n");
    }
   
   // Closing files after doing decoding operation
    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output_image);

    return e_success;

}

// Function defination to read and validate arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    printf("Read and validate\n");
    char *addr1 =strstr(argv[2],"."); // returns the staring addess of . in argv[2]
    if(strcmp(addr1,".bmp")==0)
    {
        decInfo->stego_image_fname=argv[2];
        printf("stego_image_fname :%s\n",decInfo->stego_image_fname);
    }
    else
      return e_failure; 

    
    if(argv[3]==NULL)
    {
        strcpy(decInfo->output_image_fname,"output"); // null means copy output as a filename for output
        printf("output image name is %s\n",decInfo->output_image_fname);
    }
    else if(argv[3] != NULL)
    {
      if(!strchr(argv[3],'.')) // checks if . is present in argv[3] or not
      {
         strcpy(decInfo->output_image_fname,argv[3]); // if not copies the name to output_image_fname
         printf("output image name is %s\n",decInfo->output_image_fname);

      }
      else
      {
         char *out = strchr(argv[3],'.'); // returns the starting address of '.'
        strncpy(decInfo->output_image_fname,argv[3],out-argv[3]); // copies the difference of out and argv[3] i.e, untill .
       //i.e higher address - lower address
        decInfo->output_image_fname[out-argv[3]] = '\0'; // terminating it with null
      }
      
       
      //  char *addr2 = strtok(argv[3],(char *)'.'); // returns the address of the argv[3] till .
      //   if(addr2 != NULL)
      //   {
      //       strcpy(decInfo->output_image_fname,addr2);
      //       printf("output image name is %s\n",decInfo->output_image_fname);
      //   }
        // else
        //     return e_failure;
    }

    return e_success;
}

// Function defination to open stego.bmp file
Status opening_files(DecodeInfo *decInfo)
{
   // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }

    return e_success;
}

// Function defination to decode data from image which reads data from stego_bmp and calls the function decode_byte_from_lsb 
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
   char image_buffer[8];
   for(int i=0;i<size;i++)
   {
    // reading 8 bytes from stego_image each time and storing in image_buffer
      fread(image_buffer,8,1,fptr_stego_image); // reading 8 bytes from stego.bmp and storing it into image buffer
      
      // calling the function decode_byte_from_lsb and storing decoded byte in data's particular index
      data[i] = decode_byte_from_lsb(image_buffer); // passing image_buffer of 8 bytes to the function decode_byte_from_lsb 
      
   }
     return e_success;

}

// Function defination of decode_byte_from_lsb , Main core logic to decode/extract data i.e, data from 8 bytes and storing it in 1 byte
char decode_byte_from_lsb(char *image_buffer)
{
  char res=0;
    for(int i=0;i<8;i++)
    {
      char single_bit = (image_buffer[i] & 1); // getting the lsb bit from each byte image_buffer that is  of 8 bytes and storing it in single_bit
      res = res | (single_bit<< (7 - i)); // adding each bit from msb to lsb position to res;
    }
  
  return res; // returning the total 1 byte of decoded data
}

// Function defination to decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image,54,SEEK_SET); // sets the position of the fptr to 55th byte
    
    //char image_buffer[8], decoded_data[2];
    char  decoded_data[4];
    // running loop for 2 times because wkt, magic string is of 2 bytes i.e, #*
    // for(int i=0;i<2;i++)
    // {
    //   fread(image_buffer,8,1,decInfo->fptr_stego_image); // reading 8 bytes from stego_image each time and storing in image_buffer
    //   decoded_data[i] = decode_byte_from_lsb(image_buffer); // passing image_buffer of 8 bytes to the function decode_byte_from_lsb 
    //   // decode_byte_from_lsb function returns a character of 1 byte , collecting that in decoded_data
    // }
    decode_data_from_image(decoded_data,2,decInfo->fptr_stego_image);
    
    if(strcmp(MAGIC_STRING,decoded_data)==0) // comparing the decoded decode_data with original MAGIC_STRING
    {
      // if both matched means succeess
      printf("Magic string decoded successfully and it is matching to the oringinal code\n");
      return e_success;
    }
    

  return e_failure;

}

// Function defination to decode size from lsb
uint decode_size_from_lsb(char *image_buffer)
{
    uint res = 0;

    // Loop through 32 bits (4 bytes)
    for (int i = 0; i < 32; i++) 
    {
        char single_bit = (image_buffer[i] & 1); // Extract LSB from each byte
        res |= (single_bit << (31 - i));        // Position bits from MSB to LSB
    }

    return res;
}

/* Function to Decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo) // declared by me
{
  char image_buffer[32];
  
  fread(image_buffer,32,1,decInfo->fptr_stego_image);// reading 32 bytes from stego_image and storing in image_buffer
  decInfo->secret_file_extn_size =decode_size_from_lsb(image_buffer); // passing image_buffer of 32 bytes to the function decode_size_from_lsb 
  // decode_size_from_lsb function returns a integer of 4 bytes , storing it in secret_file_extn_size
  printf("extn_size is %u\n",decInfo->secret_file_extn_size);
  return e_success;
}

// Function to decode secret_file_extn
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extension_buffer[8];
    decode_data_from_image(decInfo->secret_file_extn,decInfo->secret_file_extn_size,decInfo->fptr_stego_image);
    // for (i = 0; i < decInfo->secret_file_extn_size; i++) // Running loop for secret_file_extn_size times
    // {
    //     fread(extension_buffer, 8, 1, decInfo->fptr_stego_image); // Reading 8 bytes from fptr_stego_image
    //     decInfo->secret_file_extn[i] = decode_byte_from_lsb(extension_buffer); // Decode byte from LSB
    // }
  
    decInfo->secret_file_extn[decInfo->secret_file_extn_size] = '\0'; // Terminate the extension with null character
    printf("secret file extension is: %s\n", decInfo->secret_file_extn);
    
    strcat(decInfo->output_image_fname, decInfo->secret_file_extn); // Concatenate the extension to the output_image_fname
    printf("output image name is: %s\n", decInfo->output_image_fname);

    return e_success;
}

// Function to open the output file in writing mode
Status open_output_file(DecodeInfo *decInfo)
{
  decInfo->fptr_output_image = fopen(decInfo->output_image_fname,"w");
  if(decInfo->fptr_output_image == NULL)
  {
    perror("fopen");
    fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_image_fname);
    return e_failure;
  }
  return e_success;
}

/* Funcion to Decode secret file size*/
Status decode_file_size(DecodeInfo *decInfo) // declared by me
{
  char image_buffer[32];
  
  fread(image_buffer,32,1,decInfo->fptr_stego_image);// reading 32 bytes from stego_image and storing in image_buffer
  decInfo->secret_file_size =decode_size_from_lsb(image_buffer); // passing image_buffer of 32 bytes to the function decode_size_from_lsb 
  // decode_size_from_lsb function returns a integer of 4 bytes , storing it in secret_file_size

  return e_success;
}

/* Decode secret file data*/
Status decode_file_data(DecodeInfo *decInfo)
{
  rewind(decInfo->fptr_output_image); // for saftey purpose , rewinding the filepointer of output_image to the starting
  char image_buffer[1], ch;
  
  for(int i=0;i<decInfo->secret_file_size;i++) // loop runs untill total secret_file_size i.e, total bytes to be decoded
  {
    //  fread(image_buffer,8,1,decInfo->fptr_stego_image);
    //  ch = decode_byte_from_lsb(image_buffer);
     decode_data_from_image(image_buffer,1,decInfo->fptr_stego_image); // this function decodes 1 byte of data from 8 bytes each time until secret_file_size
     ch = image_buffer[0];
     fwrite(&ch,1,1,decInfo->fptr_output_image); // writing that each character intodecInfo->fptr_output_image
  }
  
  
   return e_success;
}
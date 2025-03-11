#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
//#include "common.h"

/* 
 * Structure to store information required for
 * decoding secret file from stego.bmp i.e, encoded image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct
{
    /* Source Image info  (beautiful.bmp) */
    char *stego_image_fname; // to store source stego.bmp file name
    FILE *fptr_stego_image;  // file pointer to store source stego.bmp file
    uint secret_file_extn_size; // to store decoded secret file extn size
    char secret_file_extn[10]; // to store decoded secret file extn
    uint secret_file_size; // to store secret file total size
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Output Image info (output file)*/
    char output_image_fname[20]; // to store outpot file name
    FILE *fptr_output_image; // file pointer to store output file 


} DecodeInfo; // new name for struct


/* Decoding fucntion prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status opening_files(DecodeInfo *decInfo);

/* Get Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);

/* Decode a byte from LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/* Decode size from lsb*/
uint decode_size_from_lsb(char *image_buffer);

/* Decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo); // declared by me

/* Decode secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo); 

/* Opening the output file after decoding the extension*/
Status open_output_file(DecodeInfo *decInfo);

/* Decode secret file extension*/
Status decode_file_size(DecodeInfo *decInfo); // declared by me

/* Decode secret file data*/
Status decode_file_data(DecodeInfo *decInfo);


#endif


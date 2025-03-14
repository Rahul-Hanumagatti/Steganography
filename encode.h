#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */


typedef struct
{
    /* Source Image info */
    char *src_image_fname; // to store source beautiful.bmp file name
    FILE *fptr_src_image;  // to store source beautiful.bmp file
    uint image_capacity; // to store the image capacity
    //uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname; // to store secret text.txt file name
    FILE *fptr_secret; // to store secret text.txt file
    long size_secret_file; // to store the size of secret file
    char *extn_of_secret_file; // secret file extension , self declared
    int extn_size; // size of extn of secret file, self declared
    // char extn_secret_file[MAX_FILE_SUFFIX];
    // char secret_data[MAX_SECRET_BUF_SIZE];
    

    /* Stego Image Info */
    char *stego_image_fname; // to store result.bmp file name
    FILE *fptr_stego_image; // to store result.bmp file

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

//Status magic_string();

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string( char *magic_string, EncodeInfo *encInfo);

/* Encode size to lsb*/
Status encode_size_to_lsb(long size, char *image_buffer);

/* Encode secret file extension size*/
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo); // declared by me

/* Encode secret file extenstion */ 
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif

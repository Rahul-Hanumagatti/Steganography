#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    //printf("%d\n",argc);  
    EncodeInfo encInfo; // structure for encoding
    DecodeInfo decInfo; // structure for decoding
    uint image_size;
    if(argc >=3) // to check whether minimum arguments are passed to argv[] or not , i.e, min of be 3 arguments must be passed
    {
        if(check_operation_type(argv) == e_encode) // function call to check whether arg[1] is encode or decode
        {
            printf("You have selected for endcode operation\n");
           if(read_and_validate_encode_args(argv, &encInfo)== e_success) // function call for validation of the arguments
           {
             printf("Validation of all the input arguments entered through argv[] is successfull\n");
             if(do_encoding(&encInfo) == e_success) // function call to do encoding
             {
              printf("Encoding the secret file data into beautiful.bmp file completed successfully\n");
             }
           } 
           else
             printf("Validation unsuccessfull\n");
        }
        else if(check_operation_type(argv) == e_decode) // function call to check whether arg[1] is decode or encode
        {
            printf("You have selected for decode operation\n");
           if(read_and_validate_decode_args(argv, &decInfo)== e_success) // function call for validation of the arguments
           {
             printf("Validation of all the input arguments entered through argv[] is successfull\n");
             if(do_decoding(&decInfo) == e_success) // function call to do encoding
             {
               printf("Decoded the data successfully\n");
             }
           } 
           else
             printf("Validation unsuccessfull\n");
        }
        else 
          printf("Entered Argument is neither -e nor -d\n");

    }
    else
      printf("Argument count must be greater than or equal to 3\n");
  
    
 
    return 0;
}

OperationType check_operation_type(char *argv[]){
    if(strcmp(argv[1],"-e")==0)
    {
         return e_encode; // enum
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
        return e_unsupported;
}

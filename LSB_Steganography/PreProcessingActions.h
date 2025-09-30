#ifndef FILEHANDLING
#define FILEHANDLING
#include "types.h"
#include "encode.h"
#include "decode.h"

/* Command Line Interface Operations to be done */
Status CLI_Operations(EncodeInfo *encInfo,DecodeInfo *decInfo, int *OperationPtr, int argc, char *argv[]);
/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo,int* OperationPtr);
/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo,int *OperationPtr);
/* Check operation type */
OperationType check_operation_type(char *argv[]);
/* check if the image.bmp file is valid */
Status CLI_Validate_BMP_Image_File(char *argv[], EncodeInfo *encInfo);
/* check if the image.bmp file is valid when user selects decode operation type*/
Status CLI_Validate_decode_BMP_Image_File(char *argv[],  DecodeInfo *decInfo);
/* check if the secret.txt file is valid */
Status CLI_Validate_Secret_File(char *argv[], EncodeInfo *encInfo);
/* Check if Output file is present, if absent create file */
Status CLI_Validate_Encode_Output_File(char *argv[], EncodeInfo *encInfo);
/* Check if Output file is present, if absent create file */
Status CLI_Validate_Decode_Output_File(char *argv[], DecodeInfo *decInfo);

#endif

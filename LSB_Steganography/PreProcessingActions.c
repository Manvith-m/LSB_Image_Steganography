#include <stdio.h>
#include <string.h>
#include "PreProcessingActions.h"
#include "types.h"
#include "common.h"

/* Operations on the Arguments received in the command line
 * Takes in pointers to decode and encode info and argc and argv
 * gives e_success if the Command line Arguments meet the expection
 * proceed to decode on encode on successful return
*/
Status CLI_Operations(EncodeInfo *encInfo,DecodeInfo *decInfo, int *OperationPtr, int argc, char *argv[])
{   
    if(argc <= 2)
    {
        printf("INFO : Invalid number of arguments..\n");
        printf("INFO : Please choose one of the following..\n\n");
        printf("INFO : Encoding : -e file_to_encode.bmp secret_data.txt [outputfile.txt]\n");
        printf("INFO : Decoding : -d file_to_decode.bmp [outputfile.txt]\n\n");
        printf("INFO : Exiting Application..\n");
        return e_failure;
    }
     if(check_operation_type(argv) == e_encode)
     {
        *OperationPtr = e_encode;
        printf("INFO : User has chosen Encoding..\n");
        if(read_and_validate_encode_args(argc, argv, encInfo, OperationPtr) == e_success)
        {
            printf("INFO : Arguments Validation successful..\n");
            printf("INFO : Arguments meet Expected Criteria..\n");
            return e_success;
        }
        else   
            return e_failure;

     }
     else if(check_operation_type(argv) == e_decode)
     {
        *OperationPtr = e_decode;
        printf("INFO : User has chosen Decoding..\n");
        if(read_and_validate_decode_args(argc, argv, decInfo, OperationPtr) == e_success)
        {
            printf("INFO : Arguments Validation successful..\n");
            printf("INFO : Arguments meet Expected Criteria..\n");
            return e_success;
        }
        else   
            return e_failure;

     }
     else if(check_operation_type(argv) == e_unsupported)
     {
        printf("INFO : User has chosen Wrong type of Operation..\n");
        printf("INFO : Please choose one of the following..\n\n");
        printf("INFO : Encoding : -e file_to_encode.bmp secret_data.txt [outputfile.txt]\n");
        printf("INFO : Decoding : -d file_to_decode.bmp [outputfile.txt]\n\n");
        printf("INFO : Exiting Application..\n");
        return e_failure;
     }

}
/*
* If user selects encode operation then validate the command line arguments accordingly
*  Encoding : -e file_to_encode.bmp secret_data.txt [outputfile.txt]
*/
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo,int* OperationPtr)
{
    if(argc < 4 || argc > 5)
    {
        printf("INFO : Argument Validation Failed..\nINFO : Invalid Argument Count..\n");
        printf("INFO : Please choose one of the following..\n\n");
        printf("INFO : Encoding : -e file_to_encode.bmp secret_data.txt [outputfile.txt]\n");
        printf("INFO : Decoding : -d file_to_decode.bmp [outputfile.txt]\n\n");
        printf("INFO : Exiting Application..\n");
        return e_failure;
    }
    else
    {
        if(CLI_Validate_BMP_Image_File(argv , encInfo) == e_failure) {
            printf("INFO : Argument Validation Failed..\n");
            printf("INFO : Invalid Image.bmp file..\n");
            return e_failure;
        }
        if(CLI_Validate_Secret_File(argv , encInfo) == e_failure) {
            printf("INFO : Argument Validation Failed..\n");
            printf("INFO : Invalid Secret.txt file..\n");
            return e_failure;
        }
        if(CLI_Validate_Encode_Output_File(argv , encInfo) == e_failure) {
            printf("INFO : Argument Validation Failed..\n");
            printf("INFO : Invalid Secret.txt file..\n");
            return e_failure;
        }
        return e_success;
    }
}
/*
* If user selects decode operation then validate the command line arguments accordingly
* Decoding : -d file_to_decode.bmp [outputfile.txt]
*/
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo,int* OperationPtr)
{
    if(argc < 3 || argc > 4)
    {
        printf("INFO : Argument Validation Failed..\nINFO : Invalid Argument Count..\n");
        printf("INFO : Please choose one of the following..\n\n");
        printf("INFO : Encoding : -e file_to_encode.bmp secret_data.txt [outputfile.txt]\n");
        printf("INFO : Decoding : -d file_to_decode.bmp [outputfile.txt]\n\n");
        printf("INFO : Exiting Application..\n");
        return e_failure;
    }
    else
    {
        if(CLI_Validate_decode_BMP_Image_File(argv, decInfo) == e_failure) {
            printf("INFO : Argument Validation Failed..\n");
            printf("INFO : Invalid/No Image.bmp file..\n");
            return e_failure;
        }
        if(CLI_Validate_Decode_Output_File(argv , decInfo) == e_failure) {
            printf("INFO : Argument Validation Failed..\n");
            printf("INFO : Invalid Secret.txt file..\n");
            return e_failure;
        }
        return e_success;
    }
}
/* check if the image.bmp file is valid when user selects encode*/
Status CLI_Validate_BMP_Image_File(char *argv[], EncodeInfo *encInfo)
{
    if((strstr(argv[2],".bmp")) != NULL)
    {
        encInfo->src_image_fname = argv[2];
        return e_success;
    }
    else
        return e_failure;
}
/* check if the image.bmp file is valid when user selects decode operation type*/
Status CLI_Validate_decode_BMP_Image_File(char *argv[],  DecodeInfo *decInfo)
{
    if((strstr(argv[2],".bmp")) != NULL)
    {
        decInfo->encoded_image_fname = argv[2];
        return e_success;
    }
    else
        return e_failure;
}
/* check if the secret.txt file is valid */
Status CLI_Validate_Secret_File(char *argv[], EncodeInfo *encInfo)
{
    if((strstr(argv[3],EXTENSION_TXT)) != NULL)
    {
        encInfo->secret_fname = argv[3];
        for(int i = 0;i<MAX_FILE_SUFFIX;i++) encInfo->extn_secret_file[i] = EXTENSION_TXT[i];
        return e_success;
    }
    else
        return e_failure;
}
/* Check if Output file is present, if absent create file */
Status CLI_Validate_Encode_Output_File(char *argv[], EncodeInfo *encInfo)
{
        if(argv[4] == NULL)
            encInfo->stego_image_fname = "stegano.bmp";
        else
        {
            if(strstr(argv[4],".bmp") != NULL)
            {
                encInfo->stego_image_fname = argv[4];
                return e_success;
            }
            else
                return e_failure;
        }
        return e_success;
}
/* Check if Output file is present, if absent create file */
Status CLI_Validate_Decode_Output_File(char *argv[], DecodeInfo *decInfo)
{       
        if(argv[3] == NULL)
            decInfo->out_txt_fname = "decodedOutput.txt";
        else
        {
            if((strstr(argv[3],".txt")) != NULL)
            {
                decInfo->out_txt_fname = argv[3];
                return e_success;
            }
            else
                return e_failure;
        }
    return e_success;
}
/* Check operation type */
OperationType check_operation_type(char *argv[])
{
    
    if(strcmp(argv[1],"-e") == 0)
        return e_encode;
    else if(strcmp(argv[1],"-d") == 0)
        return e_decode;
    else   
        return e_unsupported;
}

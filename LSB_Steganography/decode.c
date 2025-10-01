#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Start decoding
 * Input: Decode Info Structure
 * Output: Out.txt with the data from input.bmp decoded
 * from input.bmp
*/
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_success)
    {
        printf("INFO : Open Successful..\n");
        printf("INFO : Files have been openend..\n");
        if(check_magic_string(decInfo) != e_success)
            return e_failure;
        if(decode_out_file_extn_size(decInfo) != e_success)
            return e_failure;
        if(decode_out_file_extn(decInfo) != e_success)
            return e_failure;
        if(decode_data_size(decInfo) != e_success)
            return e_failure;
        if(decode_data_to_file(decInfo) != e_success)
            return e_failure;
        return e_success;
    }
    else
        return e_failure;
}
/* Check if magic string is present in the input bmp file
 * Input: Encode Info Structure
 * Output: Return success if magic string is present 
 * if success proceed with decoding else stop and return
 * */
Status check_magic_string(DecodeInfo *decInfo)
{
    
    char magic_string[MAGIC_STRING_LENGTH];

    char buffer[MAGIC_STRING_LENGTH * 8];

    fread(buffer,MAGIC_STRING_LENGTH * 8,1,decInfo->fptr_encoded_image);

    if(decode_byte_from_lsb_to_buffer(buffer, MAGIC_STRING_LENGTH * 8, magic_string) == e_success)
    {
        for(int i = 0;i < MAGIC_STRING_LENGTH; i++)
        {
            if(magic_string[i] != MAGIC_STRING[i])
            {
                printf("INFO :: ERROR :: Invalid Magic string..\n");
                printf("INFO :: Exiting..\n");
                return e_failure;
            }
        }
        printf("INFO : Magic String Found..\n");
        printf("INFO : Initiating decoding\n");
        printf("INFO : Magic string extracted..\n");
        return e_success;
    }
    return e_success;
}
/* get the size of data to be written to file
 * Input: decode Info Structure
 * Output: Return success if size fetch is successful
 * if success proceed with decoding else stop and return
 * */
Status decode_data_size(DecodeInfo *decInfo)
{
    char buffer[MAX_FILE_SUFFIX * 8];

    fread(buffer,MAX_FILE_SUFFIX * 8,1,decInfo->fptr_encoded_image);

    if(decode_num_from_lsb_to_int(buffer,&decInfo->data_size) != e_success)
        return e_failure;
    printf("debug : %ld\n",decInfo->data_size);
    printf("INFO : Extension file size extracted..\n");
    return e_success;
}
/* get the data to be written to file and write to file
 * Input: decode Info Structure
 * Output: Return success if data is written succesfully
 * if success proceed with decoding else stop and return
 * */
Status decode_data_to_file(DecodeInfo *decInfo)
{
    char src_buffer[decInfo->data_size * 8];
    char dest_buffer[decInfo->data_size];
    fread(src_buffer,decInfo->data_size * 8,1,decInfo->fptr_encoded_image);

    if(decode_byte_from_lsb_to_buffer(src_buffer, decInfo->data_size* 8, dest_buffer) == e_success)
    {
        fwrite(dest_buffer,decInfo->data_size,1,decInfo->fptr_out_txt);
        printf("INFO : Data Transfer complete..\n");
        printf("INFO : Decoding Completed..\n");
        return e_success;
    }
    return e_success;
}
/* get the size of extension of the out file code in the bmp
 * Input: decode Info Structure
 * Output: Return success if extension fetch is successful
 * if success proceed with decoding else stop and return
 * */
Status decode_out_file_extn_size(DecodeInfo *decInfo)
{
    
    char buffer[MAX_FILE_SUFFIX * 8];

    fread(buffer,MAX_FILE_SUFFIX * 8,1,decInfo->fptr_encoded_image);

    if(decode_num_from_lsb_to_int(buffer,&decInfo->exten_out_file_size) != e_success)
        return e_failure;
    printf("INFO : Extension file size extracted..\n");
    return e_success;
}
/* get the extension of the out file code in the bmp
 * Input: decode Info Structure
 * Output: Return success if extension fetch is successful
 * if success proceed with decoding else stop and return
 * */
Status decode_out_file_extn(DecodeInfo *decInfo)
{

    char buffer[MAX_FILE_SUFFIX * 8];

    fread(buffer,MAX_FILE_SUFFIX * 8,1,decInfo->fptr_encoded_image);
    
    if(decode_byte_from_lsb_to_buffer(buffer, MAX_FILE_SUFFIX * 8, decInfo->exten_out_file) == e_success)
    {
        printf("debug : ");    
        for(int i =0;i<4;i++) printf("%c",decInfo->exten_out_file[i]); 
        printf("\nINFO : Magic string extracted..\n");      
        return e_success;
    }
    return e_failure;
}
/* 
 * extract 1 bit from each of 8 bytes of bmp file
 * Inputs: source buffer and destination buffer
 * Output: 1 bit from each lsb of 8 bytes is combine into 
 * single byte and stored in the dest buffer
 * Return Value: e_success or e_failure, on file errors
 */
Status decode_byte_from_lsb_to_buffer(const char * src_buffer, int src_buffer_size, char * dest_buffer)
{
    int block_count = src_buffer_size / 8;
    
    for(int block = 0;block < block_count; block++)
    {
        char byte = 0x00;  // reset for each new byte
        
        for (int i = 0; i < 8; i++) {
            char temp = src_buffer[block*8 + i];
            byte |= (temp & 1)<<i;
        }
        dest_buffer[block] = byte;
    }
    return e_success;
}
/* 
 * extract 1 bit from each of 8 bytes of bmp file
 * Inputs: source buffer and destination integer
 * Output: 1 bit from each lsb of 8 bytes is combined into 
 * single byte and stored in the intiger
 * Return Value: e_success or e_failure, on file errors
 */
Status decode_num_from_lsb_to_int(const char * src_buffer, long int *extn_size)
{
    int size = 0;
    for (int i = 0; i < (sizeof(int)*8); i++) {
        int temp = src_buffer[i];
        size |= (temp & 1)<<i;
    }
    *extn_size = size;
    return e_success;
}
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Out.txt file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_decode_files(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_encoded_image = fopen(decInfo->encoded_image_fname, "rb");
    // Do Error handling
    if (decInfo->fptr_encoded_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->encoded_image_fname);

    	return e_failure;
    }
    fseek(decInfo->fptr_encoded_image,54,SEEK_SET);

    // Output file
    decInfo->fptr_out_txt = fopen(decInfo->out_txt_fname, "wb");
    // Do Error handling
    if (decInfo->fptr_out_txt == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->out_txt_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
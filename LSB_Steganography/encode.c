#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Start encoding
 * Input: Encode Info Structure
 * Output: Out.bmp with the data from secret.txt encoded
 * from input.bmp
*/
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("INFO : Open Successful..\n");
        printf("INFO : Files have been openend..\n");
        if(check_capacity(encInfo) != e_failure)
        {
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) != e_failure)
           {   
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success &&
                encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success &&
                encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success&&
                encode_secret_file_data(encInfo) == e_success)
                {
                    copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
                    printf("INFO : Encoding Complete...\n");
                    return e_success;
                }
                else
                    return e_failure;
           }
        }
        else
        {
            printf("INFO : Capacity Insufficient..\n");
            return e_failure;
        }
        return e_success;
    }
    else
        return e_failure;
}
/* check_capacity
 * Input: Encode Info Structure
 * Output: Return success if there is enough space in the input.bmp
 * file to accommodate the secret text
 * */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(encInfo->image_capacity > (16+32+32+32+32)+(encInfo->size_secret_file*8))
    {
        printf("INFO : Image capacity is sufficient..\n");
        return e_success;
    }
    else
        return e_failure;
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
    // printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* Get size of text from secret file
 * Input: secret file ptr
 * Output: strlen(text in secret file) * 8 ( since each character is 
 * going to occupy 8 bytes )
 */
long get_file_size(FILE *fptr_image)
{
    long int data_length;
    fseek(fptr_image, 0, SEEK_END);
    data_length = ftell(fptr_image);
    rewind(fptr_image);
    return data_length;
}
/* Get size of text from secret file
 * Input: secret file ptr
 * Output: strlen(text in secret file) * 8 ( since each character is 
 * going to occupy 8 bytes )
 */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);
    return e_success;
}
/* Encode the magic string into the out.bmp
 * Input: ptr to magic string and out.bmp file ptr
 * Output: e_success if all the target bytes are copied into the destination file
 */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string,strlen(magic_string),encInfo) == e_success)
    {
        printf("INFO : Magic String encoded..\n");
        return e_success;
    }
    else
        return e_failure;
}
/* Encode the size and type of secret file into the out.bmp
 * Input: ptr to secret file name and out.bmp file ptr
 * Output: e_success if all the target bytes are copied into the destination file
 */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    if(encode_num_to_lsb(strlen(file_extn),encInfo)==e_success)
    {
        printf("INFO : Secret file extension size encoded successfully..\n");
    }
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo)==e_success)
    {
        printf("INFO : Secret file extension encoded successfully..\n");
    }

    return e_success;
}
/* Encode the size of data in secret file into the out.bmp
 * Input: secret file data size and out.bmp file ptr
 * Output: e_success if all the target bytes are copied into the destination file
 */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    if(encode_num_to_lsb(file_size,encInfo)==e_success)
    {
        printf("INFO : Secret file extension size encoded successfully..\n");
    }
    return e_success;
}
/* Encode the in secret file into the out.bmp
 * Input: encode info structure with secret file data
 * Output: e_success if all the target bytes are copied into the destination file
 */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char data_buffer[(encInfo->size_secret_file)];
    fread(data_buffer,(encInfo->size_secret_file),1,encInfo->fptr_secret);

    if(encode_data_to_image(data_buffer,(int)encInfo->size_secret_file,encInfo)==e_success)
    {
        printf("INFO : Secret file data encoded successfully..\n");
    }

    return e_success;
}
/* Encode data into the output file
 * Input: source and destination file pointers
 * Output: e_success if all the target bytes are copied into the destination file
 */
Status encode_data_to_image(const char *data, int size, EncodeInfo *encInfo)
{ 
    for(int i = 0;i<size;i++)
    {
        printf("INFO ::: ENCODING DATA ::: %d ::: \r",i++);
        fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo->image_data);
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    printf("\n");
    return e_success;
}
/*
*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    int i = 0;
    while(fread(&ch,1,1,fptr_src)>0)
    {
        printf("INFO ::: COPYING IMAGE DATA ::: %d ::: \r",i++);
        fwrite(&ch,1,1,fptr_dest);
    }
    printf("\n");
    return e_success;
}
/* Encode a character into 8 bytes of the bmp image
 * Input: constant char and Encode info ptr
 * Output: encodes each bit of character into a byte of the bmp and then
 * copies the new bytes into the output.bmp file
 */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0;i < 8 ;i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data>>i)&1);
    }
    return e_success;
}
/* Encode a number into 8 bytes of the bmp image
 * Input: integer num and Encode info ptr
 * Output: encodes each bit of number into a byte of the bmp and then
 * copies the new bytes into the output.bmp file
 */
Status encode_num_to_lsb(int size, EncodeInfo *encInfo)
{
    char num_buffer[sizeof(int)*8];
    fread(num_buffer,sizeof(int)*8,1,encInfo->fptr_src_image);
    for(int i = 0;i<sizeof(int)*8;i++)
    {
        printf("INFO ::: ENCODING SIZE ::: %d ::: \r",i++);
        num_buffer[i] = (num_buffer[i] & 0xFE) | ((size>>i)&1);
    }
    printf("\n");
    fwrite(num_buffer,sizeof(int)*8,1,encInfo->fptr_stego_image);
    return e_success;
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
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
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
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
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

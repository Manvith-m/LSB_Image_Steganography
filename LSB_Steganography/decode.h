#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */


#define MAX_DATA_BUF_SIZE (MAX_SECRET_BUF_SIZE*8)


typedef struct _DecodeInfo
{
    /* Source Image info */
    char *encoded_image_fname;
    FILE *fptr_encoded_image;

    /* Output File Info */
    char *out_txt_fname;
    FILE *fptr_out_txt;
    long int data_size;
    long int exten_out_file_size;
    char exten_out_file[MAX_FILE_SUFFIX];

} DecodeInfo;


/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* check capacity */
Status check_magic_string(DecodeInfo *decInfo);

/* Get image size */
Status decode_out_file_extn(DecodeInfo *decInfo);

/* Get image size */
Status decode_out_file_extn_size(DecodeInfo *decInfo);

/* Get file size */
Status decode_data_size(DecodeInfo *decInfo);

/* Get file size */
Status decode_data_to_file(DecodeInfo *decInfo);

/* decodes 1 byte of char and stores it in a buffer */
Status decode_byte_from_lsb_to_buffer(const char * src_buffer, int src_buffer_size,  char * dest_buffer);

/* decodes 1 byte of int from encoded bmp file */
Status decode_num_from_lsb_to_int(const char * src_buffer, long int *extn_size);

#endif

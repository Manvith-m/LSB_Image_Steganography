#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "PreProcessingActions.h"

int main(int argc, char *argv[])
{
    // Create a structure for the data 
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    int OperationFlag = e_unsupported;
    int *OperationPtr = &OperationFlag;
    // Carry Out File operations like
    // 1. Check for files in directory
    // 2. 

    if(CLI_Operations(&encInfo, &decInfo, OperationPtr, argc, argv) == e_success)
    {
        switch (OperationFlag)
        {
            case e_encode:
                /* code */
                if(do_encoding(&encInfo) == e_success) return e_success;
                else return e_failure;
                break;
            case e_decode:
                /* code */
                if(do_decoding(&decInfo) == e_success) return e_success;
                else return e_failure;
                break;
            default:
                break;
        }
        
    }
}



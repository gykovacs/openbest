#include <matio.h>

#include "openbest-ds/config-ds.h"
#include "openbest-ds/optionTable.h"

int openTest(int argc, char** argv)
{
    mat_t* matfp;

    matfp= Mat_Open(argv[1], MAT_ACC_RDONLY);
    if ( NULL == matfp )
    {
        fprintf(stderr, "Error openning MAT file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    Mat_Close(matfp);
    return 0;
}

int readTest(int argc, char** argv)
{
    mat_t* matfp;

    printf("open\n");
    matfp= Mat_Open(argv[1], MAT_ACC_RDONLY);
    if ( NULL == matfp )
    {
        fprintf(stderr, "Error openning MAT file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("read\n");
    matvar_t* matvar;
    matvar= Mat_VarRead(matfp, "x");
    if ( NULL != matvar )
    {
        Mat_VarPrint(matvar, 0);
        Mat_VarFree(matvar);
    }

    Mat_Close(matfp);
    return 0;
}

int numberOfFieldsTest(int argc, char** argv)
{
    mat_t* matfp;

    printf("open\n");
    matfp= Mat_Open(argv[1], MAT_ACC_RDONLY);
    if ( NULL == matfp )
    {
        fprintf(stderr, "Error openning MAT file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("read\n");
    matvar_t* matvar;
    matvar= Mat_VarReadInfo(matfp, "z");
    if ( NULL != matvar )
    {
        printf("%d %d\n", matvar->data_type, matvar->class_type);
        printf("size: %d\n", Mat_VarGetSize(matvar));
        Mat_VarPrint(matvar, 0);

        int nof= Mat_VarGetNumberOfFields(matvar);
        printf("number of fields: %d\n", nof);

        matvar_t* res= Mat_VarGetCell(matvar, 1);
        printf("%p\n", res);
        nof= Mat_VarGetNumberOfFields(res);
        printf("number of fields: %d\n", nof);
        Mat_VarPrint(res, 0);
        Mat_VarFree(matvar);
    }

    Mat_Close(matfp);
    return 0;
}

int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();

    setPrefix(ot, "openbest matio tests");
    addUsage(ot, "test-ds <option-name> arg1 arg2 ...");

    bool open= false;
    bool read= false;
    bool nof= false;

    addOption(ot, "--open", OPTION_BOOL, (char*)&open, 0, "open test");
    addOption(ot, "--read", OPTION_BOOL, (char*)&read, 0, "read test");
    addOption(ot, "--nof", OPTION_BOOL, (char*)&nof, 0, "read test");

    if ( processArgs(ot, &argc, argv) )
      return -1;

    if ( open )
        return openTest(argc, argv);
    else if ( read )
        return readTest(argc, argv);
    else if ( nof )
        return numberOfFieldsTest(argc, argv);

    return 0;
}

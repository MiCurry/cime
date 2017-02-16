/*
 * More tests for PIO data reading and writing routines.
 *
 * Ed Hartnett
 */
#include <pio.h>
#include <pio_internal.h>
#include <pio_tests.h>

/* The number of tasks this test should run on. */
#define TARGET_NTASKS 4

/* The minimum number of tasks this test should run on. */
#define MIN_NTASKS 4

/* The name of this test. */
#define TEST_NAME "test_pioc_fill"

/* Number of processors that will do IO. */
#define NUM_IO_PROCS 1

/* Number of computational components to create. */
#define COMPONENT_COUNT 1

/* The number of dimensions in the example data. In this test, we
 * are using three-dimensional data. */
#define NDIM 3

/* The length of our sample data along each dimension. */
#define X_DIM_LEN 4
#define Y_DIM_LEN 4

/* The number of timesteps of data to write. */
#define NUM_TIMESTEPS 2

/* The name of the variable in the netCDF output files. */
#define VAR_NAME "foo"

/* The dimension names. */
char dim_name[NDIM][PIO_MAX_NAME + 1] = {"timestep", "x", "y"};

/* Length of the dimensions in the sample data. */
int dim_len[NDIM] = {NC_UNLIMITED, X_DIM_LEN, Y_DIM_LEN};

/* Some sample data values to write. */
char text[] = "hi";
char char_data = 2;
signed char byte_data = -42;
short short_data = -300;
int int_data = -10000;
long int long_data = -20000;
float float_data = -42.42;
double double_data = -420000000000.5;
unsigned char ubyte_data = 43;
unsigned short ushort_data = 666;
unsigned int uint_data = 666666;
long long int64_data = -99999999999;
unsigned long long uint64_data = 99999999999;

char char_fill_value = 18;
signed char byte_fill_value = -18;
short short_fill_value = -1818;
int int_fill_value = -18181818;
float float_fill_value = 18.18;
double double_fill_value = 1818181818.18;
unsigned char ubyte_fill_value = 18;
unsigned short ushort_fill_value = 1818;
unsigned int uint_fill_value = 18181818;
long long int64_fill_value = -181818181818;
unsigned long long uint64_fill_value = 181818181818;

char text_array[X_DIM_LEN/2][Y_DIM_LEN];
char char_array[X_DIM_LEN/2][Y_DIM_LEN];
signed char byte_array[X_DIM_LEN/2][Y_DIM_LEN];
short short_array[X_DIM_LEN/2][Y_DIM_LEN];
int int_array[X_DIM_LEN/2][Y_DIM_LEN];
long int long_array[X_DIM_LEN/2][Y_DIM_LEN];
float float_array[X_DIM_LEN/2][Y_DIM_LEN];
double double_array[X_DIM_LEN/2][Y_DIM_LEN];
unsigned char ubyte_array[X_DIM_LEN/2][Y_DIM_LEN];
unsigned short ushort_array[X_DIM_LEN/2][Y_DIM_LEN];
unsigned int uint_array[X_DIM_LEN/2][Y_DIM_LEN];
long long int64_array[X_DIM_LEN/2][Y_DIM_LEN];
unsigned long long uint64_array[X_DIM_LEN/2][Y_DIM_LEN];

/* Fill up the data arrays with some values. */
void init_arrays()
{
    for (int x = 0; x < X_DIM_LEN/2; x++)
    {
        strcpy(text_array[x], text);
        for (int y = 0; y < Y_DIM_LEN; y++)
        {
            char_array[x][y] = char_data + y + x * Y_DIM_LEN;
            byte_array[x][y] = byte_data + y + x * Y_DIM_LEN;
            short_array[x][y] = short_data + y + x * Y_DIM_LEN;
            int_array[x][y] = int_data + y + x * Y_DIM_LEN;
            long_array[x][y] = int_data + y + x * Y_DIM_LEN;
            float_array[x][y] = float_data + y + x * Y_DIM_LEN;
            double_array[x][y] = double_data + y + x * Y_DIM_LEN;
            ubyte_array[x][y] = ubyte_data + y + x * Y_DIM_LEN;
            ushort_array[x][y] = ushort_data + y + x * Y_DIM_LEN;
            uint_array[x][y] = uint_data + y + x * Y_DIM_LEN;
            int64_array[x][y] = int64_data + y + x * Y_DIM_LEN;
            uint64_array[x][y] = uint64_data + y + x * Y_DIM_LEN;
        }
    }
}

/* Use the vara functions to write some data to an open test file. */
int putget_write_vara(int ncid, int *varid, PIO_Offset *start, PIO_Offset *count,
                      int flavor)
{
    int ret;
    printf("now writing data\n");

    if ((ret = PIOc_put_vara_schar(ncid, varid[0], start, count, (signed char *)byte_array)))
        return ret;

    if ((ret = PIOc_put_vara_text(ncid, varid[1], start, count, (char *)text_array)))
        return ret;

    if ((ret = PIOc_put_vara_short(ncid, varid[2], start, count, (short *)short_array)))
        return ret;

    if ((ret = PIOc_put_vara_int(ncid, varid[3], start, count, (int *)int_array)))
        return ret;

    if ((ret = PIOc_put_vara_float(ncid, varid[4], start, count, (float *)float_array)))
        return ret;

    if ((ret = PIOc_put_vara_double(ncid, varid[5], start, count, (double *)double_array)))
        return ret;

    printf("now wrote classic data\n");
    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        printf("now writing netcdf4 data\n");
        if ((ret = PIOc_put_vara_uchar(ncid, varid[6], start, count, (unsigned char *)ubyte_array)))
            return ret;
        if ((ret = PIOc_put_vara_ushort(ncid, varid[7], start, count, (unsigned short *)ushort_array)))
            return ret;
        if ((ret = PIOc_put_vara_uint(ncid, varid[8], start, count, (unsigned int *)uint_array)))
            return ret;
        if ((ret = PIOc_put_vara_longlong(ncid, varid[9], start, count, (long long *)int64_array)))
            return ret;
        if ((ret = PIOc_put_vara_ulonglong(ncid, varid[10], start, count, (unsigned long long *)uint64_array)))
            return ret;
    }

    return 0;
}

int check_fill(int ncid, int *varid, int flavor, int default_fill)
{
    int fill_mode;
    char char_fill_value_in;
    signed char byte_fill_value_in;
    short short_fill_value_in;
    int int_fill_value_in;
    float float_fill_value_in;
    double double_fill_value_in;
    unsigned char ubyte_fill_value_in;
    unsigned short ushort_fill_value_in;
    unsigned int uint_fill_value_in;
    long long int64_fill_value_in;
    unsigned long long uint64_fill_value_in;
    int ret;

    printf("checking fill values for flavor %d default_fill %d\n", flavor, default_fill);
    
    if ((ret = PIOc_inq_var_fill(ncid, varid[0], &fill_mode, &byte_fill_value_in)))
        return ret;
    printf("byte_fill_value_in = %d\n", (int)byte_fill_value_in);
    if (fill_mode != NC_FILL || byte_fill_value_in != (default_fill ? NC_FILL_BYTE : byte_fill_value))
        return ERR_WRONG;
    fill_mode = -99;

    if ((ret = PIOc_inq_var_fill(ncid, varid[1], &fill_mode, &char_fill_value_in)))
        return ret;
    if (fill_mode != NC_FILL || char_fill_value_in != (default_fill ? NC_FILL_CHAR : char_fill_value))
        return ERR_WRONG;
    fill_mode = -99;

    if ((ret = PIOc_inq_var_fill(ncid, varid[2], &fill_mode, &short_fill_value_in)))
        return ret;
    if (fill_mode != NC_FILL || short_fill_value_in != (default_fill ? NC_FILL_SHORT : short_fill_value))
        return ERR_WRONG;
    fill_mode = -99;

    if ((ret = PIOc_inq_var_fill(ncid, varid[3], &fill_mode, &int_fill_value_in)))
        return ret;
    printf("int_fill_value_in = %d\n", int_fill_value_in);
    if (fill_mode != NC_FILL || int_fill_value_in != (default_fill ? NC_FILL_INT : int_fill_value))
        return ERR_WRONG;
    fill_mode = -99;

    if ((ret = PIOc_inq_var_fill(ncid, varid[4], &fill_mode, &float_fill_value_in)))
        return ret;
    if (fill_mode != NC_FILL || float_fill_value_in != (default_fill ? NC_FILL_FLOAT : float_fill_value))
        return ERR_WRONG;
    fill_mode = -99;

    if ((ret = PIOc_inq_var_fill(ncid, varid[5], &fill_mode, &double_fill_value_in)))
        return ret;
    if (fill_mode != NC_FILL || double_fill_value_in != (default_fill ? NC_FILL_DOUBLE : double_fill_value))
        return ERR_WRONG;
    fill_mode = -99;

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_inq_var_fill(ncid, varid[6], &fill_mode, &ubyte_fill_value_in)))
            return ret;
        if (fill_mode != NC_FILL || ubyte_fill_value_in != (default_fill ? NC_FILL_UBYTE : ubyte_fill_value))
            return ERR_WRONG;
        fill_mode = -99;
            
        if ((ret = PIOc_inq_var_fill(ncid, varid[7], &fill_mode, &ushort_fill_value_in)))
            return ret;
        if (fill_mode != NC_FILL || ushort_fill_value_in != (default_fill ? NC_FILL_USHORT : ushort_fill_value))
            return ERR_WRONG;
        fill_mode = -99;
            
        if ((ret = PIOc_inq_var_fill(ncid, varid[8], &fill_mode, &uint_fill_value_in)))
            return ret;
        if (fill_mode != NC_FILL || uint_fill_value_in != (default_fill ? NC_FILL_UINT : uint_fill_value))
            return ERR_WRONG;
        fill_mode = -99;
            
        if ((ret = PIOc_inq_var_fill(ncid, varid[9], &fill_mode, &int64_fill_value_in)))
            return ret;
        if (fill_mode != NC_FILL || int64_fill_value_in != (default_fill ? NC_FILL_INT64 : int64_fill_value))
            return ERR_WRONG;
        fill_mode = -99;
            
        if ((ret = PIOc_inq_var_fill(ncid, varid[10], &fill_mode, &uint64_fill_value_in)))
            return ret;
        if (fill_mode != NC_FILL || uint64_fill_value_in != (default_fill ? NC_FILL_UINT64 : uint64_fill_value))
            return ERR_WRONG;
        fill_mode = -99;
    }

    return 0;
}

/* Use the vara functions to read some data from an open test file. */
int putget_read_vara(int ncid, int *varid, PIO_Offset *start, PIO_Offset *count,
                     int default_fill, int flavor)
{
    signed char byte_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    char text_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    short short_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned char ubyte_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    int int_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    float float_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    double double_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned short ushort_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned int uint_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    long long int64_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned long long uint64_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    int x, y;
    int ret;

    /* Read the data we wrote. */
    if ((ret = PIOc_get_vara_schar(ncid, varid[0], start, count, (signed char *)byte_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_text(ncid, varid[1], start, count, (char *)text_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_short(ncid, varid[2], start, count, (short *)short_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_int(ncid, varid[3], start, count, (int *)int_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_float(ncid, varid[4], start, count, (float *)float_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_double(ncid, varid[5], start, count, (double *)double_array_in)))
        return ret;

    for (x = 0; x < X_DIM_LEN/2; x++)
    {
        if (strncmp(text_array_in[x], text, strlen(text)))
                return ERR_WRONG;
        for (y = 0; y < Y_DIM_LEN; y++)
        {
            if (byte_array_in[x][y] != byte_array[x][y])
                return ERR_WRONG;
            if (short_array_in[x][y] != short_array[x][y])
                return ERR_WRONG;
            if (int_array_in[x][y] != int_array[x][y])
                return ERR_WRONG;
            if (float_array_in[x][y] != float_array[x][y])
                return ERR_WRONG;
            if (double_array_in[x][y] != double_array[x][y])
                return ERR_WRONG;
        }
    }

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_get_vara_uchar(ncid, varid[6], start, count, (unsigned char *)ubyte_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_ushort(ncid, varid[7], start, count, (unsigned short *)ushort_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_uint(ncid, varid[8], start, count, (unsigned int *)uint_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_longlong(ncid, varid[9], start, count, (long long *)int64_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_ulonglong(ncid, varid[10], start, count, (unsigned long long *)uint64_array_in)))
            return ret;
        for (x = 0; x < X_DIM_LEN/2; x++)
            for (y = 0; y < Y_DIM_LEN; y++)
            {
                if (ubyte_array_in[x][y] != ubyte_array[x][y])
                    return ERR_WRONG;
                if (ushort_array_in[x][y] != ushort_array[x][y])
                    return ERR_WRONG;
                if (uint_array_in[x][y] != uint_array[x][y])
                    return ERR_WRONG;
                if (int64_array_in[x][y] != int64_array[x][y])
                    return ERR_WRONG;
                if (uint64_array_in[x][y] != uint64_array[x][y])
                    return ERR_WRONG;
            }
    }

    /* Check some fill value stuff. */
    if ((ret = check_fill(ncid, varid, flavor, default_fill)))
        return ret;

    return 0;
}

/* Use the vara functions to read some data from an open test
 * file. Expect only fill data. */
int putget_read_vara_fill(int ncid, int *varid, PIO_Offset *start, PIO_Offset *count,
                          int default_fill, int flavor)
{
    signed char byte_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    char text_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    short short_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned char ubyte_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    int int_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    float float_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    double double_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned short ushort_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned int uint_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    long long int64_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    unsigned long long uint64_array_in[X_DIM_LEN/2][Y_DIM_LEN];
    int x, y;
    int ret;

    /* Read the data we wrote. */
    if ((ret = PIOc_get_vara_schar(ncid, varid[0], start, count, (signed char *)byte_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_text(ncid, varid[1], start, count, (char *)text_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_short(ncid, varid[2], start, count, (short *)short_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_int(ncid, varid[3], start, count, (int *)int_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_float(ncid, varid[4], start, count, (float *)float_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_double(ncid, varid[5], start, count, (double *)double_array_in)))
        return ret;

    for (x = 0; x < X_DIM_LEN/2; x++)
    {
        for (y = 0; y < Y_DIM_LEN; y++)
        {
            if (byte_array_in[x][y] != (default_fill ? NC_FILL_BYTE : byte_fill_value))
                return ERR_WRONG;
            if (text_array_in[x][y] != (default_fill ? NC_FILL_CHAR : char_fill_value))
                return ERR_WRONG;
            if (short_array_in[x][y] != (default_fill ? NC_FILL_SHORT : short_fill_value))
                return ERR_WRONG;
            if (int_array_in[x][y] != (default_fill ? NC_FILL_INT : int_fill_value))
                return ERR_WRONG;
            if (float_array_in[x][y] != (default_fill ? NC_FILL_FLOAT : float_fill_value))
                return ERR_WRONG;
            if (double_array_in[x][y] != (default_fill ? NC_FILL_DOUBLE : double_fill_value))
                return ERR_WRONG;
        }
    }

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_get_vara_uchar(ncid, varid[6], start, count, (unsigned char *)ubyte_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_ushort(ncid, varid[7], start, count, (unsigned short *)ushort_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_uint(ncid, varid[8], start, count, (unsigned int *)uint_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_longlong(ncid, varid[9], start, count, (long long *)int64_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_ulonglong(ncid, varid[10], start, count, (unsigned long long *)uint64_array_in)))
            return ret;
        for (x = 0; x < X_DIM_LEN/2; x++)
            for (y = 0; y < Y_DIM_LEN; y++)
            {
                if (ubyte_array_in[x][y] != (default_fill ? NC_FILL_UBYTE : ubyte_fill_value))
                    return ERR_WRONG;
                if (ushort_array_in[x][y] != (default_fill ? NC_FILL_USHORT : ushort_fill_value))
                    return ERR_WRONG;
                if (uint_array_in[x][y] != (default_fill ? NC_FILL_UINT : uint_fill_value))
                    return ERR_WRONG;
                if (int64_array_in[x][y] != (default_fill ? NC_FILL_INT64 : int64_fill_value))
                    return ERR_WRONG;
                if (uint64_array_in[x][y] != (default_fill ? NC_FILL_UINT64 : uint64_fill_value))
                    return ERR_WRONG;
            }
    }

    return 0;
}

/* Create a test file for the tests to write data to and check by
 * reading it back. In this function we create the file, define the
 * dims and vars, and pass back the ncid.
 *
 * @param iosysid the IO system ID.
 * @param try the number of the test run, 0 for var, 1 for var1, 2 for
 * vara, 3 for vars.
 * @param flavor the PIO IO type.
 * @param dim_len array of length NDIM of the dimension lengths.
 * @param varid array of varids for the variables in the test file.
 * @param filename the name of the test file to create.
 * @param default_fill true if default fill values are to be used.
 * @param ncidp pointer that gets the ncid of the created file.
 * @returns 0 for success, error code otherwise.
 */
int create_putget_file(int iosysid, int flavor, int *dim_len, int *varid, const char *filename,
                       int default_fill, int *ncidp)
{
    int dimids[NDIM];        /* The dimension IDs. */
    int num_vars = NUM_CLASSIC_TYPES;
    int xtype[NUM_NETCDF_TYPES] = {PIO_BYTE, PIO_CHAR, PIO_SHORT, PIO_INT, PIO_FLOAT, PIO_DOUBLE,
                                    PIO_UBYTE, PIO_USHORT, PIO_UINT, PIO_INT64, PIO_UINT64, PIO_STRING};
    int ncid;
    int ret;

    /* Create the netCDF output file. */
    if ((ret = PIOc_createfile(iosysid, &ncid, &flavor, filename, PIO_CLOBBER)))
        return ret;

    /* Turn on fill mode. */
    int old_mode;
    if ((ret = PIOc_set_fill(ncid, NC_FILL, &old_mode)))
        return ret;
    printf("old_mode = %d\n", old_mode);
    if (old_mode != NC_NOFILL)
        return ERR_WRONG;

    /* Define netCDF dimensions and variable. */
    for (int d = 0; d < NDIM; d++)
        if ((ret = PIOc_def_dim(ncid, dim_name[d], (PIO_Offset)dim_len[d], &dimids[d])))
            return ret;

    /* For netcdf-4, there are extra types. */
    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
        num_vars = NUM_NETCDF_TYPES;

    /* Define variables. */
    for (int v = 0; v < num_vars; v++)
    {
        char var_name[PIO_MAX_NAME + 1];
        snprintf(var_name, PIO_MAX_NAME, "%s_%d", VAR_NAME, xtype[v]);
        printf("defining var %s\n", var_name);
        if ((ret = PIOc_def_var(ncid, var_name, xtype[v], NDIM, dimids, &varid[v])))
            return ret;
    }

    /* Maybe set fill values. */
    if (!default_fill)
    {
        if ((ret = PIOc_def_var_fill(ncid, varid[0], NC_FILL, &byte_fill_value)))
            return ret;
        if ((ret = PIOc_def_var_fill(ncid, varid[1], NC_FILL, &char_fill_value)))
            return ret;
        if ((ret = PIOc_def_var_fill(ncid, varid[2], NC_FILL, &short_fill_value)))
            return ret;
        if ((ret = PIOc_def_var_fill(ncid, varid[3], NC_FILL, &int_fill_value)))
            return ret;
        if ((ret = PIOc_def_var_fill(ncid, varid[4], NC_FILL, &float_fill_value)))
            return ret;
        if ((ret = PIOc_def_var_fill(ncid, varid[5], NC_FILL, &double_fill_value)))
            return ret;
        if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
        {
            if ((ret = PIOc_def_var_fill(ncid, varid[6], NC_FILL, &ubyte_fill_value)))
                return ret;
            if ((ret = PIOc_def_var_fill(ncid, varid[7], NC_FILL, &ushort_fill_value)))
                return ret;
            if ((ret = PIOc_def_var_fill(ncid, varid[8], NC_FILL, &uint_fill_value)))
                return ret;
            if ((ret = PIOc_def_var_fill(ncid, varid[9], NC_FILL, &int64_fill_value)))
                return ret;
            if ((ret = PIOc_def_var_fill(ncid, varid[10], NC_FILL, &uint64_fill_value)))
                return ret;
        }
    }

    if ((ret = PIOc_enddef(ncid)))
        return ret;

    /* Pass back the ncid. */
    *ncidp = ncid;

    return 0;
}

/* Test data fill values.
 *
 * This function creates a file with 3 dimensions, with a var of each
 * type. Then it uses the vara functions to write, and then read a
 * subset of the variable data from the test file. The rest of the
 * data are fill values.
 *
 * @param iosysid the iosystem ID that will be used for the test.
 * @param num_flavors the number of different IO types that will be tested.
 * @param flavor an array of the valid IO types.
 * @param my_rank 0-based rank of task.
 * @returns 0 for success, error code otherwise.
 */
int test_fill(int iosysid, int num_flavors, int *flavor, int my_rank,
              MPI_Comm test_comm)
{
    int dim_len[NDIM] = {NUM_TIMESTEPS, X_DIM_LEN, Y_DIM_LEN};

    printf("running tests for %d flavors\n", num_flavors);

    /* Test with and without default fill values. */
    for (int default_fill = 0; default_fill < 2; default_fill++)
    {
        /* Use PIO to create the example file in each of the four
         * available ways. */
        for (int fmt = 0; fmt < num_flavors; fmt++)
        {
            char filename[PIO_MAX_NAME + 1]; /* Test filename. */
            char iotype_name[PIO_MAX_NAME + 1];
            int ncid;
            int varid[NUM_NETCDF_TYPES];
            int ret;    /* Return code. */

            /* Create a filename. */
            if ((ret = get_iotype_name(flavor[fmt], iotype_name)))
                return ret;
            snprintf(filename, PIO_MAX_NAME, "%s_default_fill_%d_%s.nc", TEST_NAME, default_fill, iotype_name);

            /* Create test file with dims and vars defined. */
            printf("%d creating test file %s for flavor = %d...\n", my_rank, filename, flavor[fmt]);
            if ((ret = create_putget_file(iosysid, flavor[fmt], dim_len, varid, filename,
                                          default_fill, &ncid)))
                return ret;
            printf("created file %s\n", filename);

            /* Write some data. */
            PIO_Offset start[NDIM] = {1, 0, 0};
            PIO_Offset count[NDIM] = {1, X_DIM_LEN/2, Y_DIM_LEN};

            printf("writing data to %s\n", filename);
            /* Use the no-type vara functions to write some data. */
            if ((ret = putget_write_vara(ncid, varid, start, count, flavor[fmt])))
                return ret;

            printf("wrote data to %s\n", filename);
            /* Make sure all data are written (pnetcdf needs this). */
            if ((ret = PIOc_sync(ncid)))
                return ret;

            /* Use the vara functions to read some data. */
            if ((ret = putget_read_vara(ncid, varid, start, count, default_fill, flavor[fmt])))
                return ret;

            /* Close the netCDF file. */
            if ((ret = PIOc_closefile(ncid)))
                ERR(ret);

            /* Access to read it. */
            printf("about to try to open file %s\n", filename);
            if ((ret = PIOc_openfile(iosysid, &ncid, &(flavor[fmt]), filename, PIO_WRITE)))
                ERR(ret);

            /* Use the vara functions to read some data. */
            if ((ret = putget_read_vara(ncid, varid, start, count, default_fill, flavor[fmt])))
                return ret;

            /* Use the vara functions to read some data which are just fill values. */
            start[0] = 0;
            if ((ret = putget_read_vara_fill(ncid, varid, start, count, default_fill, flavor[fmt])))
                return ret;

            /* Close the netCDF file. */
            if ((ret = PIOc_closefile(ncid)))
                ERR(ret);

        } /* next flavor */
    }

    return PIO_NOERR;
}

/* Run all the tests. */
int test_all(int iosysid, int num_flavors, int *flavor, int my_rank, MPI_Comm test_comm,
             int async)
{
    int my_test_size;
    int ret; /* Return code. */

    if ((ret = MPI_Comm_size(test_comm, &my_test_size)))
        MPIERR(ret);

    /* Test read/write stuff. */
    printf("%d Testing fill. async = %d\n", my_rank, async);
    if ((ret = test_fill(iosysid, num_flavors, flavor, my_rank, test_comm)))
        ERR(ret);

    return PIO_NOERR;
}

/* Run Tests for NetCDF-4 Functions. */
int main(int argc, char **argv)
{
    int ret;
    
    /* Initialize data arrays with sample data. */
    init_arrays();

    /* Change the 5th arg to 3 to turn on logging. */
    if ((ret = run_test_main(argc, argv, MIN_NTASKS, TARGET_NTASKS, 0,
                             TEST_NAME, dim_len, COMPONENT_COUNT, NUM_IO_PROCS)))
        return ret;

    return 0;
}

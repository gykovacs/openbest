#include "openbest-av/buildSpatialMap.h"

#include "openbest-ds/printOut.h"
#include "openbest-ds/basicAlgorithms.h"

void buildSpatialMap(float* weight, int n_weight,
                     real* target_map, int n_target_map,
                     bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                     int* expand_map, int n_expand_map,
                     double nugget, berkeleyAverageOptions* options

                     )
{
    int pool_size= 1;
    double mix_term= 1.0 - nugget;
    int len_T= n_occurance_table2;
    int len_S= n_occurance_table1;

    int len_M= n_target_map;

    bool table_mode;
    if ( len_M == 1 )
    {
        table_mode= true;
        tprintf("Generate Spatial Table\n");
    }
    else
    {
        table_mode= false;
        tprintf("Generate Spatial Map\n");
    }

    // The following describes an elaborate process by which results associated
    // with time t_i are used to accelerate the computation of the coefficients
    // at time t_i+1. As the station network changes very little between time
    // steps, the coefficients are very similar and this approach provides a
    // very large performance enhancement; however, the algorithm required to
    // implement this makes the resulting code considerably more opaque.

    // Break the spatial network changes into multiple block that share most of
    // their stations in common.

    bool* common_list= bnalloc(n_occurance_table1 * n_occurance_table2);
    setb(common_list, n_occurance_table1 * n_occurance_table2, false);
    int n_common_list1= n_occurance_table1;
    int n_common_list2= n_occurance_table2;

    int* common_index= inalloc(len_T);
    seti(common_index, len_T, 0);

    bool* common= bnalloc(n_occurance_table1);
    int n_common= n_occurance_table1;
    bool* base= bnalloc(n_occurance_table1);
    int n_base= n_occurance_table1;
    int i, j, k;
    for ( j= 0; j < len_T; ++j )
    {
        int sum_occurance_table_j= 0;
        for ( i= 0; i < n_occurance_table1; ++i )
            sum_occurance_table_j+= occurance_table[i*n_occurance_table2 + j];

        if ( sum_occurance_table_j < 50 )
            common_index[j]= 0;
        else if ( common_index[j] == 0 )
        {
            int m= j;
            while ( m < len_T )
            {
                setb(common, n_common, false);
                setb(base, n_base, false);

                for ( i= 0; i < n_occurance_table1; ++i )
                    for ( k= j; k <= m; ++k )
                    {
                        common[i]&= occurance_table[i*n_occurance_table2 + k];
                        base[i]|= occurance_table[i*n_occurance_table2 + k];
                    }

                int sum_common= 0;
                int sum_base= 0;
                for ( i= 0; i < n_common; ++i )
                {
                    sum_common+= common[i];
                    sum_base+= base[i];
                }

                if ( sum_common < 0.7 * sum_base )
                {
                    --m;
                    break;
                }
                ++m;
            }

            if ( m > len_T )
                m= len_T;

            int max_common_index= maxI(common_index, j) + 1;
            for ( i= j; i < m; ++i )
                common_index[i]= max_common_index;

            for ( i= 0; i < n_common_list1; ++i )
            {
                common_list[i*n_common_list2 + common_index[j]]= 0;
                for ( k= 0; k < n_common_list2; ++k )
                {
                    common_list[i*n_common_list2 + common_index[j]]&= occurance_table[i*n_occurance_table2 + k];
                }
            }
        }
    }

    int max_common_index= maxI(common_index, n_common_index) + 1;
    bool* common_list2= bnalloc(n_common_list1 * max_common_index );

    for ( i= 0; i < n_common_list1; ++i )
        for ( j= 0; j < max_common_index; ++j )
            common_list2[i*max_common_index + j]= common_list[i*n_common_list2 + j];
    free(common_list);
    common_list= common_list2;
    n_common_list2= max_common_index;


    int n;

    for ( n= 0; n < max_common_index; n+= pool_size )
    {
        tprintf("Begin of Build Spatial Maps %f\n", n / max_common_index );

        int** remap_pass= (int**)malloc(sizeof(int*)*pool_size);
        int* n_remap_pass= inalloc(pool_size);
        int** remap_s_pass= (int**)malloc(sizeof(int*)*pool_size);
        int* n_remap_s_pass= inalloc(pool_size);
        float** weight2_pass= (float**)malloc(sizeof(float*)*pool_size);
        int* n_weight2_pass1= inalloc(pool_size);
        int* n_weight2_pass2= inalloc(pool_size);
        float** weight2_copy= (float**)malloc(sizeof(float*)*pool_size);
        int* n_weight2_copy1= inalloc(pool_size);
        int* n_weight2_copy2= inalloc(pool_size);
        real** target_map2_pass= (real**)malloc(sizeof(real*)*pool_size);
        int* n_target_map2_pass1= inalloc(pool_size);
        int* n_target_map2_pass2= inalloc(pool_size);
        real** target_map2_copy= (real**)malloc(sizeof(real*)*pool_size);
        int* n_target_map2_copy1= inalloc(pool_size);
        int* n_target_map2_copy2= inalloc(pool_size);



        // Preallocate exchange variable used for communicating with parallel nodes (when available)
        if ( pool_size > 1 )
        {
            // TODO
        }
        else
        {

        }

        for ( k= 0; k < pool_size; ++k )
        {

        }

        bool** occ_fragment;
        occ_fragment= (bool**)malloc(sizeof(bool*)*n);
        n_occ_fragment= n;
        int* occ_fragment_sizes2= inalloc(n);

        for ( j= n; j <= n + pool_size - 1; ++j )
        {
            int* f= inalloc(n_common_index);
            int n_f= 0;
            for ( i= 0; i < n_common_index; ++i )
                if ( common_index[i] == j )
                    f[n_f++]= i;
            if ( n_f == 0 )
                continue;

            // This breaks the dataset into packets of time and removes values
            // not related to the current time slice in order to conserve memory
            // during the parallel operations.

            occ_fragment[j - n]= bnalloc(n_f*n_occurance_table1);
            occ_fragment_sizes2[j - n]= n_f;

            for ( i= 0; i < n_occurance_table1; ++i )
                for ( k= 0; k < n_f; ++k )
                    occ_fragment[j - n][i*n_f + k]= occurance_table[i*n_occurance_table2 + f[k]];

            int* needed= inalloc(n_occurance_table1);
            for ( i= 0; i < n_occurance_table1; ++i )
            {
                needed[i]= 0;
                for ( k= 0; k < n_f; ++k )
                    needed[i]|= occurance_table[i*n_occurance_table2 + f[k]];
            }

            int* needed_expand= inalloc(n_occurance_table1);
            for ( i= 0; i < n_occurance_table1; ++i )
                needed_expand[i]= expand_map[needed[i]];

            int n_needed= n_occurance_table1;
            uniqueIA(&needed_expand, &n_needed);
            free(needed);
            needed= needed_expand;

            int* list= copyIA(needed, n_needed);
            int n_list= n_needed;

            int max_needed= maxI(needed, n_needed);
            int* remap= inalloc(max_needed);
            seti(remap, max_needed, 0);
            int n_remap= max_needed;

            for ( i= 0; i < n_list; ++i )
                remap[list[i]]= i;

            int common_set= j;

            int* needed_s= inalloc(n_occurance_table1);
            int n_needed_s= n_occurance_table1;
            if ( common_set != 0 )
            {
                for ( i= 0; i < n_occurance_table1; ++i )
                {
                    needed_s[i]= 0;
                    for ( k= 0; k < n_f; ++k )
                        needed_s[i]|= occurance_table[i*n_occurance_table2 + f[k]];
                    needed_s[i]&= common_list[i*n_common_list2 + j];

                    for ( i= 0; i < n_occurance_table1; ++i )
                        needed_s[i]= expand_map[needed_s[i]];
                    uniqueIA(&needed_s, &n_needed_s);
                }
            }
            else
            {
                free(needed_s);
                needed_s= NULL;
                n_needed_s= 0;
            }
            if ( n_needed_s == 0 )
            {
                needed_s= needed;
                n_needed_s= n_needed;
            }

            free(list);
            list= copyIA(needed_s, n_needed_s);
            n_list= n_needed_s;

            int max_needed_s= maxI(needed_s, n_needed_s);
            int* remap_s= inalloc(max_needed_s);
            seti(remap_s, max_needed_s, 0);
            int n_remap_s= max_needed_s;

            for ( i= 0; i < n_list; ++i )
                remap_s[list[i]]= i;

            float* weight2= (float*)malloc(sizeof(float)*n_needed*n_needed_s);
            int n_weight21= n_needed;
            int n_weight22= n_needed_s;
            int l= 0, m;
            for ( i= 0; i < n_weight; ++i )
            {
                m= 0;
                for ( k= 0; k < n_weight; ++k )
                {
                    if ( needed[i] && needed_s[k] )
                        weight2[l*n_needed_s + m++]= weight[i*n_weight + k];
                }
            }

            real* target_map2= rnalloc(n_target_map1 * n_needed);
            l= 0;
            for ( i= 0; i < n_target_map1; ++i )
                if ( needed[i] )
                {
                    for ( j= 0; j < n_target_map2; ++j )
                        target_map2[l++ * n_needed + j]= target_map[i*n_target_map2 + j];
                }


        }
    }

}

#include "openbest-av/buildSpatialMap.h"

#include "openbest-ds/printOut.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/equationSolvers.h"

#define CONSTANT1 50
//#define CONSTANT1 20
#define CONSTANT2 0.7
//#define CONSTANT2 0.4

void buildSpatialMap(float* weight, int n_weight,
                     float* target_map, int n_target_map1, int n_target_map2,
                     bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                     int* expand_map, int n_expand_map,
                     double nugget, berkeleyAverageOptions* options,

                     double** spatial_tableIO, int* n_spatial_table1, int* n_spatial_table2,
                     double** coverage_mapIO, int* n_coverage_map1, int* n_coverage_map2)
{
/*    printArray2FloatFile("weight", weight, n_weight, n_weight);
    printArray2FloatFile("target_map_init", target_map, n_target_map1, n_target_map2);
    printArray2BFile("occurance_table", occurance_table, n_occurance_table1, n_occurance_table2);
    printArrayIFile("expand_map", expand_map, n_expand_map);*/

    tprintf("weight: %d %d\n", n_weight, n_weight);
    tprintf("target_map: %d %d\n", n_target_map1, n_target_map2);
    tprintf("occurance_table: %d %d\n", n_occurance_table1, n_occurance_table2);
    tprintf("expand_map: %d\n", n_expand_map);
    tprintf("nugget: %f\n", nugget);

    int pool_size= 1;
    double mix_term= 1.0 - nugget;
    int len_T= n_occurance_table2;
    int len_S= n_occurance_table1;

    int len_M= n_target_map1;

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

    tprintf("len_T, len_S, len_M: %d %d %d\n", len_T, len_S, len_M);

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

    int i, j, k;
    /*for ( i= 0; i < n_occurance_table1; ++i )
    {
        for ( j= 0; j < n_occurance_table2; ++j )
            fprintf(stderr, "%d ", occurance_table[i*n_occurance_table2 + j]);
        fprintf(stderr, "\n");
    }*/

    int* common_index= inalloc(len_T);
    int n_common_index= len_T;
    seti(common_index, len_T, -1);

    bool* common= bnalloc(n_occurance_table1);
    int n_common= n_occurance_table1;
    bool* base= bnalloc(n_occurance_table1);
    int n_base= n_occurance_table1;

    for ( j= 0; j < len_T; ++j )
    {
        int sum_occurance_table_j= 0;
        for ( i= 0; i < n_occurance_table1; ++i )
            sum_occurance_table_j+= occurance_table[i*n_occurance_table2 + j];

        if ( sum_occurance_table_j < CONSTANT1 )
            common_index[j]= -1;
        else if ( common_index[j] == -1 )
        {
            int m= j;
            while ( m < len_T )
            {
                setb(common, n_common, true);
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
                if ( sum_common < CONSTANT2 * sum_base )
                {
                    --m;
                    break;
                }
                ++m;
            }

            if ( m >= len_T )
                m= len_T - 1;

            int max_common_index= maxI(common_index, j) + 1;
            for ( i= j; i <= m; ++i )
                common_index[i]= max_common_index;

            for ( i= 0; i < n_common_list1; ++i )
            {
                common_list[i*n_common_list2 + common_index[j]]= true;
                for ( k= j; k <= m; ++k )
                {
                    common_list[i*n_common_list2 + common_index[j]]&= occurance_table[i*n_occurance_table2 + k];
                }
            }
        }
    }
    free(base);

    int max_common_index= maxI(common_index, n_common_index) + 1;
    bool* common_list2= bnalloc(n_common_list1 * max_common_index );

    for ( i= 0; i < n_common_list1; ++i )
        for ( j= 0; j < max_common_index; ++j )
            common_list2[i*max_common_index + j]= common_list[i*n_common_list2 + j];
    free(common_list);
    common_list= common_list2;
    n_common_list2= max_common_index;

    tprintf("common_list: %d %d\n", n_common_list1, n_common_list2);
    tprintf("common_index: %d\n", n_common_index);
    /*printArrayIFile("common_index", common_index, n_common_index);
    printArray2BFile("common_list", common_list, n_common_list1, n_common_list2);*/

    float** exports1= (float**)malloc(sizeof(float*)*len_T);
    int* n1_exports1= inalloc(len_T);
    seti(n1_exports1, len_T, 0);
    int* n2_exports1= inalloc(len_T);
    seti(n2_exports1, len_T, 0);
    float** exports2= (float**)malloc(sizeof(float*)*len_T);
    int* n1_exports2= inalloc(len_T);
    int* n2_exports2= inalloc(len_T);
    float** exports3= (float**)malloc(sizeof(float*)*len_T);
    int* n1_exports3= inalloc(len_T);
    int* n2_exports3= inalloc(len_T);

    for ( i= 0; i < len_T; ++i )
        exports1[i]= exports2[i]= exports3[i]= NULL;

    int entries= 0;

    int n;

    int** remap_pass= (int**)malloc(sizeof(int*)*pool_size);
    int* n_remap_pass= inalloc(pool_size);
    int** remap_s_pass= (int**)malloc(sizeof(int*)*pool_size);
    int* n_remap_s_pass= inalloc(pool_size);
    float** weight2_pass;
    int* n_weight2_pass1;
    int* n_weight2_pass2;
    float** weight2_copy;
    int* n_weight2_copy1;
    int* n_weight2_copy2;
    real** target_map2_pass;
    int* n_target_map2_pass1;
    int* n_target_map2_pass2;
    real** target_map2_copy;
    int* n_target_map2_copy1;
    int* n_target_map2_copy2;
    bool* need_inv= bnalloc(pool_size);
    float** B_pass;
    int* n_B_pass;
    int** common_cnts= (int**)malloc(sizeof(int*)*pool_size);
    int* n_common_cnts= inalloc(pool_size);
    float* B= NULL;
    int n_B1;
    int n_B2;
    float** B_copy;
    int* n_B_copy;
    bool** occ_copy;
    int* n_occ_copy1;
    int* n_occ_copy2;

    float* weight2= NULL;
    real* target_map2= NULL;

    int* f;
    int n_f;

    tprintf("max_common_index: %d\n", max_common_index);

    for ( n= -1; n < max_common_index; n+= pool_size )
    {
        tprintf("Begin of Build Spatial Maps %d %f\n", n, (float)n / max_common_index );

        // Preallocate exchange variable used for communicating with parallel nodes (when available)
        if ( pool_size > 1 )
        {
            // TODO
        }
        else
        {
            B_pass= (float**)malloc(sizeof(float*)*pool_size);
            n_B_pass= inalloc(pool_size);
            target_map2_pass= (real**)malloc(sizeof(real*)*pool_size);
            n_target_map2_pass1= inalloc(pool_size);
            n_target_map2_pass2= inalloc(pool_size);
            weight2_pass= (float**)malloc(sizeof(float*)*pool_size);
            n_weight2_pass1= inalloc(pool_size);
            n_weight2_pass2= inalloc(pool_size);
        }

        for ( k= 0; k < pool_size; ++k )
        {

        }

        bool** occ_fragment;
        occ_fragment= (bool**)malloc(sizeof(bool*)*(n+2));
        int n_n_occ_fragment= n + 2;
        int* n_occ_fragment1= inalloc(n_n_occ_fragment);
        seti(n_occ_fragment1, n_n_occ_fragment, 0);
        int* n_occ_fragment2= inalloc(n_n_occ_fragment);
        seti(n_occ_fragment2, n_n_occ_fragment, 0);


        for ( j= n; j <= n + pool_size - 1; ++j )
        {
            f= inalloc(n_common_index);
            n_f= 0;
            for ( i= 0; i < n_common_index; ++i )
                if ( common_index[i] == j )
                    f[n_f++]= i;
            if ( n_f == 0 )
                continue;

            // This breaks the dataset into packets of time and removes values
            // not related to the current time slice in order to conserve memory
            // during the parallel operations.

            occ_fragment[j - n]= (bool*)bnalloc(n_f*n_occurance_table1);
            n_occ_fragment1[j - n]= n_occurance_table1;
            n_occ_fragment2[j - n]= n_f;

            for ( i= 0; i < n_occurance_table1; ++i )
                for ( k= 0; k < n_f; ++k )
                    occ_fragment[j - n][i*n_f + k]= occurance_table[i*n_occurance_table2 + f[k]];

            int* needed= inalloc(n_occurance_table1);
            int n_needed= n_occurance_table1;
            seti(needed, n_needed, 0);
            for ( i= 0; i < n_occurance_table1; ++i )
                for ( k= 0; k < n_f; ++k )
                    needed[i]|= occurance_table[i*n_occurance_table2 + f[k]];

            //printArrayI("needed", needed, n_needed);
            int* needed_expand= inalloc(n_needed);
            int n_needed_expand= 0;
            for ( i= 0; i < n_needed; ++i )
                if ( needed[i] )
                    needed_expand[n_needed_expand++]= expand_map[i];

            n_needed= n_needed_expand;
            uniqueIA(&needed_expand, &n_needed);
            free(needed);
            needed= needed_expand;

            if ( n_needed == 0 )
                continue;

            int* list= copyIA(needed, n_needed);
            int n_list= n_needed;

            int max_needed= maxI(needed, n_needed);
            int* remap= inalloc(max_needed+1);
            int n_remap= max_needed+1;
            seti(remap, n_remap, -1);

            for ( i= 0; i < n_list; ++i )
                remap[list[i]]= i;

            int common_set= j;

            int* needed_s= inalloc(n_occurance_table1);
            int n_needed_s= n_occurance_table1;

            if ( common_set != -1 )
            {
                for ( i= 0; i < n_occurance_table1; ++i )
                {
                    needed_s[i]= 0;
                    for ( k= 0; k < n_f; ++k )
                        needed_s[i]|= occurance_table[i*n_occurance_table2 + f[k]];
                    needed_s[i]&= !common_list[i*n_common_list2 + j];
                }
                n_needed_s= 0;
                for ( i= 0; i < n_occurance_table1; ++i )
                    if ( needed_s[i] )
                        needed_s[n_needed_s++]= expand_map[i];
                uniqueIA(&needed_s, &n_needed_s);
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
            int* remap_s= inalloc(max_needed_s+1);
            int n_remap_s= max_needed_s + 1;
            seti(remap_s, n_remap_s, -1);

            for ( i= 0; i < n_list; ++i )
                remap_s[list[i]]= i;

            int n_weight21;
            int n_weight22;
            createSubArrayIndex2Float(weight, n_weight, n_weight, needed, n_needed, needed_s, n_needed_s, &weight2, &n_weight21, &n_weight22);

            int l= 0, m;

            target_map2= rnalloc(n_target_map1 * n_needed);
            int n_target_map21= n_target_map1;
            int n_target_map22= n_needed;
            l= 0;

            for ( i= 0; i < n_target_map21; ++i )
                for ( k= 0; k < n_target_map22; ++k )
                    target_map2[i * n_target_map22 + k]= target_map[i*n_target_map2 + needed[k]];

            /*printArrayI("needed", needed, n_needed);
            printArray2R("target_map2", target_map2, n_target_map21, n_target_map22);*/

            remap_pass[j-n]= remap;
            n_remap_pass[j-n]= n_remap;
            remap_s_pass[j-n]= remap_s;
            n_remap_s_pass[j-n]= n_remap_s;
            weight2_pass[j-n]= weight2;
            n_weight2_pass1[j-n]= n_weight21;
            n_weight2_pass2[j-n]= n_weight22;
            target_map2_pass[j-n]= target_map2;
            n_target_map2_pass1[j-n]= n_target_map21;
            n_target_map2_pass2[j-n]= n_target_map22;

            if ( j != -1 )
            {
                // Prepare information on the common submatrix used to
                // accelerate the computational performance.

                int* common= inalloc(n_common_list1);
                int n_common= n_common_list1;
                for ( i= 0; i < n_common_list1; ++i )
                    common[i]= common_list[i*n_common_list2 + j];

                float* weight3;
                int n_weight31;
                int n_weight32;

                createSubArrayIndex2Float(weight, n_weight, n_weight, needed, n_needed, needed, n_needed, &weight3, &n_weight31, &n_weight32);

                int l= 0, m;

                int* cnts= inalloc(n_remap);
                int n_cnts= n_remap;
                seti(cnts, n_remap, 0);

                int sum_common= 0;
                for ( i= 0; i < n_common; ++i )
                    sum_common+= common[i];

                int* expanded= inalloc(sum_common);
                int n_expanded= 0;
                for ( i= 0; i < n_common; ++i )
                    if ( common[i] )
                        expanded[n_expanded++]= expand_map[i];

                for ( m= 0; m < n_expanded; ++m )
                    cnts[expanded[m]]= cnts[expanded[m]] + 1;

                int* selection= inalloc(n_cnts);
                int n_selection= n_cnts;
                for ( i= 0; i < n_cnts; ++i )
                    selection[i]= cnts[i] > 0;

                int n_remap_selection= 0;
                for ( i= 0; i < n_selection; ++i )
                    if ( selection[i] )
                        ++n_remap_selection;

                int* remap_selection= inalloc(n_remap_selection);

                k= 0;
                for ( i= 0; i < n_selection; ++i )
                    if ( selection[i] )
                        remap_selection[k++]= remap[i];

                createSubArrayIndex2Float(weight3, n_weight31, n_weight32, remap_selection, n_remap_selection, remap_selection, n_remap_selection, &B, &n_B1, &n_B2);

                k= 0;
                for ( i= 0; i < n_selection; ++i )
                    if ( selection[i] )
                        B[k*n_B2 + k++]= (1.0 + (cnts[i] - 1.0) * mix_term)/(double)(cnts[i]);

                if ( n_B1 * n_B2 * 8 > 100e6 && ! options->clusterMode )
                {
                    need_inv[j-n]= false;
                    float* B_inv;
                    invertMatrixFloatN(B, n_B1, n_B1, &B_inv);
                    free(B);
                    B= B_inv;
                }
                else
                {
                    need_inv[j-n]= true;
                }

                //printArray2FloatFile("B", B, n_B1, n_B2);

                B_pass[j-n]= B;
                n_B_pass[j-n]= n_B1;
                common_cnts[j-n]= cnts;
                n_common_cnts[j-n]= n_cnts;

                free(remap_selection);
                free(expanded);
                free(selection);
            }
            else
            {
                B_pass[j-n]= NULL;
                n_B_pass[j-n]= 0;
                common_cnts[j-n]= NULL;
                n_common_cnts[j-n]= 0;
            }
            // This passes the large arrays to the workers and clears them out
            // of the memory on the main node. This is necessary to avoid out
            // of memory faults on the master node while building up the various
            // pieces.

            if ( (j-n) % 4 == 3 )
            {
                if ( pool_size > 1 )
                {
                    // TODO
                }
            }
        }

        if ( pool_size > 1 )
        {
            // TODO
        }
        else
        {
            B_copy= B_pass;
            n_B_copy= n_B_pass;
            target_map2_copy= target_map2_pass;
            n_target_map2_copy1= n_target_map2_pass1;
            n_target_map2_copy2= n_target_map2_pass2;
            occ_copy= occ_fragment;
            n_occ_copy1= n_occ_fragment1;
            n_occ_copy2= n_occ_fragment2;
            weight2_copy= weight2_pass;
            n_weight2_copy1= n_weight2_pass1;
            n_weight2_copy2= n_weight2_pass2;
        }

        target_map2_pass= NULL;
        n_target_map2_pass1= NULL;
        n_target_map2_pass2= NULL;
        weight2_pass= NULL;
        n_weight2_pass1= NULL;
        n_weight2_pass2= NULL;
        occ_fragment= NULL;
        n_occ_fragment1= NULL;
        n_occ_fragment2= NULL;

        int len_W= n_weight;
        int n_items= (n + pool_size - 1) < max_common_index ? (n + pool_size - 1) : max_common_index;
        n_items= n_items - n + 1;
        int* items= inalloc(n_items+1);

        for ( i= n ; i <= n + n_items; ++i )
            items[i - n]= i;

        if ( pool_size > 1 )
        {
            // TODO
        }
        else
        {
            int* index= inalloc(n_common_index);
            int n_index= 0;

            for ( i= 0; i < n_common_index; ++i )
                if ( common_index[i] == n )
                    index[n_index++]= i;

            //printArrayI("index", index, n_index);

            int* common= inalloc(n_common_list1);
            int n_common= n_common_list1;
            int* common_cnts_= NULL;
            int n_common_cnts_= 0;
            float* Bi= NULL;
            int n_Bi1= 0, n_Bi2= 0;
            if ( n > -1 )
            {
                for ( i= 0; i < n_common_list1; ++i )
                    common[i]= common_list[i*n_common_list2 + n];
                common_cnts_= common_cnts[0];
                n_common_cnts_= n_common_cnts[0];
                if ( need_inv[0] )
                {
                    invertMatrixFloatN(B_copy[0], n_B_copy[0], n_B_copy[0], &Bi);

                    n_Bi1= n_B_copy[0];
                    n_Bi2= n_B_copy[0];
                }
                else
                {
                    Bi= B_copy[0];
                    n_Bi1= n_B_copy[0];
                    n_Bi2= n_B_copy[0];
                }
            }
            else
            {
                free(common);
                common= NULL;
                n_common= 0;
                common_cnts[0]= NULL;
                n_common_cnts[0]= 0;
                Bi= NULL;
                n_Bi1= n_Bi2= 0;
            }

            /*tprintf("common: %d\n", n_common);
            tprintf("common_cnts_: %d\n", n_common_cnts_);
            tprintf("Bi: %d %d\n", n_Bi1, n_Bi2);
            tprintf("occ_copy: %d %d\n", n_occ_copy1[0], n_occ_copy2[0]);
            tprintf("expand_map: %d\n", n_expand_map);
            tprintf("remap_pass: %d\n", n_remap_pass[0]);
            tprintf("remap_s_pass: %d\n", n_remap_s_pass[0]);
            tprintf("weight2: %d %d\n", n_weight2_copy1[0], n_weight2_copy2[0]);
            tprintf("target_map2_copy: %d %d\n", n_target_map2_copy1[0], n_target_map2_copy2[0]);
            tprintf("%d %d\n", len_W, len_M);
            getchar();*/
            computeSpatialMapBlock(common, n_common,
                                   common_cnts_, n_common_cnts_,
                                   Bi, n_Bi1, n_Bi2,
                                   occ_copy[0], n_occ_copy1[0], n_occ_copy2[0],
                                   expand_map, n_expand_map,
                                   remap_pass[0], n_remap_pass[0],
                                   remap_s_pass[0], n_remap_s_pass[0],
                                   weight2_copy[0], n_weight2_copy1[0], n_weight2_copy2[0],
                                   target_map2_copy[0], n_target_map2_copy1[0], n_target_map2_copy2[0],
                                   len_M, len_W,
                                   index, n_index,
                                   mix_term,
                                   options,
                                   exports1, n1_exports1, n2_exports1,
                                   exports2, n1_exports2, n2_exports2,
                                   exports3, n1_exports3, n2_exports3,
                                   f, n_f);

            /*for ( i= 0; i < n_f; ++i )
                printf("%d,%d ", n1_exports1[f[i]], n2_exports1[f[i]]);*/
            for ( i= 0; i < n_f; ++i )
                entries+= f[i];

            if ( index ) free(index);
            if ( common ) free(common);
            if ( Bi ) free(Bi);
            if ( occ_copy[0] ) free(occ_copy[0]);
            if ( remap_pass[0] ) free(remap_pass[0]);
            if ( remap_s_pass[0] ) free(remap_s_pass[0] );
            if ( weight2_copy[0] ) free(weight2_copy[0] );
            if ( target_map2_copy[0] ) free(target_map2_copy[0]);
        }

        // Periodically consolidate and compress the returned values to save
        // memory.
        if ( ( entries > 100 || maxI(items, n_items) == maxI(common_index, n_common_index)) && !table_mode )
        {
            // TODO
        }
    }

    tprintf("End of Build Spatial Maps\n");

    if ( !table_mode )
    {
        // TODO
    }
    else
    {
        // Operations for values that are already globally integrated

        int len_SM= n1_exports1[0];

        *spatial_tableIO= dnalloc(len_T*len_SM);


        int sum= 0;
        for ( i= 0; i < len_T; ++i )
        {
            sum= 0;
            for ( j= 0; j < n1_exports1[i]; ++j )
                if ( exports1[i][j] != 0 )
                    sum+= 1;
        }

        for ( i= 0; i < len_SM; ++i )
            for ( j= 0; j < len_T; ++j )
                if ( n1_exports1[j] == 0 || n1_exports1[j] == 2 )
                    (*spatial_tableIO)[i*len_T+j]= 0;
                else
                    (*spatial_tableIO)[i*len_T+j]= exports1[j][i];
        *n_spatial_table1= len_SM;
        *n_spatial_table2= len_T;
    }

    for ( i= 0; i < len_T; ++i )
    {
        if ( exports1[i] )  free(exports1[i]);
        if ( exports2[i] )  free(exports2[i]);
        if ( exports3[i] )  free(exports3[i]);
    }

    if ( exports1 ) free(exports1);
    if ( exports2 ) free(exports2);
    if ( exports3 ) free(exports3);

    if ( n1_exports1 )  free(n1_exports1);
    if ( n2_exports1 )  free(n2_exports1);
    if ( n1_exports2 )  free(n1_exports2);
    if ( n2_exports2 )  free(n2_exports2);
    if ( n1_exports3 )  free(n1_exports3);
    if ( n2_exports3 )  free(n2_exports3);

    for ( i= 0; i < pool_size; ++i )
    {
        if ( common_cnts[i] )
            free(common_cnts[i]);
    }
    if ( remap_pass )
        free(remap_pass);
    if ( remap_s_pass )
        free(remap_s_pass);
    if ( common_cnts )
        free(common_cnts);
    if ( n_remap_pass )
        free(n_remap_pass);
    if ( n_remap_s_pass )
        free(n_remap_s_pass);
    if ( n_common_cnts )
        free(n_common_cnts);

    if ( common_list ) free(common_list);
    if ( common_index) free(common_index);
    if ( need_inv ) free(need_inv);
}



void computeSpatialMapBlock(int* common, int n_common,
                            int* common_cnts, int n_common_cnts,
                            float* Bi, int n_Bi1, int n_Bi2,
                            bool* occ_fragment, int n_occ_fragment1, int n_occ_fragment2,
                            int* expand_map, int n_expand_map,
                            int* remap, int n_remap,
                            int* remap_s, int n_remap_s,
                            float* weight, int n_weight1, int n_weight2,
                            real* target_map, int n_target_map1, int n_target_map2,
                            int len_M, int len_W,
                            int* index, int n_index,
                            double mix_term,
                            berkeleyAverageOptions* options,


                            float** exports1IO, int* n1_exports1IO, int* n2_exports1IO,
                            float** exports2IO, int* n1_exports2IO, int* n2_exports2IO,
                            float** exports3IO, int* n1_exports3IO, int* n2_exports3IO,
                            int* f1, int n_f1)
{
    /*tprintf("n_common: %d, n_common_cnts: %d, n_Bi1, n_Bi2: %d x %d\n", n_common, n_common_cnts, n_Bi1, n_Bi2);
    tprintf("n_occ_fragment1 x 2: %d x %d, n_expand_map: %d, n_remap: %d, n_remap_s: %d\n", n_occ_fragment1, n_occ_fragment2, n_expand_map, n_remap, n_remap_s);
    tprintf("n_weight1,2: %d x %d, n_target_map1,2: %d x %d\n", n_weight1, n_weight2, n_target_map1, n_target_map2);
    tprintf("len_M: %d, len_W: %d, n_index: %d, mix_term: %f\n", len_M, len_W, n_index, mix_term);*/

    /*printArrayIFile("common", common, n_common);
    printArrayIFile("common_cnts", common_cnts, n_common_cnts);
    printArray2FloatFile("Bi", Bi, n_Bi1, n_Bi2);
    printArray2BFile("occ_fragment", occ_fragment, n_occ_fragment1, n_occ_fragment2);
    printArrayIFile("expand_map", expand_map, n_expand_map);
    printArrayIFile("remap", remap, n_remap);
    printArrayIFile("remap_s", remap_s, n_remap_s);
    printArray2FloatFile("weight", weight, n_weight1, n_weight2);
    printArray2RFile("target_map", target_map, n_target_map1, n_target_map2);*/

    bool table_mode;
    if ( len_M == 1 )
        table_mode= true;
    else
        table_mode= false;

    int loops= n_occ_fragment2;

    float** exports1= (float**)malloc(sizeof(float*)*loops);
    int* n1_exports1= inalloc(loops);
    int* n2_exports1= inalloc(loops);
    float** exports2= (float**)malloc(sizeof(float*)*loops);
    int* n1_exports2= inalloc(loops);
    int* n2_exports2= inalloc(loops);
    float** exports3= (float**)malloc(sizeof(float*)*loops);
    int* n1_exports3= inalloc(loops);
    int* n2_exports3= inalloc(loops);

    int i, j, k;

    for ( i= 0; i < loops; ++i )
        exports1[i]= exports2[i]= exports3[i]= NULL;

    for ( j= 0; j < loops; ++j )
    {
        bool* f= fnalloc(n_occ_fragment1);
        int n_f= n_occ_fragment1;

        for ( i= 0; i < n_occ_fragment1; ++i )
            f[i]= occ_fragment[i*n_occ_fragment2 + j];

        int any_f= 0;
        for ( i= 0; i < n_f; ++i )
            any_f|= f[i];

        if ( !any_f )
        {
            exports1[j]= (float*)malloc(sizeof(float)*2);
            exports1[j][0]= 0.0;
            exports1[j][1]= 0.0;
            n1_exports1[j]= 2;
            n2_exports1[j]= 1;

            exports2[j]= (float*)malloc(sizeof(float)*1);
            exports2[j][0]= 0;
            n1_exports2[j]= 1;
            n2_exports2[j]= 1;

            exports3[j]= (float*)malloc(sizeof(float)*1);
            exports3[j][0]= 0;
            n1_exports3[j]= 1;
            n2_exports3[j]= 1;

            continue;
        }

        if ( common != NULL && n_common != 0 )
        {
            for ( i= 0; i < n_f; ++i )
                f[i]= f[i] & !common[i];
        }

        int* cnts= inalloc(n_remap);
        int n_cnts= n_remap;
        seti(cnts, n_cnts, 0);

        int sum_f= 0;
        for ( i= 0; i < n_f; ++i )
            sum_f+= f[i];

        int* expanded= inalloc(sum_f);
        int n_expanded= 0;
        for ( i= 0; i < n_f; ++i )
            if ( f[i] )
                expanded[n_expanded++]= expand_map[i];

        //printArrayI("expanded", expanded, n_expanded);

        int m;
        for ( m= 0; m < n_expanded; ++m )
            cnts[expanded[m]]= cnts[expanded[m]] + 1;

        bool* selection= bnalloc(n_cnts);
        int n_selection= n_cnts;
        int sum_selection= 0;
        for ( i= 0; i < n_selection; ++i )
        {
            selection[i]= cnts[i] > 0;
            sum_selection+= selection[i];
        }

        //printArrayB("selection", selection, n_selection);

        // Select rows of weight function corresponding to active stations and
        // add in the nugget.

        int* re_select= inalloc(sum_selection);
        int n_re_select= 0;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                re_select[n_re_select++]= remap[i];

        int* re_select_s= inalloc(sum_selection);
        int n_re_select_s= 0;
        for ( i= 0; i < n_selection && i < n_remap_s; ++i )
            if ( selection[i] )
                re_select_s[n_re_select_s++]= remap_s[i];

        //printArrayI("re_select", re_select, n_re_select);

        float* B_b;
        int n_B_b1;
        int n_B_b2;
        if ( common != NULL && n_common != 0 )
        {
            int* remap_cc= inalloc(n_common_cnts);
            int n_remap_cc= 0;

            for ( i= 0; i < n_common_cnts; ++i )
                if ( common_cnts[i] >= 1 )
                    remap_cc[n_remap_cc++]= remap[i];

            createSubArrayIndex2Float(weight, n_weight1, n_weight2, remap_cc, n_remap_cc, re_select_s, n_re_select_s, &B_b, &n_B_b1, &n_B_b2);
        }
        else
        {
            B_b= NULL;
            n_B_b1= 0;
            n_B_b2= 0;
        }

        float* B_d;
        int n_B_d1;
        int n_B_d2;

        createSubArrayIndex2Float(weight, n_weight1, n_weight2, re_select, n_re_select, re_select_s, n_re_select_s, &B_d, &n_B_d1, &n_B_d2);

        //printArrayB("selection", selection, n_selection);
        int k= 0;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                B_d[k*n_B_d2 + k++]= (1.0 + (cnts[i] - 1.0) * mix_term)/cnts[i];

        //printArray2Float("B_d", B_d, n_B_d1, n_B_d2);

        // Target weights for active stations
        int* fx= inalloc(n_common_cnts + n_selection);
        int n_fx= 0;
        for ( i= 0; i < n_common_cnts; ++i )
            if ( common_cnts[i] > 0 )
                fx[n_fx++]= i;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] > 0 )
                fx[n_fx++]= i;

   //     printArrayI("remap", remap, n_remap);
     //   printArrayI("fx", fx, n_fx);

        float* A1= (float*)malloc(sizeof(float)*n_fx*n_target_map1);
        int n_A11= n_target_map1;
        int n_A12= n_fx;
        for ( k= 0; k < n_target_map1; ++k )
            for ( i= 0; i < n_fx; ++i )
                A1[k*n_fx + i]= target_map[k*n_target_map2 + remap[fx[i]]];

        float* f_weights_map;
        int n_f_weights_map1;
        int n_f_weights_map2;
        if ( !table_mode )
        {
            float* Bi2;
            int n_Bi21;
            int n_Bi22;
            if ( common != NULL && n_common != 0 )
            {
                invWithPartial2(B_b, n_B_b1, n_B_b2, B_d, n_B_d1, n_B_d2, Bi, n_Bi1, n_Bi2, &Bi2, &n_Bi21, &n_Bi22);
            }
            else
            {
                invertMatrixFloatN(B_d, n_B_d1, n_B_d2, &Bi2);
                n_Bi21= n_B_d2;
                n_Bi22= n_B_d1;
            }
            matrixMultiplicationNF(A1, n_A11, n_A12, Bi2, n_Bi21, n_Bi22, &f_weights_map, &n_f_weights_map1, &n_f_weights_map2);
        }
        else
        {
            // Compute A1/B
            // TODO
            if ( Bi != NULL && n_Bi1 != 0 && n_Bi2 != 0 )
            {
                // Quick division with known partial inverse
                float* A1T= (float*)malloc(sizeof(float)*n_A11*n_A12);
                float* BiT= (float*)malloc(sizeof(float)*n_Bi1*n_Bi2);

                for ( i= 0; i < n_A11; ++i )
                    for ( k= 0; k < n_A12; ++k )
                        A1T[k*n_A11 + i]= A1[i*n_A12 + k];
                for ( i= 0; i < n_Bi1; ++i )
                    for ( k= 0; k < n_Bi2; ++k )
                        BiT[k*n_Bi1 + i]= Bi[i*n_Bi2 + k];

                int n_A1= n_A11 > n_A12 ? n_A11 : n_A12;
                /*printArray2Float("A1T", A1T, n_A11, n_A12);
                printArray2Float("BiT", BiT, n_Bi1, n_Bi2);
                printArray2Float("B_b", B_b, n_B_b1, n_B_b2);
                printArray2Float("B_d", B_d, n_B_d1, n_B_d2);*/
                mldivideWithParitalInverse2(B_b, n_B_b1, n_B_b2, B_d, n_B_d1, n_B_d2, A1T, n_A1, BiT, n_Bi2, n_Bi1, &f_weights_map, &n_f_weights_map1, &n_f_weights_map2);
            }
            else
            {
                // We do direct division when the common portion is too small to be worth worrying about
                f_weights_map= (float*)malloc(sizeof(float)*n_B_d1);
                double* ftmp= dnalloc(n_B_d1);

                double* B_dT= dnalloc(n_B_d1 * n_B_d2);

                double* Atmp= dnalloc(n_A11*n_A12);

                for ( i= 0; i < n_A11*n_A12; ++i )
                    Atmp[i]= A1[i];
                for ( i= 0; i < n_B_d1; ++i )
                    for ( k= 0; k < n_B_d2; ++k )
                    {
                        B_dT[k*n_B_d1 + i]= B_d[i*n_B_d2 + k];
                    }

                int n_A1= n_A11 > n_A12 ? n_A11 : n_A12;

                solveLinEqHD(B_dT, n_B_d2, n_B_d1, Atmp, 1, ftmp);

                n_f_weights_map1= n_B_d1;
                n_f_weights_map2= 1;

                for ( i= 0; i < n_f_weights_map1*n_f_weights_map2; ++i )
                    f_weights_map[i]= ftmp[i];
            }
            //printArray2Float("f_weights_map", f_weights_map, n_f_weights_map2, n_f_weights_map1);
        }
        if ( A1 )
        {
            free(A1);
            A1= NULL;
        }
        if ( B_b )
        {
            free(B_b);
            B_b= NULL;
        }
        if ( B_d )
        {
            free(B_d);
            B_d= NULL;
        }

        if ( !table_mode )
        {
            // TODO
        }

        int tmp_cnts= 0;
        for ( i= 0; i < n_common_cnts; ++i )
            if ( common_cnts[i] >= 1 )
                ++tmp_cnts;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                ++tmp_cnts;

        int* cnts_n= inalloc(tmp_cnts);
        int n_cnts_n= 0;
        for ( i= 0; i < n_common_cnts; ++i )
            if ( common_cnts[i] >= 1 )
                cnts_n[n_cnts_n++]= common_cnts[i];
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                cnts_n[n_cnts_n++]= cnts[i];

        if ( cnts )
        {
            free(cnts);
            cnts= NULL;
        }
        cnts= cnts_n;
        n_cnts= n_cnts_n;

        // TODO bsxfun
        int n_f_weights_map= n_f_weights_map1 > n_f_weights_map2 ? n_f_weights_map1 : n_f_weights_map2;

        //tprintf("f_weights_map, cnts: %d %d\n", n_f_weights_map, n_cnts);
        for ( i= 0; i < n_f_weights_map; ++i )
            f_weights_map[i]/= cnts[i];

        if ( !table_mode )
        {
            // TODO
        }

        if ( !table_mode )
        {
            // Store in spatial table
        }
        else
        {
            // Store in spatial table
            float* temp_reduced= (float*)malloc(sizeof(float)*len_M*len_W);
            for ( i= 0; i < len_M*len_W; ++i )
                temp_reduced[i]= 0;

            int* indices= inalloc(n_common_cnts + n_selection);
            int n_indices= 0;
            for ( i= 0; i < n_common_cnts; ++i )
                if ( common_cnts[i] >= 1 )
                    indices[n_indices++]= i;
            for ( i= 0; i < n_selection; ++i )
                if ( selection[i] )
                    indices[n_indices++]= i;

            for ( i= 0; i < len_M; ++i )
                for ( k= 0; k < n_indices; ++k )
                {
                    temp_reduced[i*len_W + indices[k]]= f_weights_map[k];
                }

            float* temp_full= (float*)malloc(sizeof(float)*len_M*n_expand_map);
            for ( i= 0; i < len_M*n_expand_map; ++i )
                temp_full[i]= 0;

            int l= 0;
            for ( i= 0; i < len_M; ++i )
            {
                l= 0;
                for ( k= 0; k < n_expand_map; ++k )
                    if ( f[k] )
                        temp_full[i*n_expand_map + k]= temp_reduced[i*len_W + expanded[l++]];
            }

            for ( i= 0; i < len_M; ++i )
                for ( k= 0; k < n_common; ++k )
                {
                    if ( common[k] )
                        temp_full[i*n_expand_map + k]= temp_reduced[i*len_W + expand_map[k]];
                }

            float* temp_fullT= (float*)malloc(sizeof(float)*len_M*n_expand_map);
            for ( i= 0; i < len_M; ++i )
                for ( k= 0; k < n_expand_map; ++k )
                    temp_fullT[k*len_M + i]= temp_full[i*n_expand_map + k];

            /*printArray2FloatFile("temp_fullT", temp_fullT, len_M, n_expand_map);*/

            exports1[j]= temp_fullT;
            n1_exports1[j]= n_expand_map;
            n2_exports1[j]= len_M;
        }

    }
    for ( i= 0; i < n_f1; ++i )
    {
        exports1IO[f1[i]]= exports1[i];
        exports2IO[f1[i]]= exports2[i];
        exports3IO[f1[i]]= exports3[i];
        n1_exports1IO[f1[i]]= n1_exports1[i];
        n2_exports1IO[f1[i]]= n2_exports1[i];
        n1_exports2IO[f1[i]]= n1_exports2[i];
        n2_exports2IO[f1[i]]= n2_exports2[i];
        n1_exports3IO[f1[i]]= n1_exports3[i];
        n2_exports3IO[f1[i]]= n2_exports3[i];
    }
    free(exports1);
    free(exports2);
    free(exports3);
    free(n1_exports1);
    free(n2_exports1);
    free(n1_exports2);
    free(n2_exports2);
    free(n1_exports3);
    free(n2_exports3);
}

void mldivideWithParitalInverse2(float* M_C, int n_M_C1, int n_M_C2,
                                 float* M_D, int n_M_D1, int n_M_D2,
                                 float* T, int n_T,
                                 float* Ai, int n_Ai1, int n_Ai2,

                                 float** XIO, int* n_X1IO, int* n_X2IO)
{
    // Helper function for determining a matrix division solution given a known
    // partial inverse.



    float* M_B= (float*)malloc(sizeof(float)*n_M_C1*n_M_C2);
    int n_M_B1= n_M_C2;
    int n_M_B2= n_M_C1;

    //tprintf("Ai, T, M_B: %d,%d %d %d,%d\n", n_Ai1, n_Ai2, n_T, n_M_B1, n_M_B2);

    int i, j, k, l;
    for ( i= 0; i < n_M_C1; ++i )
        for ( j= 0; j < n_M_C2; ++j )
            M_B[j*n_M_C1 + i]= M_C[i*n_M_C2 + j];

    float* T1= (float*)malloc(sizeof(float)*n_M_B2);
    int n_T1= n_M_B2;
    for ( i= 0; i < n_M_B2; ++i )
        T1[i]= T[i];
    float* T2= (float*)malloc(sizeof(float)*(n_T - n_M_B2));
    int n_T2= n_T - n_M_B2;
    for ( i= n_M_B2; i < n_T; ++i )
        T2[i - n_M_B2]= T[i];

    float* X;
    if ( M_D == NULL || n_M_D1 == 0 || n_M_D2 == 0 )
    {
        // MATRIX AND VECTOR DIMENSIONS DO NOT AGREE!!!
        //tprintf("T: %d, Ai: %d x %d\n", n_T, n_Ai1, n_Ai2);
        X= (float*)malloc(sizeof(float)*n_Ai1);

        for ( i= 0; i < n_Ai1; ++i )
        {
            X[i]= 0;
            for ( j= 0; j < n_Ai2; ++j )
                X[i]+= T[j] * Ai[i*n_Ai2 + j];
        }
        *XIO= X;
        *n_X1IO= n_Ai1;
        *n_X2IO= 1;
        return;
    }

    float* Y;
    int n_Y1;
    int n_Y2;
    /*printArray2Float("Ai", Ai, n_Ai1, n_Ai2);
    printArrayFloat("T1", T1, n_T1);*/
    matrixMultiplicationNF(Ai, n_Ai1, n_Ai2, T1, n_T1, 1, &Y, &n_Y1, &n_Y2);

    int n_Y= n_Y1 > n_Y2 ? n_Y1 : n_Y2;

    float* MBAi;
    int n_MBAi1;
    int n_MBAi2;

    matrixMultiplicationNF(M_B, n_M_B1, n_M_B2, Ai, n_Ai1, n_Ai2, &MBAi, &n_MBAi1, &n_MBAi2);

    float* MBAiMC;
    int n_MBAiMC1;
    int n_MBAiMC2;

    matrixMultiplicationNF(MBAi, n_MBAi1, n_MBAi2, M_C, n_M_C1, n_M_C2, &MBAiMC, &n_MBAiMC1, &n_MBAiMC2);

    float* MBY;
    int n_MBY1;
    int n_MBY2;

    matrixMultiplicationNF(M_B, n_M_B1, n_M_B2, Y, n_Y, 1, &MBY, &n_MBY1, &n_MBY2);
    //printArray2Float("M_D", M_D, n_M_D1, n_M_D2);
    //printArray2Float("MBAiMC", MBAiMC, n_MBAiMC1, n_MBAiMC2);

    int n_MBY= n_MBY1 > n_MBY2 ? n_MBY1 : n_MBY2;

    double* A= dnalloc(n_M_D1*n_M_D2);
    int n_A1= n_M_D1;
    int n_A2= n_M_D2;
    for ( i= 0; i < n_M_D1 * n_M_D2; ++i )
        A[i]= M_D[i] - MBAiMC[i];

    double* b= dnalloc(n_MBY);
    for ( i= 0; i < n_MBY; ++i )
        b[i]= T2[i] - MBY[i];
    int n_b= n_MBY;

    double* X2tmp= dnalloc(n_A2);
    int n_X2tmp= n_A2;

    //tprintf("A: %d,%d b: %d\n", n_A1, n_A2, n_b);
    //printArray2D("A", A, n_A1, n_A2);
    //printArrayD("b", b, n_A1);
    solveLinEqHD(A, n_A1, n_A2, b, 1, X2tmp);

    //printArrayD("X2d", X2tmp, n_X2tmp);
    float* X2= (float*)malloc(sizeof(float)*n_X2tmp);
    int n_X2= n_X2tmp;
    for ( i= 0; i < n_X2tmp; ++i )
        X2[i]= X2tmp[i];

    //printArrayFloat("X2", X2, n_X2);

    float* X1= (float*)malloc(sizeof(float)*n_Y);
    int n_X1= n_Y;
    if ( n_Ai1 > n_X2 )
    {
        float* tmp;
        int n_tmp1;
        int n_tmp2;
        matrixMultiplicationNF(M_C, n_M_C1, n_M_C2, X2, n_X2, 1, &tmp, &n_tmp1, &n_tmp2);

        float* res;
        int n_res1;
        int n_res2;
        matrixMultiplicationNF(Ai, n_Ai1, n_Ai2, tmp, n_tmp1, n_tmp2, &res, &n_res1, &n_res2);
        int n_res= n_res1 > n_res2 ? n_res1 : n_res2;

        /*printf("X, Y, res: %d %d %d\n", n_X1, n_Y, n_res);
        printArrayFloat("Y", Y, n_Y);
        printArrayFloat("res", res, n_res);*/
        for ( i= 0; i < n_Y; ++i )
            X1[i]= Y[i] - res[i];
    }
    else
    {
        float* tmp;
        int n_tmp1;
        int n_tmp2;
        matrixMultiplicationNF(Ai, n_Ai1, n_Ai2, M_C, n_M_C1, n_M_C2, &tmp, &n_tmp1, &n_tmp2);

        float* res;
        int n_res1;
        int n_res2;
        matrixMultiplicationNF(tmp, n_tmp1, n_tmp2, X2, n_X2, 1, &res, &n_res1, &n_res2);
        int n_res= n_res1 > n_res2 ? n_res1 : n_res2;

        for ( i= 0; i < n_Y; ++i )
            X1[i]= Y[i] - res[i];
    }

    X= (float*)malloc(sizeof(float)*(n_X1 + n_X2));
    for ( i= 0; i < n_X1; ++i )
        X[i]= X1[i];
    for ( ; i < n_X2 + n_X1; ++i )
        X[i]= X2[i-n_X1];

    *XIO= X;
    *n_X1IO= n_X1 + n_X2;
    *n_X2IO= 1;
}

void invWithPartial2(float* M_B, int n_M_B1, int n_M_B2,
                     float* M_D, int n_M_D1, int n_M_D2,
                     float* Ai, int n_Ai1, int n_Ai2,
                     float** resIO, int* n_res1IO, int* n_res2IO)
{
    /*printArray2Float("M_B", M_B, n_M_B1, n_M_B2);
    printArray2Float("M_D", M_D, n_M_D1, n_M_D2);
    printArray2Float("Ai", Ai, n_Ai1, n_Ai2);*/
    //getchar();

    // Helper function for computing an inverse matrix given a known partial inverse
    float* M_C= (float*)malloc(sizeof(float)*n_M_B1*n_M_B2);
    int i, j, k;
    int n_M_C1= n_M_B2;
    int n_M_C2= n_M_B1;

    for ( i= 0; i < n_M_B1; ++i )
        for ( j= 0; j < n_M_B2; ++j )
            M_C[j*n_M_C2 + i]= M_B[i*n_M_B2 + j];

    float* SS;
    int n_SS1, n_SS2;
    matrixMultiplicationNF(Ai, n_Ai1, n_Ai2, M_B, n_M_B1, n_M_B2, &SS, &n_SS1, &n_SS2);
    float* M_CSS;
    int n_M_CSS1, n_M_CSS2;

    matrixMultiplicationNF(M_C, n_M_C1, n_M_C2, SS, n_SS1, n_SS2, &M_CSS, &n_M_CSS1, &n_M_CSS2);

    float* SP= (float*)malloc(sizeof(float)*n_M_D1*n_M_D2);
    int n_SP1= n_M_D1;
    int n_SP2= n_M_D2;

    for ( i= 0; i < n_SP1*n_SP2; ++i )
        SP[i]= M_D[i] - M_CSS[i];

    float* SPi;
    int n_SPi1;
    int n_SPi2;
    invertMatrixFloatN(SP, n_SP1, n_SP2, &SPi);
    n_SPi1= n_SP2;
    n_SPi2= n_SP1;

    float* SPiM_C;
    int n_SPiM_C1;
    int n_SPiM_C2;

    matrixMultiplicationNF(SPi, n_SPi1, n_SPi2, M_C, n_M_C1, n_M_C2, &SPiM_C, &n_SPiM_C1, &n_SPiM_C2);

    float* RR;
    int n_RR1;
    int n_RR2;

    matrixMultiplicationNF(SPiM_C, n_SPiM_C1, n_SPiM_C2, Ai, n_Ai1, n_Ai2, &RR, &n_RR1, &n_RR2);

    float* SSRR;
    int n_SSRR1;
    int n_SSRR2;

    matrixMultiplicationNF(SS, n_SS1, n_SS2, RR, n_RR1, n_RR2, &SSRR, &n_SSRR1, &n_SSRR2);

    float* SSSPi;
    int n_SSSPi1;
    int n_SSSPi2;

    matrixMultiplicationNF(SS, n_SS1, n_SS2, SPi, n_SPi1, n_SPi2, &SSSPi, &n_SSSPi1, &n_SSSPi2);

    float* AiSSRR= (float*)malloc(sizeof(float)*n_Ai1 * n_Ai2);
    for ( i= 0; i < n_Ai1*n_Ai2; ++i )
        AiSSRR[i]= Ai[i] + SSRR[i];
    for ( i= 0; i < n_SSSPi1*n_SSSPi2; ++i )
        SSSPi[i]= -SSSPi[i];
    for ( i= 0; i < n_RR1*n_RR2; ++i )
        RR[i]= -RR[i];

    float* res;
    int n_res1= n_Ai1 + n_RR1;
    int n_res2= n_Ai2 + n_SSSPi2;
    res= (float*)malloc(sizeof(float)*n_res1*n_res2);

    int ii;
    int jj;
    for ( i= 0; i < n_res1; ++i )
        for ( j= 0; j < n_res2; ++j )
        {
            if ( i < n_Ai1 && j < n_Ai2 )
            {
                res[i*n_res2 + j]= AiSSRR[i*n_Ai2 + j];
            }
            else if ( i < n_Ai1 )
            {
                jj= j - n_Ai2;
                res[i*n_res2 + j]= SSRR[i*n_SSRR2 + jj];
            }
            else if ( i >= n_Ai1 && j < n_Ai2 )
            {
                ii= i - n_Ai1;
                res[i*n_res2 + j]= RR[ii*n_RR2 + j];
            }
            else
            {
                ii= i - n_Ai1;
                jj= j - n_Ai2;
                res[i*n_res2 + j]= SPi[ii*n_SPi1 + jj];
            }
        }

    *resIO= res;
    *n_res1IO= n_res1;
    *n_res2IO= n_res2;
}

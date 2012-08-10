#include "openbest-av/buildSpatialMap.h"

#include "openbest-ds/printOut.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/equationSolvers.h"

//#define CONSTANT1 50
#define CONSTANT1 5
//#define CONSTANT2 0.7
#define CONSTANT2 0.4

void buildSpatialMap(float* weight, int n_weight,
                     float* target_map, int n_target_map1, int n_target_map2,
                     bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                     int* expand_map, int n_expand_map,
                     double nugget, berkeleyAverageOptions* options,

                     double** spatial_tableIO, int* n_spatial_table1, int* n_spatial_table2,
                     double** coverage_mapIO, int* n_coverage_map1, int* n_coverage_map2)
{
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
    seti(common_index, len_T, 0);

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
            common_index[j]= 0;
        else if ( common_index[j] == 0 )
        {
            printf("ci%d ", j);
            //dea();
            int m= j;
            while ( m < len_T )
            {
                //deb();
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
                //printf("%d:%d ", sum_common, sum_base);
                if ( sum_common < CONSTANT2 * sum_base )
                {
                    --m;
                    break;
                }
                ++m;
            }

            //dec();
            if ( m >= len_T )
                m= len_T - 1;

            //printf("%d,%d ", j, m);
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

    tprintf("common_index:\n");
    for ( i= 0; i < n_common_index; ++i )
        printf("%d ", common_index[i]);
    printf("\n");

    int max_common_index= maxI(common_index, n_common_index) + 1;
    bool* common_list2= bnalloc(n_common_list1 * max_common_index );

    for ( i= 0; i < n_common_list1; ++i )
        for ( j= 0; j < max_common_index; ++j )
            common_list2[i*max_common_index + j]= common_list[i*n_common_list2 + j];
    free(common_list);
    common_list= common_list2;
    n_common_list2= max_common_index;


    int n;

    int** remap_pass= (int**)malloc(sizeof(int*)*pool_size);
    int* n_remap_pass= inalloc(pool_size);
    int** remap_s_pass= (int**)malloc(sizeof(int*)*pool_size);
    int* n_remap_s_pass= inalloc(pool_size);
    float** weight2_pass= (float**)malloc(sizeof(float*)*pool_size);
    int* n_weight2_pass1= inalloc(pool_size);
    int* n_weight2_pass2= inalloc(pool_size);
    float** weight2_copy;
    int* n_weight2_copy1;
    int* n_weight2_copy2;
    real** target_map2_pass= (real**)malloc(sizeof(real*)*pool_size);
    int* n_target_map2_pass1= inalloc(pool_size);
    int* n_target_map2_pass2= inalloc(pool_size);
    real** target_map2_copy;
    int* n_target_map2_copy1;
    int* n_target_map2_copy2;
    bool* need_inv= bnalloc(pool_size);
    float** B_pass= (float**)malloc(sizeof(float*)*pool_size);
    int* n_B_pass= inalloc(pool_size);
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
    float* target_map2= NULL;

    tprintf("expand_map: \n");
    for ( i= 0; i < n_expand_map; ++i )
        printf("%d ", expand_map[i]);

    tprintf("max_common_index: %d\n", max_common_index);
    for ( n= 0; n < max_common_index; n+= pool_size )
    {
        tprintf("Begin of Build Spatial Maps %d %f\n", n, (float)n / max_common_index );

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
        occ_fragment= (bool**)malloc(sizeof(bool*)*(n+1));
        int n_n_occ_fragment= n + 1;
        int* n_occ_fragment= inalloc(n_n_occ_fragment);
        seti(n_occ_fragment, n_n_occ_fragment, 0);

        tprintf("j iteration %d\n", j);
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

            /*tprintf("n_f: %d\n", n_f);
            for ( i= 0; i < n_f; ++i )
                printf("%d ", f[i]);
            printf("\n"); fflush(stdout);*/
            occ_fragment[j - n]= bnalloc(n_f*n_occurance_table1);
            n_occ_fragment[j - n]= n_f;

            for ( i= 0; i < n_occurance_table1; ++i )
                for ( k= 0; k < n_f; ++k )
                    occ_fragment[j - n][i*n_f + k]= occurance_table[i*n_occurance_table2 + f[k]];

            int* needed= inalloc(n_occurance_table1);
            seti(needed, n_occurance_table1, 0);
            for ( i= 0; i < n_occurance_table1; ++i )
                for ( k= 0; k < n_f; ++k )
                    needed[i]|= occurance_table[i*n_occurance_table2 + f[k]];

            /*printf("needed: ");
            for ( i= 0; i < n_occurance_table1; ++i )
                printf("%d ", needed[i]);
            printf("\n"); fflush(stdout);*/

            int* needed_expand= inalloc(n_occurance_table1);
            int n_needed_expand= 0;
            for ( i= 0; i < n_occurance_table1; ++i )
                if ( needed[i] )
                    needed_expand[n_needed_expand++]= expand_map[i];

            int n_needed= n_needed_expand;
            uniqueIA(&needed_expand, &n_needed);
            free(needed);
            needed= needed_expand;

            /*printf("needed: ");
            for ( i= 0; i < n_needed; ++i )
                printf("%d ", needed[i]);
            printf("\n"); fflush(stdout);*/

            if ( n_needed == 0 )
                continue;

            int* list= copyIA(needed, n_needed);
            int n_list= n_needed;

            //tprintf("---\n");
            int max_needed= maxI(needed, n_needed);
            int* remap= inalloc(max_needed+1);
            int n_remap= max_needed;
            seti(remap, max_needed, 0);

            /*printf("list %d: ", max_needed);
            for ( i= 0; i < n_list; ++i )
                printf("%d ", list[i]);
            printf("\n"); fflush(stdout);*/
            for ( i= 0; i < n_list; ++i )
                remap[list[i]]= i;

            //tprintf("...\n");
            int common_set= j;

            int* needed_s= inalloc(n_occurance_table1);
            int n_needed_s= n_occurance_table1;
            int minns, maxns;

            /*tprintf("f: \n");
            for ( i= 0; i < n_f; ++i )
                printf("%d ", f[i]);
            printf("\n");

            tprintf("n_expand_map: %d\n", n_expand_map);

            for ( i= 0; i < n_occurance_table1; ++i )
                printf("%d ", common_list[i*n_common_list2 + j]);
            printf("\n");*/

            if ( common_set != 0 )
            {
                //dea();
                for ( i= 0; i < n_occurance_table1; ++i )
                {
                    needed_s[i]= 0;
                    for ( k= 0; k < n_f; ++k )
                        needed_s[i]|= occurance_table[i*n_occurance_table2 + f[k]];
                    //printf("%d ", needed_s[i]);
                    needed_s[i]&= common_list[i*n_common_list2 + j];
                }
                /*for ( i= 0; i < n_needed_s; ++i )
                    if ( needed_s[i] != 0 )
                        printf("%d ", needed_s[i]);
                printf("\n");*/
                /*minns= minI(needed_s, n_needed_s);
                maxns= maxI(needed_s, n_needed_s);*/
                //printf("%d %d\n", minns, maxns);
                //deb();
                n_needed_s= 0;
                for ( i= 0; i < n_occurance_table1; ++i )
                    if ( needed_s[i] )
                        needed_s[n_needed_s++]= expand_map[i];
                uniqueIA(&needed_s, &n_needed_s);
                //dec();
            }
            else
            {
                free(needed_s);
                needed_s= NULL;
                n_needed_s= 0;
            }

            printf("needed_s: \n");
            if ( needed_s )
            {
                for ( i= 0; i < n_needed_s; ++i )
                    printf("%d ", needed_s[i]);
                printf("\n");
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
            seti(remap_s, max_needed_s, 0);
            int n_remap_s= max_needed_s;

            for ( i= 0; i < n_list; ++i )
                remap_s[list[i]]= i;

            int n_weight21;
            int n_weight22;
            createSubArrayIndex2Float(weight, n_weight, n_weight, needed, n_needed, needed_s, n_needed_s, &weight2, &n_weight21, &n_weight22);

            int l= 0, m;

            target_map2= (float*)malloc(sizeof(float)*n_target_map1 * n_needed);
            int n_target_map21= n_target_map1;
            int n_target_map22= n_needed;
            l= 0;

            /*printf("%f\n", target_map[n_target_map1*n_target_map2 - 1]); fflush(stdout);
            printf("%d,%d\n", n_target_map1, n_target_map2);
            for ( i= 0; i < n_needed; ++i )
                printf("%d ", needed[i]);
            fflush(stdout);*/
            for ( i= 0; i < n_target_map1; ++i )
                for ( k= 0; k < n_needed; ++k )
                    target_map2[i * n_needed + k]= target_map[i*n_target_map2 + needed[k]];

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

            if ( j != 0 )
            {
                int* common= inalloc(n_common_list1);
                int n_common= n_common_list1;
                for ( i= 0; i < n_common_list1; ++i )
                    common[i]= common_list[i*n_common_list2 + j];

                float* weight3;
                int n_weight31;
                int n_weight32;

                createSubArrayIndex2Float(weight2, n_weight21, n_weight22, needed, n_needed, needed, n_needed, &weight3, &n_weight31, &n_weight32);

                int l= 0, m;

                int* cnts= inalloc(n_remap);
                int n_cnts= n_remap;
                seti(cnts, n_remap, 0);
                int* expanded= inalloc(n_common);
                int n_expanded= n_common;
                for ( i= 0; i < n_common; ++i )
                    expanded[i]= expand_map[common[i]];

                for ( m= 0; m < n_expanded; ++m )
                    cnts[expanded[m]]= cnts[expanded[m]] + 1;

                int* selection= inalloc(n_cnts);
                int n_selection= n_cnts;
                for ( i= 0; i < n_cnts; ++i )
                    selection[i]= cnts[i] > 0;

                int* remap_selection= inalloc(n_selection);
                int n_remap_selection= n_selection;
                for ( i= 0; i < n_selection; ++i )
                    remap_selection[i]= remap[selection[i]];

                createSubArrayIndex2Float(weight3, n_weight31, n_weight32, remap_selection, n_remap_selection, remap_selection, n_remap_selection, &B, &n_B1, &n_B2);

                for ( i= 0; i < n_weight31; ++i )
                    B[i*n_weight32 + i]= (1.0 + (cnts[selection[i]] - 1.0) * mix_term)/(double)(cnts[selection[i]]);

                if ( n_B1 * n_B2 * 8 > 100e6 && ! options->clusterMode )
                {
                    need_inv[j-n]= false;
                    //invertMatrix(B, n_B1);
                    float* B_inv;
                    invertMatrixFloatN(B, n_B1, n_B1, &B_inv);
                    free(B);
                    B= B_inv;
                }
                else
                {
                    need_inv[j-n]= true;
                }

                B_pass[j-n]= B;
                n_B_pass[j-n]= n_B1;
                common_cnts[j-n]= cnts;
                n_common_cnts[j-n]= n_cnts;
                free(weight3);
            }
            else
            {
                B_pass[j-n]= NULL;
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

        if ( weight2 )
            free(weight2);
        if ( target_map2 )
            free(target_map2);
        if ( B )
            free(B);

        if ( pool_size > 1 )
        {
            // TODO
        }
        else
        {
            B_copy= B_pass;
            n_B_copy= n_B_pass;
            target_map2_copy= target_map2_pass;
            occ_copy= occ_fragment;
            weight2_copy= weight2_pass;
        }

        /*free(B_pass);
        free(target_map2_pass);
        free(weight2_pass);
        free(occ_fragment);*/
        //B_pass= NULL;
        //target_map2_pass= NULL;
        //weight2_pass= NULL;
        //occ_fragment= NULL;

        /*int len_W= n_weight1;
        int n_items= (n + pool_size - 1) < max_common_index ? (n + pool_size - 1) : max_common_index;
        n_items= n - n_items + 1;
        int* items= inalloc(n_items);
        for ( i= n + pool_size - 1; i <= max_common_index; ++i )
            items[i]= i;

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

            int* common= inalloc(n_common_list1);
            int* common_cnts_;
            int n_common_cnts_;
            float* Bi;
            int n_Bi1, n_Bi2;
            if ( n > 0 )
            {
                for ( i= 0; i < n_common_list1; ++i )
                    common[i]= common_list[i*n_common_list2 + n];
                common_cnts_= common_cnts[0];
                n_common_cnts_= n_common_cnts[0];
                if ( need_inv[0] )
                {
                    Bi= invertMatrix(B_copy[0], n_B_copy[0], n_B_copy[0]);
                    n_Bi1= n_B_copy1[0];
                    n_Bi2= n_B_copy2[0];
                }
                else
                {
                    Bi= B_copy[0];
                    n_Bi1= n_B_copy1[0];
                    n_Bi2= n_B_copy2[0];
                }

                // computeSpatialMapBlock
            }
        }

        // Periodically consolidate and compress the returned values to save
        // memory.

        if ( ( entries > 100 || maxI(items, n_itmes) == maxI(common_index, n_common_index)) && !table_mode )
        {
            // TODO
        }*/
    }

    /*tprintf("End of Build Spatial Maps\n");

    if ( !table_mode )
    {
        // TODO
    }
    else
    {
        // Operations for values that are already globally integrated

        for ( k= 0; k < n_n_exports1; ++k )
        {
            if ( exports1[k] == 0 || n_exports1[k] == 2 )
                exports1[k]= sparse(len_S, 1);
            else
                esports1[k]= spares(exports1[k]);
        }

        /**spatial_tableIO= exports1;
        int n_spatial_table1= n_n_export*/

        //tprintf("End of Generate Spatial Table\n");
    //}*/
}



void computeSpatialMapBlock(int* common, int n_common,
                            int* common_cnts, int n_common_cnts,
                            float* Bi, int n_Bi1, int n_Bi2,
                            bool* occ_fragment, int n_occ_fragment1, int n_occ_fragment2,
                            int* expand_map, int n_expand_map,
                            int* remap, int n_remap,
                            int* remap_s, int n_remap_s,
                            float* weight, int n_weight1, int n_weight2,
                            float* target_map, int n_target_map,
                            int len_M, int len_W,
                            int* index, int n_index,
                            double mix_term,
                            berkeleyAverageOptions* options,

                            float** exports1IO, int* n1_exports1IO, int* n2_exports1IO,
                            float** exports2IO, int* n1_exports2IO, int* n2_exports2IO,
                            float** exports3IO, int* n1_exports3IO, int* n2_exports3IO,
                            int* f, int n_f)
{
    /*bool table_mode;
    if ( len_M == 1 )
        table_mode= true;
    else
        table_mode= false;

    int loops= n_occ_fragment2;

    float** exports1= (int**)malloc(sizeof(int*)*loops);
    int* n1_exports1= inalloc(loops);
    int* n2_exports1= inalloc(loops);
    float** exports2= (int**)malloc(sizeof(int*)*loops);
    int* n1_exports2= inalloc(loops);
    int* n2_exports2= inalloc(loops);
    float** exports3= (int**)malloc(sizeof(int*)*loops);
    int* n1_exports3= inalloc(loops);
    int* n2_exports3= inalloc(loops);

    int i, j, k;

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
            exports1[j]= inalloc(2);
            exports1[j][0]= 0;
            exports1[j][1]= 0;
            n_exports1[j]= 2;

            exports2[j]= inalloc(1);
            exports2[j][0]= 0;
            n_exports2[j]= 1;

            exports3[j]= inalloc(1);
            exports3[j][0]= 0;
            n_exports3[j]= 1;

            continue;
        }

        if ( common != NULL && n_common != 0 )
        {
            for ( i= 0; i < n_f; ++i )
                f[i]= f[i] & !common[i];
        }

        int* cnts= inalloc(n_remap);
        int n_cnts= n_remap;
        int* expanded= inalloc(n_f);
        int n_expanded= n_f;
        for ( i= 0; i < n_f; ++i )
            expanded[i]= expanded_map[f[i]];

        int m;
        for ( m= 0; m < n_expanded; ++m )
            cnts[expanded[m]]= cnts[expanded[m]] + 1;

        bool* selection= bnalloc(n_cnts);
        int n_selection= n_cnts;
        int sum_selection= 0;
        for ( i= 0; i < n_selection; ++i )
        {
            selection[i]= cnts[i] > 0;
            sum_select+= selection[i];
        }

        // Select rows of weight function corresponding to active stations and
        // add in the nugget.

        int* re_select= inalloc(sum_selection);
        int n_re_select= sum_selection;
        k= 0;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                re_select[k++]= remap[i];

        int* re_select_s= inalloc(sum_selection);
        int n_re_select_s= sum_selection;
        k= 0;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                re_select_s[k++]= remap[i];

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

        for ( i= 0; i < n_B_d1; ++i )
            B_d[i*n_B_d2 + i]= (1.0 + (cnts[selection[i]] - 1.0) * mix_term)/cnts[selection[i]];

        // Target weights for active stations
        int* fx= inalloc(n_common_cnts + n_selection);
        int n_fx= 0;
        for ( i= 0; i < n_common_cnts; ++i )
            if ( common_cnts[i] > 0 )
                fx[n_fx++]= i;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] > 0 )
                fx[n_fx++]= i;

        float* A1= (float*)malloc(sizeof(float)*n_fx);
        int n_A1= n_fx;
        for ( i= 0; i < n_fx; ++i )
            A1[i]= target_map[remap[fx[i]]];

        float* f_weights_map;
        int n_f_weights_map1;
        int n_f_weights_map2;
        if ( !table_mode )
        {

        }
        else
        {
            // Compute A1/B
            if ( !(n_Bi1 == 0 || n_Bi2 == 0 || Bi == 0) )
            {
                // Quick division with known partial inverse
                float* BiT= (float*)malloc(sizeof(float)*(n_Bi1*n_Bi2));
                for ( i= 0; i < n_Bi1; ++i )
                    for ( k= 0; k < n_Bi2; ++k )
                    {
                        BiT[k*n_Bi1 + i]= Bi[i*n_Bi2 + k];
                    }
                mldivideWithPartialInverse2(B_b, n_B_b1, n_B_b2, B_d, n_B_d1, n_B_d2, A1, n_A1, BiT, n_Bi2, n_Bi1, &f_weights_map, &n_f_weights_map1, &n_f_weights_map2);
            }
            else
            {
                // We do direct division when the common portion is too small to
                // be worth worrying about
                f_weights_map= (float*)malloc(sizeof(float)*n_B_b2);
                float* B_dT= (float*)(sizeof(float)*(n_B_d1 * n_B_d2));
                for ( i= 0; i < n_B_d1; ++i )
                    for ( k= 0; k < n_B_d2; ++k )
                    {
                        B_dT[k*n_B_d1 + i]= B_d[i*n_B_d2 + k];
                    }
                solveLinEqHD(B_dT, n_B_d2, n_B_d1, A1, n_A1, f_weights_map);
            }
        }
        free(A1);
        free(B_b);
        free(B_d);

        if ( !table_mode )
        {
            // TODO
        }

        int tmp_cnts= 0;
        for ( i= 0; i < n_common_cnts; ++i )
            if ( n_common_cnts[i] >= 1 )
                ++tmp_cnts;
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                ++tmp_cnts;

        cnts= (int*)realloc(cnts, sizeof(int)*tmp_cnts);
        n_cnts= 0;
        for ( i= 0; i < n_common_cnts; ++i )
            if ( n_common_cnts[i] >= 1 )
                cnts[n_cnts++]= common_cnts[i];
        for ( i= 0; i < n_selection; ++i )
            if ( selection[i] )
                cnts[n_cnts++]= cnts[i];

        // TODO bsxfun
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
            int* indices= (int*)malloc(sizeof(int)*(n_common_cnts + n_selection));
            int n_indices= 0;
            for ( i= 0; i < n_common_cnts; ++i )
                if ( common_cnts[i] >= 1 )
                    indices[n_indices++]= i;
            for ( i= 0; i < selection; ++i )
                if ( selection[i] )
                    indices[n_indices++]= i;

            for ( i= 0; i < len_M; ++i )
                for ( k= 0; k < n_indeces; ++k )
                {
                    temp_reduced[i*len_W + indeces[k]]= f_weights_map[k];
                }

            float* temp_full= (float*)malloc(sizeof(float)*len_M*n_expand_map);
            for ( i= 0; i < len_M*n_expand_map; ++i )
                temp_full[i]= 0;

            for ( i= 0; i < len_M; ++i )
                for ( k= 0; k < n_f; ++k )
                    temp_full[i*n_expand_map + f[k]]= temp_reduced[i*len_W + expanded[k]];
            for ( i= 0; i < len_M; +3i )
                for ( k= 0; k < n_common; ++k )
                    temp_full[i*n_expand_map + common[k]]= temp_reduced[i*len_W + expand_map[common[k]]];

            float* temp_fullT= (float*)malloc(sizeof(float)*len_M*n_expand_map);
            for ( i= 0; i < len_M; ++i )
                for ( k= 0; k < n_expand_map; ++k )
                    temp_fullT[k*len_M + i]= temp_full[i*n_expand_map + k];

            exports1[j]= temp_fullT;
            n1_exports1[j]= n_expand_map;
            n2_exports1[j]= len_M;
        }

    }
    for ( i= 0; i < n_f; ++i )
    {
        exports1IO[f[i]]= exports1[i];
        exports2IO[f[i]]= exports2[i];
        exports3IO[f[i]]= exports3[i];
        n1_exports1IO[f[i]]= n1_exports1[i];
        n2_exports1IO[f[i]]= n2_exports1[i];
        n1_exports2IO[f[i]]= n1_exports2[i];
        n2_exports2IO[f[i]]= n2_exports2[i];
        n1_exports3IO[f[i]]= n1_exports3[i];
        n2_exports3IO[f[i]]= n2_exports3[i];
    }*/
}

void mldivideWithParitalInverse2(float* M_C, int n_M_C1, int n_M_C2,
                                 float* M_D, int n_M_D1, int n_M_D2,
                                 float* T, int n_T,
                                 float* Ai, int n_Ai1, int n_Ai2,

                                 float** XIO, int* n_X1IO, int* n_X2IO)
{
    // Helper function for determining a matrix division solution given a known
    // partial inverse.

    /*float* M_B= (float*)malloc(sizeof(float)*n_M_C1*n_M_C2);
    int n_M_B1= n_M_C2;
    int n_M_B2= n_M_C1;

    int i, j, k, l;
    for ( i= 0; i < n_M_C1; ++i )
        for ( j= 0; j < n_M_C2; ++j )
            n_B[j*n_M_C1 + i]= M_C[i*n_M_C2 + j];

    float* T1= (float*)malloc(sizeof(float)*n_M_B1);
    int n_T1= n_M_B1;
    for ( i= 0; i < n_M_B1; ++i )
        T1[i]= T[i];
    float* T2= (float*)malloc(sizeof(float)*(n_T - n_M_B1));
    int n_T2= n_T - n_M_B1;
    for ( i= n_M_B1; i < n_T; ++i )
        T2[i]= T[i];

    float* X;
    if ( M_D == NULL || n_M_D1 == 0 || n_M_D2 == 0 )
    {
        // MATRIX AND VECTOR DIMENSIONS DO NOT AGREE!!!
        tprintf("T: %d, Ai: %d x %d\n", n_T, n_Ai1, n_Ai2);
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

    int n_MBY= n_MBY1 > n_MBY2 ? n_MBY1 : n_MBY2;

    float* A= (float*)malloc(sizeof(float)*n_M_D1*n_M_D2);
    int n_A1= n_M_D1;
    int n_A2= n_M_D2;
    for ( i= 0; i < n_M_D1 * n_M_D2; ++i )
        A[i]= M_D[i] - MBAiMC[i];

    float* b= (float*)malloc(sizeof(float)*n_MBY);
    for ( i= 0; i < n_MBY; ++i )
        b[i]= T2[i] - MBY[i];
    int n_b= n_MBY;

    float* X2= (float*)malloc(sizeof(float)*n_A2);
    int n_X2= n_A2;

    solveLinEqHD(A, n_A1, n_A2, b, 1, X2);

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

    float* X= (float*)malloc(sizeof(float)*(n_X1 + n_X2));
    for ( i= 0; i < n_X1; ++i )
        X[i]= X1[i];
    for ( ; i < n_X2; ++i )
        X[i]= X2[i-n_X1];

    *XIO= X;
    *n_X1IO= n_X1 + n_X2;
    *n_X2IO= 1;*/
}

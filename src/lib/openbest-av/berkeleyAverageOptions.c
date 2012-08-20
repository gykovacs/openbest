#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-io/loadOptions.h"

berkeleyAverageOptions* createBAON()
{
    berkeleyAverageOptions* p= (berkeleyAverageOptions*)malloc(sizeof(berkeleyAverageOptions));
    p->localMode= true;
    //p->gridSize= 16000;
    p->gridSize= 2000;
    p->gridApproximationDistance= 4000;
    //p->useLandMask= true;
    p->useLandMask= false;
    p->minMonths= 2;
    p->minStations= 5;

    load_r_p(&(p->correlationParameters), &(p->n_correlationParameters));
    p->correlationLimitDistance= load_max_d();

    p->badFlags= iBadFlags;
    p->n_badFlags= n_badFlags;
    p->useSiteWeighting= true;
    p->siteWeightingGlobalCutoffMultiplier= FLT_MAX;
    p->siteWeightingCutoffMultiplier= 5;
    p->siteWeightingLocalized= true;
    p->siteWeightingRemoveSelf= true;

    p->useOutlierWeighting= true;
    p->outlierWeightingGlobalCutoffMultiplier= 4;
    p->outlierWeightingCutoffMultiplier= 2.5;
    p->outlierWeightingLocalized= true;
    p->outlierWeightingRemoveSelf= false;

    p->useScalpel= true;
    p->scalpelMetadata= true;
    p->scalpelGaps= true;
    p->scalpelGapLength= 1;
    p->scalpelDeclaredMoves= true;
    p->scalpelSuspectedMoves= true;
    p->scalpelTOBChanges= true;
    p->scalpelTOBPersistence= 0.5;
    p->scalpelTOBDifference= 4;
    p->scalpelEmpirical= true;
    p->scalpelEmpiricalMaxDistance= 2500;
    p->scalpelEmpiricalMaxPairs= 300;
    p->scalpelEmpiricalBestPairs= 25;
    p->scalpelEmpiricalCut= 0.9999;
    p->scalpelEmpiricalMaxSegment= 10*12;
    p->removeInsignificantBreaks= false;

    p->useIterativeReweighting= true;
    p->precisionTarget= 0.001;
    p->precisionTargetMinStations= 50;
    p->maxIterations= 30;
    p->useBroydenMethod= true;
    p->useSeed= false;
    p->n_seedMonthlyTimes= 0;
    p->seedMonthlyTimes= NULL;
    p->n_seedMonthlyValues= 0;
    p->seedMonthlyValues= NULL;

    p->spatialMapsEmptyCellCut= 0.02;
    p->spatialMapsTrivialMaxCut= 0.025;
    p->spatialMapsTrivialSumCut= 0.005;

    p->limitEmpiricalFitRegion= true;
    p->empiricalFitRegionCutoff= 0.8;
    p->limitEmpiricalBaselineRegion= true;
    p->empiricalBaselineRegionCutoff= 0.8;

    p->fullBaselineMapping= true;
    p->fullBaselineTargetLats= NULL;
    p->n_fullBaselineTargetLats= 0;
    p->fullBaselineAltitudeDegree= 2;

    p->computeUncertainty= true;
    p->computeStatisticalUncertainty= true;
    p->statisticalUncertaintyLocal= false;
    p->useJackKnife= true;
    p->statisticalUncertaintyInnerLoops= 8;
    p->statisticalUncertaintyOuterLoops= 1;
    p->statisticalUncertaintyMinRepeats= 3;
    p->statisticalUncertaintyBenchmarkMinDate= 1960;
    p->statisticalUncertaintyBenchmarkMaxDate= 2000;
    p->computeEmpiricalSpatialUncertainty= true;
    p->spatialUncertaintyBenchmarkMinDate= 1960;
    p->spatialUncertaintyBenchmarkMaxDate= 2000;
    p->computeAnalyticSpatialUncertainty= true;

    p->clusterMode= false;
}

berkeleyAverageOptions* createBAON1(char* mode)
{
    berkeleyAverageOptions* p= createBAON();

    if ( strcmp(mode, "quick") == 0 )
    {
        p->localMode= false;
        p->useSiteWeighting= false;
        p->useOutlierWeighting= false;
        p->useIterativeReweighting= false;
        p->computeUncertainty= false;
        p->scalpelEmpirical= false;

        p->computeEmpiricalSpatialUncertainty= false;
        p->supplementEmpiricalSpatialWithAnalytic= false;
        p->computeAnalyticSpatialUncertainty= false;

        p->clusterMode= false;

        p->fullBaselineMapping= false;
    }
    else if ( strcmp(mode, "weighted") == 0 )
    {
        ///An average with outlier and site weighting, handled iteratively, but does not perform the localization and mapping functions.
        p->localMode= false;
        p->computeUncertainty= false;
        p->outlierWeightingLocalized= false;
        p->siteWeightingLocalized= false;
        p->siteWeightingGlobalCutoffMultiplier= 5;
        p->outlierWeightingGlobalCutoffMultiplier= 2.5;

        p->computeEmpiricalSpatialUncertainty= false;
        p->supplementEmpiricalSpatialWithAnalytic= false;
        p->computeAnalyticSpatialUncertainty= false;

        p->clusterMode= false;
    }
    else if ( strcmp(mode, "local") == 0 )
    {
        ///Localized mapping with uncertainty, this is the default set.
    }
    else if ( strcmp(mode, "complete") == 0 )
    {
        ///Same as local but with localized statistical uncertainty.
        p->statisticalUncertaintyLocal= true;
    }
    return p;
}

berkeleyAverageOptions* createBerkeleyAverageOptionsQuick()
{
    return createBAON1("quick");
}

void berkeleyAverageOptionsConsistency(berkeleyAverageOptions* p)
{
    if ( p->localMode && p->gridSize < 100 )
        eprintf("gridSize is too small");
    if ( p->gridSize > 50000 && p->localMode )
        wprintf("very large gridSize may provide poor performance");

    if ( p->gridApproximationDistance < 0 )
        eprintf("gridApproximationDistance < 0");
    if ( p->gridApproximationDistance > 100 )
        eprintf("gridApproximationDistance > 100 km is not recommended");
    if ( p->gridApproximationDistance > 0.5*22500/sqrt(p->gridSize))
        wprintf("gridApproximationDistance should not exceed half the mean distance between grid cells");

    if ( p->minMonths < 3 )
        eprintf("minMonths < 3");
    if ( p->minStations < 1 )
        eprintf("minStations < 1 ");

    if ( p->useSiteWeighting )
    {
        if ( p->siteWeightingCutoffMultiplier < 0 )
            eprintf("siteWeightingCutoffMultiplier < 0 ");
        if ( p->siteWeightingGlobalCutoffMultiplier < 0 )
            eprintf("siteWeightingGlobalCutoffMultiplier < 0 ");
        if ( p->siteWeightingLocalized && !p->localMode )
            eprintf("siteWeightingLocalized in non-local mode");
    }

    if ( p->useOutlierWeighting )
    {
        if ( p->outlierWeightingCutoffMultiplier < 0 )
            eprintf("outlierWeightingCutoffMultiplier < 0");
        if ( p->outlierWeightingGlobalCutoffMultiplier < 0 )
            eprintf("outlierWeightingGlobalCutoffMultiplier < 0");
        if ( p->outlierWeightingLocalized && !p->localMode )
            eprintf("outlierWeightingLocalized in non-local mode");
    }

    if ( p->useScalpel )
    {
        if ( p->scalpelMetadata )
        {
            if ( p->scalpelGaps && p->scalpelGapLength < 1.0/6.0 )
                eprintf("scalpelGapLength < 1/6");
            if ( p->scalpelTOBChanges && p->scalpelTOBPersistence < 1.0/6.0 )
                eprintf("scalpelTOBPersistence < 1/6");
            if ( p->scalpelTOBChanges && (p->scalpelTOBDifference < 1 || p->scalpelTOBDifference > 12 ))
                eprintf("scalpelTOBDifference not between 1 and 12");
            if ( !p->scalpelGaps && !p->scalpelTOBChanges && !p->scalpelDeclaredMoves && !p->scalpelSuspectedMoves )
                eprintf("scalpelMetaData is true, but all metadata segments are false");
        }
        if ( p->scalpelEmpirical )
        {
            if ( p->scalpelEmpiricalMaxDistance < 0 )
                eprintf("scalpelEmpiricalMaxDistance < 0");
            if ( p->scalpelEmpiricalMaxDistance < 200 )
                wprintf("scalpelEmpiricalMaxDistance < 200 km is not recommended");
            if ( p->scalpelEmpiricalMaxPairs < 1 )
                eprintf("scalpelEmpiricalMaxPaires < 1");
            if ( p->scalpelEmpiricalMaxPairs < p->scalpelEmpiricalBestPairs )
                eprintf("scalpelEmpiricalMaxPairs < scalpelEmpiricalBestPairs");
            if ( p->scalpelEmpiricalBestPairs < 1 )
                eprintf("scalpelEmpiricalBestPairs < 1");
            if ( p->scalpelEmpiricalBestPairs > 100 )
                wprintf("scalpelEmpiricalBestPairs > 100 is not recommended");
            if ( p->scalpelEmpiricalCut >= 1 || p->scalpelEmpiricalCut <= 0 )
                eprintf("scalpelEmpiricalPrimaryCut not in range 0-1");
            if ( p->scalpelEmpiricalMaxSegment < 6 )
                eprintf("scalpelEmpiricalMaxSegment is less than 6");
            if ( p->scalpelEmpiricalCut < 0.99 )
                eprintf("scalpelEmpiricalCut < 0.99 is not recommended");
            if ( p->scalpelEmpiricalMaxSegment < 5*12 )
                wprintf("scalpelEmpiricalMaxSegment < 60 is not recommended");

        }
    }

    if ( p->useIterativeReweighting )
    {
        if ( p->precisionTarget < 0 )
            eprintf("precisionTarget < 0");
        if ( p->precisionTarget < 0.0001 )
            wprintf("precisionTarget < 0.0001 is not recommended");
        if ( p->precisionTarget > 0.01 )
            wprintf("precisionTarget > 0.01 is not recommended");
        if ( p->maxIterations < 3 )
            eprintf("maxIterations < 3");
        if ( p->maxIterations > 100 )
            wprintf("maxIterations > 100 is not recommended");
        if ( p->precisionTargetMinStations < 1 )
            eprintf("precisionTargetMinStations < 1");
        if ( p->precisionTargetMinStations > 20 )
            wprintf("precisionTargetMinStations < 20 is not recommended");
    }

    if ( p->localMode )
    {
        if ( p->spatialMapsEmptyCellCut < 0 || p->spatialMapsEmptyCellCut > 1 )
            eprintf("spatialMapsEmptyCellCut not in range 0-1");
        if ( p->spatialMapsEmptyCellCut > 0.05 )
            wprintf("spatialMapsEmptyCellCut > 0.05 is not recommended");
        if ( p->spatialMapsTrivialMaxCut < 0 || p->spatialMapsTrivialMaxCut > 1 )
            eprintf("spatialMapsTrivialMaxCut not in range 0-1");
        if ( p->spatialMapsTrivialMaxCut > 0.1 )
            wprintf("spatialMapsTrivialMaxCut > 0.1 is not recommended");
        if ( p->spatialMapsTrivialSumCut < 0 || p->spatialMapsTrivialSumCut > 1 )
            eprintf("spatialMapsTrivialSumCut not in range 0-1");
        if ( p->spatialMapsTrivialSumCut > 0.05 )
            wprintf("spatialMapsTrivialSumCut > 0.05 is not recommended");
    }

    if ( p->limitEmpiricalFitRegion )
        if ( p->empiricalFitRegionCutoff < 0 || p->empiricalFitRegionCutoff > 1 )
            eprintf("empiricalFitRegionCutoff not in range 0-1");
    if ( p->limitEmpiricalBaselineRegion )
        if ( p->empiricalBaselineRegionCutoff < 0 || p->empiricalBaselineRegionCutoff > 1 )
            eprintf("empiricalBaselineRegionCutoff not in range 0-1");
    if ( p->limitEmpiricalBaselineRegion && p->limitEmpiricalFitRegion )
        if ( p->empiricalBaselineRegionCutoff < p->empiricalFitRegionCutoff )
            wprintf("empiricalBaselineRegionCutoff < empiricalFitRegionCutoff is not recommended");

    if ( p->fullBaselineMapping )
    {
        if ( minR(p->fullBaselineTargetLats, p->n_fullBaselineTargetLats) < 0 )
            eprintf("min(fullBaselineTargetLats) < 0 ");
        if ( maxR(p->fullBaselineTargetLats, p->n_fullBaselineTargetLats) > 1 )
            eprintf("max(fullBaselineTargetLats) > 1 ");
        if ( p->n_fullBaselineTargetLats == 0 )
            eprintf("fullBaselineTargetLats is empty");
        if ( p->n_fullBaselineTargetLats < 10 )
            wprintf("length(fullBaselineTargetLats) < 10 not recommended");
        if ( p->fullBaselineAltitudeDegree < 1 )
            eprintf("fullBaselineAltitudeDegree < 1");
        if ( p->fullBaselineAltitudeDegree != floor(p->fullBaselineAltitudeDegree) )
            eprintf("fullBaselineAltitudeDegree is not an integer");
        if ( p->fullBaselineAltitudeDegree > 4 )
            wprintf("fullBaselineAltitudeDegree > 4 is not recommended");
    }

    if ( p->computeUncertainty )
    {
        if ( p->computeStatisticalUncertainty )
        {
            if ( p->statisticalUncertaintyLocal && !p->localMode )
                eprintf("statisticalUncertaintyLocal true but localMode is false");
            if ( p->statisticalUncertaintyInnerLoops < 2 )
                eprintf("statisticalUncertaintyInnerLoops < 2");
            if ( p->statisticalUncertaintyInnerLoops > 10 )
                wprintf("statisticalUncertaintyInnerLoops > 10 is not recommended");
            if ( p->statisticalUncertaintyOuterLoops < 1 )
                eprintf("statisticalUncertaintyOuterLoops < 1");
            if ( p->statisticalUncertaintyOuterLoops > 5 )
                wprintf("statisticalUncertaintyOuterLoops > 5 is not recommended");
            if ( p->statisticalUncertaintyMinRepeats > p->statisticalUncertaintyInnerLoops )
                eprintf("statisticalUncertaintyMinRepeats > statisticalUnceratintyInnerLoops ");
            if ( p->statisticalUncertaintyMinRepeats < 2 )
                eprintf("statisticalUncertaintyMinRepeats < 2");
            if ( p->statisticalUncertaintyMinRepeats < 3 )
                wprintf("statisticalUncertaintyMinRepeats < 3 is not recommended");
            if ( p->statisticalUncertaintyBenchmarkMinDate >= p->statisticalUncertaintyBenchmarkMaxDate )
                eprintf("statisticalUncertaintyBenchmarkMinDate >= statisticalUncertaintyBenchmarkMaxDate");
            if ( p->statisticalUncertaintyBenchmarkMaxDate - p->statisticalUncertaintyBenchmarkMinDate < 20 )
                wprintf("statisticalUncertaintyBenchmarkMaxDate - StatisticalUncertaintyBenchmarkMinDate is less than 20 years");
        }
        if ( (p->computeEmpiricalSpatialUncertainty || p->computeAnalyticSpatialUncertainty) && !p->localMode )
            eprintf("can't generate spatial uncertainties without localization");
        if ( p->computeEmpiricalSpatialUncertainty )
        {
            if ( p->spatialUncertaintyBenchmarkMinDate >= p->spatialUncertaintyBenchmarkMaxDate )
                eprintf("spatialUncertaintyBenchmarkMinDate >= spatialUncertaintyBenchmarkMaxDate");
            if ( p->spatialUncertaintyBenchmarkMaxDate - p->spatialUncertaintyBenchmarkMinDate < 20 )
                wprintf("spatialUncertaintyBenchmarkMaxDate - spatialUncertaintyBenchmarkMInDate is less than 20 years");
            if ( !p->supplementEmpiricalSpatialWithAnalytic )
                wprintf("use of supplementEmpiricalSpatialWithAnalytic is highly recommended");
            if ( p->supplementEmpiricalSpatialWithAnalytic && !p->computeAnalyticSpatialUncertainty )
                eprintf("supplementEmpiricalSpatialWithAnalytic is true but computeAnalyticSpatialUncertainty is false");
        }
    }
}


void displayBAO(berkeleyAverageOptions* b)
{
    tprintf("BASIC MAPPING OPTIONS\n");
    tprintf("localMode: %d\n", b->localMode);
    tprintf("gridSize: %d\n", b->gridSize);
    tprintf("girdApproximationDistance: %f\n", b->gridApproximationDistance);
    tprintf("useLandMask: %d\n", b->useLandMask);
    tprintf("MINIMUM DATA SIZE REQUIREMENTS\n");
    tprintf("minMonths: %d\n", b->minMonths);
    tprintf("minStations: %d\n", b->minStations);
    tprintf("CORRELATION FUNCTION PARAMTERIZATION\n");
    tprintf("correlationParameters: %d - \n", b->n_correlationParameters);
    int i;
    for ( i= 0; i < b->n_correlationParameters; ++i )
        printf("%g ", b->correlationParameters[i]);
    printf("\n");
    tprintf("correlationLimitDistance: %f\n", b->correlationLimitDistance);
    tprintf("BAD FLAGS:\n");
    tprintf("badFlags: %d - ", b->n_badFlags);
    for ( i= 0; i < b->n_badFlags; ++i )
        printf("%d ", b->badFlags[i]);
    printf("\n");
    tprintf("SITE WEIGHTING PARAMETERS\n");
    tprintf("useSiteWeighting: %d\n", b->useSiteWeighting);
    tprintf("siteWeightingGlobalCutoffMultiplier: %f\n", b->siteWeightingGlobalCutoffMultiplier);
    tprintf("siteWeightingCutoffMultiplier: %f\n", b->siteWeightingCutoffMultiplier);
    tprintf("siteWeightingLocalized: %d\n", b->siteWeightingLocalized);
    tprintf("siteWeightingRemoveSelf: %d\n", b->siteWeightingRemoveSelf);
    tprintf("OUTLIER WEIGHTING PARAMETERS\n");
    tprintf("useOutlierWeighting: %d\n", b->useOutlierWeighting);
    tprintf("outlierWeightingGlobalCutoffMultiplier: %f\n", b->outlierWeightingGlobalCutoffMultiplier);
    tprintf("outlierWeightingCutoffMultiplier: %f\n", b->outlierWeightingCutoffMultiplier);
    tprintf("outlierWeightingLocalized: %d\n", b->outlierWeightingLocalized);
    tprintf("outlierWeightingRemoveSelf: %d\n", b->outlierWeightingRemoveSelf);
    tprintf("SCALPEL RELATED OPTIONS\n");
    tprintf("useScalpel: %d\n", b->useScalpel);
    tprintf("scalpelMetadata: %d\n", b->scalpelMetadata);
    tprintf("scalpelGaps: %d\n", b->scalpelGaps);
    tprintf("scalpelGapLength: %d\n", b->scalpelGapLength);
    tprintf("scalpelDeclaredMoves: %d\n", b->scalpelDeclaredMoves);
    tprintf("scalpelSuspectedMoves: %d\n", b->scalpelSuspectedMoves);
    tprintf("scalpelTOBChanges: %d\n", b->scalpelTOBChanges);
    tprintf("scalpelTOBPersistence: %f\n", b->scalpelTOBPersistence);
    tprintf("scalpelTOBDifference: %f\n", b->scalpelTOBDifference);
    tprintf("scalpelEmpirical: %d\n", b->scalpelEmpirical);
    tprintf("scalpelEmpiricalMaxDistance: %f\n", b->scalpelEmpiricalMaxDistance);
    tprintf("scalpelEmpiricalMaxPairs: %d\n", b->scalpelEmpiricalMaxPairs);
    tprintf("scalpelEmpiricalBestPairs: %d\n", b->scalpelEmpiricalBestPairs);
    tprintf("scalpelEmpiricalCut: %f\n", b->scalpelEmpiricalCut);
    tprintf("scalpelEmpiricalMaxSegment: %d\n", b->scalpelEmpiricalMaxSegment);
    tprintf("removeInsignificantBreaks: %d\n", b->removeInsignificantBreaks);
    tprintf("ITERATIVE REWEIGHTING OPTIONS\n");
    tprintf("useIterativeReweighting: %d\n", b->useIterativeReweighting);
    tprintf("precisionTarget: %f\n", b->precisionTarget);
    tprintf("precisionTargetMinStations: %f\n", b->precisionTargetMinStations);
    tprintf("maxIterations: %d\n", b->maxIterations);
    tprintf("useBroydenMethod: %d\n", b->useBroydenMethod);
    tprintf("useSeed: %d\n", b->useSeed);
    tprintf("seedMonthlyTimes: %d - ", b->seedMonthlyTimes);
    for ( i= 0; i < b->n_seedMonthlyTimes; ++i )
        printf("%f ", b->seedMonthlyTimes[i]);
    printf("\n");
    tprintf("seedMonthlyValues: %d - ", b->seedMonthlyValues);
    for ( i= 0; i < b->n_seedMonthlyValues; ++i )
        printf("%f ", b->seedMonthlyValues[i]);
    printf("\n");
    tprintf("SPATIAL MAP APPROXIMATION PARAMETERS\n");
    tprintf("spatialMapsEmptyCellCut: %f\n", b->spatialMapsEmptyCellCut);
    tprintf("spatialMapsTrivialMaxCut: %f\n", b->spatialMapsTrivialMaxCut);
    tprintf("spatialMapsTrivialSumCut: %f\n", b->spatialMapsTrivialSumCut);
    tprintf("EMPIRICAL REGION DETERMINATION PARAMETERS\n");
    tprintf("limitEmpiricalFitRegion: %d\n", b->limitEmpiricalFitRegion);
    tprintf("empiricalFitRegionCutoff: %f\n", b->empiricalFitRegionCutoff);
    tprintf("limitEmpiricalBaselineRegion: %d\n", b->limitEmpiricalBaselineRegion);
    tprintf("empiricalBaselineRegionCutoff: %f\n", b->empiricalBaselineRegionCutoff);
    tprintf("BASELINE MAPPING OPTIONS\n");
    tprintf("fullBaselineMapping: %d\n", b->fullBaselineMapping);
    tprintf("fullBaselineTargetLats: %d - ", b->n_fullBaselineTargetLats);
    for ( i= 0; i < b->n_fullBaselineTargetLats; ++i )
        printf("%f ", b->fullBaselineTargetLats[i]);
    printf("\n");
    tprintf("fullBaselineAltitudeDegree: %d\n", b->fullBaselineAltitudeDegree);
    tprintf("UNCERTAINTY COMPUTATION OPTIONS\n");
    tprintf("computeUncertainty: %d\n", b->computeUncertainty);
    tprintf("computeStatisticalUncertainty: %d\n", b->computeStatisticalUncertainty);
    tprintf("statisticalUnceratintyLocal: %d\n", b->statisticalUncertaintyLocal);
    tprintf("useJackKnife: %d\n", b->useJackKnife);
    tprintf("statisticalUncertaintyInnerLoops: %d\n", b->statisticalUncertaintyInnerLoops);
    tprintf("statisticalUncertaintyOuterLoops: %d\n", b->statisticalUncertaintyOuterLoops);
    tprintf("statisticalUncertaintyMinRepeats: %d\n", b->statisticalUncertaintyMinRepeats);
    tprintf("statisticalUncertaintyBenchmarkMinDate: %d\n", b->statisticalUncertaintyBenchmarkMinDate);
    tprintf("statisticalUncertaintyBenchmarkMaxDate: %d\n", b->statisticalUncertaintyBenchmarkMaxDate);
    tprintf("computeEmpiricalSpatialUncertainty: %d\n", b->computeEmpiricalSpatialUncertainty);
    tprintf("supplementEmpiricalSpatialWithAnalytic: %d\n", b->supplementEmpiricalSpatialWithAnalytic);
    tprintf("spatialUncertaintyBenchmarkMinDate: %d\n", b->spatialUncertaintyBenchmarkMinDate);
    tprintf("spatialUncertaintyBenchmarkMaxDate: %d\n", b->spatialUncertaintyBenchmarkMaxDate);
    tprintf("computeAnalyticSpatialUncertainty: %d\n", b->computeAnalyticSpatialUncertainty);
    tprintf("OTHER OPTIONS\n");
    tprintf("clutserMode: %d\n", b->clusterMode);
}

void destroyBAO(berkeleyAverageOptions* bao)
{
    free(bao->correlationParameters);
    free(bao);
}

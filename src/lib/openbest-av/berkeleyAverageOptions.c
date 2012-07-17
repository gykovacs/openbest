#include "openbest-av/berkeleyAverageOptions.h"


berkeleyAverageOptions* createBAON()
{
    berkeleyAverageOptions* p= (berkeleyAverageOptions*)malloc(sizeof(berkeleyAverageOptions));
    p->localMode= true;
    p->gridSize= 16000;
    p->gridApproximationDistance= 60;
    p->useLandMask= true;
    p->minMonths= 6;
    p->minStations= 5;
    //TODO correlation parameters
    //TODO correlation limit distance
    //TODO badFlags
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
    //TODO fullBaselineTargetLats
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
    berkeleyAverageOptionsConsistency(p);
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
            if ( !p->scalpelGaps && !p->scalpelTOBChanges && !p->scalpelDeclaredMoves && !p->scalpelSuspectedMosve )
                eprintf("scalpelMetaData is true, but all metadata segments are false");
        }
        if ( p->scalpelEMpirical )
        {
            if ( p->scalpelEmpiricalMaxDistance < 0 )
                eprintf("scalpelEmpiricalMaxDistance < 0");

        }
    }
}

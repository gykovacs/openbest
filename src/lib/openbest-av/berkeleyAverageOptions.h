#ifndef _BERKELEY_AVERAGE_OPTIONS_H_
#define _BERKELEY_AVERAGE_OPTIONS_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/primaryKeyTables.h"

/**
  * struct to represent berkeleyAverageOptions struct
  */
typedef struct
{
    /// Basic Mapping Options
    bool localMode;
    int gridSize;
    real gridApproximationDistance;
    bool useLandMask;

    /// Minimum Data Size Requirements
    int minMonths;
    int minStations;

    /// Correlation Function Parametrization
    real correlationParameters;
    real correlationLimitDistance;

    /// Bad Flags
    int* badFlags;
    int n_badFlags;

    /// Site Weighting Parameters
    bool useSiteWeighting;
    real siteWeightingGlobalCutoffMultiplier;
    real siteWeightingCutoffMultiplier;
    bool siteWeightingLocalized;
    bool siteWeightingRemoveSelf;

    /// Outlier Weighting Parameters
    bool useOutlierWeighting;
    real outlierWeightingGlobalCutoffMultiplier;
    real outlierWeightingCutoffMultiplier;
    bool outlierWeightingLocalized;
    bool outlierWeightingRemoveSelf;

    /// Scalpel Related Options
    bool useScalpel;
    bool scalpelMetadata;
    bool scalpelGaps;
    int scalpelGapLength;
    bool scalpelDeclaredMoves;
    bool scalpelSuspectedMoves;
    bool scalpelTOBChanges;
    real scalpelTOBPersistence;
    real scalpelTOBDifference;
    bool scalpelEmpirical;
    real scalpelEmpiricalMaxDistance;
    int scalpelEmpiricalMaxPairs;
    int scalpelEmpiricalBestPairs;
    real scalpelEmpiricalCut;
    int scalpelEmpiricalMaxSegment;
    bool removeInsignificantBreaks;

    /// Iterative Reweighting Options
    bool useIterativeReweighting;
    real precisionTarget;
    real precisionTargetMinStations;
    int maxIterations;
    bool useBroydenMethod;
    bool useSeed;
    real* seedMonthlyTimes;
    int n_seedMonthlyTimes;
    real* seedMonthlyValues;
    int n_seedMonthlyValues;

    /// Spatial Map Approximation Parameters
    real spatialMapsEmptyCellCut;
    real spatialMapsTrivialMaxCut;
    real spatialMapsTrivialSumCut;

    /// Empirical Region Determination Parameters
    bool limitEmpiricalFitRegion;
    real empiricalFitRegionCutoff;
    bool limitEmpiricalBaselineRegion;
    real empiricalBaselineRegionCutoff;

    /// Baseline Mapping Options
    bool fullBaselineMapping;
    int n_fullBaselineTargetLats;
    real* fullBaselineTargetLats;
    int fullBaselineAltitudeDegree;

    /// Uncertainty Computation Options
    bool computeUncertainty;
    bool computeStatisticalUncertainty;
    bool statisticalUncertaintyLocal;
    bool useJackKnife;
    int statisticalUncertaintyInnerLoops;
    int statisticalUncertaintyOuterLoops;
    int statisticalUncertaintyMinRepeats;
    int statisticalUncertaintyBenchmarkMinDate;
    int statisticalUncertaintyBenchmarkMaxDate;
    bool computeEmpiricalSpatialUncertainty;
    bool supplementEmpiricalSpatialWithAnalytic;
    int spatialUncertaintyBenchmarkMinDate;
    int spatialUncertaintyBenchmarkMaxDate;
    bool computeAnalyticSpatialUncertainty;

    bool clusterMode;
} berkeleyAverageOptions;

/**
  * creates berkeleyAverageOptions object with default values
  * @returns the new berkeleyAverageOptions instance
  */
berkeleyAverageOptions* createBAON();

/**
  * creates berkeleyAverageOptions object with some predefined configurations
  * @param mode the possible mode parameters are: "quick", "weighted", "local", "complete"
  * @returns the new berkeleyAverageOptions instance
  */
berkeleyAverageOptions* createBAON1(char* mode);

/**
  * creates berkeleyAverageOptions object with the predefined "quick" configuration
  * @returns the new berkeleyAverageOptions instance
  */
berkeleyAverageOptions* createBerkeleyAverageOptionsQuick();

/**
  * checks the consistency of the berkeleyAverageOptions object
  * @param p input berkeleyAverageOptions instance pointer
  */
void berkeleyAverageOptionsConsistency(berkeleyAverageOptions* p);

/**
  * displays the contents of the parameter berkeleyAverageOptions instance
  * @param bao input berkeleyAverageOptions instance pointer
  */
void displayBAO(berkeleyAverageOptions* bao);

void destroyBAO(berkeleyAverageOptions* bao);

#endif

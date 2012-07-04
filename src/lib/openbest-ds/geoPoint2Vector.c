#include "openbest-ds/geoPoint2Vector.h"
#include "openbest-ds/temperatureGlobals.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"

geoPoint2Vector* createGeoPoint2VectorN()
{
  geoPoint2Vector* p= (geoPoint2Vector*)malloc(sizeof(geoPoint2Vector));
  
  if ( !p )
    eprintf("malloc failed in createGeoPoint2VectorN");
  
  p->t= NULL;
  p->n= 0;
}

geoPoint2Vector* createGeoPoint2Vector1(int n)
{
  geoPoint2Vector* p= createGeoPoint2VectorN();
  p->t= (geoPoint2*)(malloc(sizeof(geoPoint2)*n));
  
  if ( ! (p->t) )
    eprintf("malloc failed in createGeoPoint2Vector1");
  p->n= n;
  return p;
}

void destroyGeoPoint2Vector(geoPoint2Vector* gpv)
{
  int i;
  for ( i= 0; i < gpv->n; ++i )
    free(gpv->t + i);
  free(gpv);
}

void displayGP2V(geoPoint2Vector* p)
{
  printf("%d geoPoints", p->n);
  fflush(stdout);
}

real* distanceGP2V(geoPoint2Vector* pv, geoPoint2* p)
{
  real* res= (real*)malloc(sizeof(real)*(pv->n));
  
  if ( !res )
    eprintf("malloc failed in distanceGP2V");
  
  int i;
  for ( i= 0; i < pv->n; ++i )
    res[i]= distanceGP2(pv->t + i, p);
}

geoPoint* centerGP2V(geoPoint2Vector* pv)
{
  real x, y, z;
  x= y= z= 0;
  
  int i;
  for ( i= 0; i < pv->n; ++i )
  {
    x+= pv->t[i].x;
    y+= pv->t[i].y;
    z+= pv->t[i].z;
  }
  x/= pv->n;
  y/= pv->n;
  z/= pv->n;
  
  real d= sqrt(x*x + y*y + z*z);
  x/= d;
  y/= d;
  z/= d;
  
  real lat= 90 - acos(z)*180/M_PI;
  real longitude= atan2(y, x)*180/M_PI;
  
  return createGeoPoint2(lat, longitude);
}

int nearest1GP2V(geoPoint2* p, geoPoint2Vector* pv)
{
  real minDist= FLT_MAX;
  int minIdx= 0;
  real tmp;
  int i;
  for ( i= 0; i < pv->n; ++i )
  {
    tmp= distanceGP2(pv->t + i, p);
    if ( tmp < minDist )
    {
      minDist= tmp;
      minIdx= i;
    }
  }
  
  return minIdx;
}

int* nearestNGP2V(geoPoint2* p, geoPoint2Vector* pv, int n)
{
  real* dist= distanceGP2V(pv, p);
  int* res= (int*)(malloc(sizeof(int)*n));
  
  if ( !res )
    eprintf("malloc failed in nearestN");
  
  real minDist= FLT_MAX;
  int minIdx= 0;
  real tmp;
  while ( n > 0 )
  {
      --n;
      
      int i;
      for ( i= 0; i < pv->n; ++i )
      {
	tmp= dist[i];
	if ( tmp < minDist )
	{
	  minDist= tmp;
	  minIdx= i;
	}
      }
      
      res[n]= minIdx;
      dist[minIdx]= FLT_MAX;
  }
  
  free(dist);
  
  return res;
}

void rotateGP2V(geoPoint2Vector* g, matrix3 rot)
{
  int i;
  for ( i= 0; i < g->n; ++i )
    rotateGP2(g->t+i, rot);
}

int isnanGP2V(geoPoint2Vector* g)
{
  int f= 1;
  int i;
  for ( i= 0; i < g->n; ++i )
    f&= isnanGP2(g->t + i);
  return f;
}

geoPoint2Vector* setUncertaintyGP2V1(geoPoint2Vector* gpv, real lat_error, real long_error, real elev_error)
{
  int i;
  for ( i= 0; i < gpv->n; ++i )
    setUncertaintyGP2(gpv->t + i, lat_error, long_error, elev_error);
  return gpv;
}

geoPoint2Vector* setUncertaintyGP2VA(geoPoint2Vector* gpv, real* lat_error, real* long_error, real* elev_error)
{
  int i;
  for ( i= 0; i < gpv->n; ++i )
    setUncertaintyGP2(gpv->t + i, lat_error[i], long_error[i], elev_error[i]);
  return gpv;
}

ucScale uncertaintyScaleGP2(geoPoint2* gp2)
{
  ucScale ucs;
  ucs.lat= 2*M_PI*earth_radius*(gp2->lat_uncertainty)/360.0;
  ucs.longitude= 2*M_PI*earth_radius*(gp2->lat_uncertainty)/360.0 * cos(gp2->latitude * M_PI/180.0);
  
  return ucs;
}

ucScale* uncertaintyScaleGP2V(geoPoint2Vector* gp2)
{
  int i;
  ucScale* res= (ucScale*)malloc(sizeof(ucScale)*(gp2->n));
  if ( !res )
      eprintf("malloc failed in uncertaintyScaleGP2V");
    
  for ( i= 0; i < gp2->n; ++i )
    res[i]= uncertaintyScaleGP2(gp2->t+i);
  
  return res;
}

geoPoint2Vector* merge(geoPoint2Vector* p)
{
  if ( p->n == 1 )
    return p;
  else if ( p->n == 0 )
  {
    geoPoint2Vector* tmp= createGeoPoint2VectorN(1);
    return tmp;
  }
  else
    return overlapTiles(p);
}

void getLatitudes(geoPoint2Vector* p, real* res)
{
  int i;
  for ( i= 0; i < p->n; ++i )
    res[i]= p->t[i].latitude;
}

void getLongitudes(geoPoint2Vector* p, real* res)
{
  int i;
  for ( i= 0; i < p->n; ++i )
    res[i]= p->t[i].longitude;
}

void getElevations(geoPoint2Vector* p, real* res)
{
  int i;
  for ( i= 0; i < p->n; ++i )
    res[i]= p->t[i].elevation;
}

void getLatUncertainties(geoPoint2Vector* p, real* res)
{
  int i;
  for ( i= 0; i < p->n; ++i )
    res[i]= p->t[i].lat_uncertainty;
}

void getLongUncertainties(geoPoint2Vector* p, real* res)
{
  int i;
  for ( i= 0; i < p->n; ++i )
    res[i]= p->t[i].long_uncertainty;
}

void getElevUncertainties(geoPoint2Vector* p, real* res)
{
  int i;
  for ( i= 0; i < p->n; ++i )
    res[i]= p->t[i].elev_uncertainty;
}

geoPoint2Vector* overlapTiles(geoPoint2Vector* p)
{
  /*real offset= 0;
  real range= 1e6;
  
  int i, j;
  int n= p->n;
  real* long2= rnalloc(n);
  real* longitude= rnalloc(n);
  real* latitude= rnalloc(n);
  real* elevation= rnalloc(n);
  real* lat_error= rnalloc(n);
  real* long_error= rnalloc(n);
  real* elev_error= rnalloc(n);
  getLongitudes(p, longitude);
  getLatitudes(p, latitude);
  getElevations(p, elevation);
  getLatUncertainties(p, lat_error);
  getLongUncertainties(p, long_error);
  getElevUncertainties(p, elev_error);
  
  real range2;
  for ( i= 0; i < p->n; ++i )
  {
    for ( j= 0; j < p->n; ++j )
      long2[j]= longitude[j] + longitude[i];
    
    for ( j= 0; j < p->n; ++j )
      long2[j]= mod(long2[j], 360.0);
    
    range2= max(long2, p->n) - min(long2, p->n);
    
    if ( range2 < range )
    {
      offset= longitude[i];
      range= range2;
    }
  }
  
  for ( j= 0; j < p->n; ++j )
    long2[j]= mod(longitude[j] + offset, 360);
  
  char* f= cnalloc(n);
  char* f2= cnalloc(n);
  isnanV(elevation, n, f);
  isnanV(elev_error, n, f2);
  for ( i= 0; i < n; ++i )
    f[i]|= f2[i];
  free(f2);
  
  real* source_set= rnalloc(n*4);
  for ( i= 0; i < n; ++i )
  {
    source_set[i*4 + 0]= longitude[i] - long_error[i];
    source_set[i*4 + 1]= latitude[i] - lat_error[i];
    source_set[i*4 + 2]= longitude[i] + long_error[i];
    source_set[i*4 + 3]= latitude[i] + lat_error[i];
  }
  
  real tiles[100][4];
  int tc= n;
  
  for ( i= 0; i < tc; ++i )
  {
    tiles[i][0]= source_set[i*4 + 0];
    tiles[i][1]= source_set[i*4 + 1];
    tiles[i][2]= source_set[i*4 + 2];
    tiles[i][3]= source_set[i*4 + 3];
  }
  
  real* scales= rnalloc(tc*2);
  
  for ( i= 0; i < tc; ++i )
  {
    scales[i*2 + 0]= tiles[i][3] - tiles[i][1];
    scales[i*2 + 1]= tiles[i][4] - tiles[i][2];
  }
  
  real* max_vals= rnalloc(tc);
  real* rescale= rnalloc(tc*2);
  
  unsigned char* tile_owners= cnalloc(100*100);
  unsigned char* tile_owner_sizes= cnalloc(100);
  for ( i= 0; i < tc; ++i )
  {
    tile_owners[i*100]= i;
    tile_owner_sizes[i]= 1;
  }
  
  int tile_start= 1;
  
  int t11, t12, t13, t14, t21, t22, t23, t24;
  
  real region1[36];
  real region2[36];
  real intersect[36];
  int c1, c2, c3;
  
  while ( tile_start < tc )
  {
    int tile_target= tile_start + 1;
    
    while ( tile_target <= tc )
    {
      t11= tiles[tile_start][0]; t12= tiles[tile_start][1]; t13= tiles[tile_start][2]; t14= tiles[tile_start][3];
      t21= tiles[tile_target][0]; t22= tiles[tile_target][1]; t23= tiles[tile_target][2]; t24= tiles[tile_target][3];
      
      if ( (t11 < t21 && t13 <= t21) || (t11 >= t23 && t13 > t23) || (t12 < t22 && t14 <= t22) || (t12 >= t24 && t14 > t24))
	tile_target= tile_target + 1;
      continue;
      
      breakOverlap(tiles + tile_start, tiles + tile_target, &region1, &region2, &intersect, &c1, &c2, &c3);
      
      int n= c1 + c2 + c3;
      real* new_tiles= rnalloc(n * 4);
      for ( i= 0; i < c1; ++i )
      {
	new_tiles[i*4 + 0]= region1[i*4 + 0];
	new_tiles[i*4 + 1]= region1[i*4 + 1];
	new_tiles[i*4 + 2]= region1[i*4 + 2];
	new_tiles[i*4 + 3]= region1[i*4 + 3];
      }
      for ( i= c1; i < c1 + c2; ++i )
      {
	new_tiles[i*4 + 0]= region2[(i-c1)*4 + 0];
	new_tiles[i*4 + 1]= region2[(i-c1)*4 + 1];
	new_tiles[i*4 + 2]= region2[(i-c1)*4 + 2];
	new_tiles[i*4 + 3]= region2[(i-c1)*4 + 3];
      }
      for ( i= c1 + c2; i < c1 + c2 + c3; ++i )
      {
	new_tiles[i*4 + 0]= intersect[(i-c1-c2)*4 + 0];
	new_tiles[i*4 + 1]= intersect[(i-c1-c2)*4 + 1];
	new_tiles[i*4 + 2]= intersect[(i-c1-c2)*4 + 2];
	new_tiles[i*4 + 3]= intersect[(i-c1-c2)*4 + 3];
      }
      
      char* new_source= cnalloc(100*100);
      char* new_source_sizes= cnalloc(100);
      
      for ( i= 0; i < c1; ++i )
      {
	for ( j= 0; j < tile_owner_sizes[i]; ++j )
	  new_source[i*100 + j]= tile_owners[tile_start*100 + j];
	new_source_sizes[i]= tile_owner_sizes[i];
      }
	
      for ( i= c1; i < c1 + c2; ++i )
      {
	for ( j= 0; j < tile_owner_sizes[i]; ++j )
	  new_source[i*100 + j]= tile_owners[tile_target*100 + j];
	new_source_sizes[i]= tile_owner_sizes[i];
      }
      
      char join[100];
      int jn;
      union(tile_owners + tile_start*100, tile_owner_sizes[tile_start], tile_owners + tile_target*100, tile_owner_sizes[tile_target], join, &jn);
      
      for ( i= c1 + c2; i < c1 + c2 + c3; ++i )
      {
	for ( j= 0; j < jn; ++j )
	  new_source[i*100 + j]= join[j];
      }
      
      tiles[tile_start*4 + 0]= new_tiles[0];
      tiles[tile_start*4 + 1]= new_tiles[1];
      tiles[tile_start*4 + 2]= new_tiles[2];
      tiles[tile_start*4 + 3]= new_tiles[3];
      
      for ( j= 0; j < new_source_sizes[0]; ++j )
	tile_owners[tile_start*100 + j]= new_sources[j];
      
      if ( c1 + c2 + c3 > 1 )
      {
	tiles[tile_target*4 + 0]= new_tiles[1*4 + 0];
	tiles[tile_target*4 + 1]= new_tiles[1*4 + 1];
	tiles[tile_target*4 + 2]= new_tiles[1*4 + 2];
	tiles[tile_target*4 + 3]= new_tiles[1*4 + 3];
	
	for ( j= 0; j < new_source_sizes[1]; ++j )
	  tile_owners[tile_target*100 + j]= new_source[1*100 + j];
	tile_owner_sizes[tile_target]= new_source_sizes[1];
      }
      else
      {
	tiles[tile_target + 0]= -1;
	tiles[tile_target + 1]= -1;
	tiles[tile_target + 2]= -1;
	tiles[tile_target + 3]= -1;
	tile_owner_sizes[tile_target]= 0;
	--tc;
	continue;
      }
      
      k= 2
      for ( i= tc + 1; i < tc + c1 + c2 + c3 - 2; ++i,++k )
	for ( j= 0; j < 4; ++j )
	  tiles[i*4 + j]= new_tiles[k*4 + j];
	
      k= 2;
      for ( i= tc + 1; i < tc + c1 + c2 + c3 - 2; ++i, ++k )
      {
	for ( j= 0; j < new_source_sizes[k]; ++j )
	  tile_owners[i*100 + j]= new_source[k*100 + j];
	tile_owner_sizes[i]= new_source_sizes[i];
      }
      
      tc= tc + c1 + c2 + c3 - 2;
      
      if ( tc == 100 )
      {
	///tiles(end+1:end+100,:) = 0;
	tile_owner_sizes[100]= 0;
      }
      
      tile_target= tile_start + 1;
    }
    
    tile_start= tile_start + 1;
    }
    
    for ( i= tc + 1; i < 100; ++i )
      tiles[i*4 + 0]= tiles[i*4 + 1]= tiles[i*4 + 2]= tiles[i*4 + 3]= -1;
    
    for ( k= 0; k < tc; ++k )
    {
      char* to= tile_owners + k*100;
      int cnt= tile_owner_sizes[k];
      
      char* f= cnalloc(cnt);
      real* max_vals_to= rnalloc(cnt);
      for ( j= 0; j < tile_owner_sizes[k]; ++j )
	max_vals_to[j]= max_vals[to[j]];
      
      lessthenr(max_vals_to, tile_owner_sizes[k], cnt, f);
      
      for ( j= 0; j < tile_owner_sizes[k]; ++j )
	if ( f[j] )
	  max_vals[to[j]]= cnt;
      
      real min0= scales[0], min1= scales[1];
      for ( i= 0; i < tc; ++i )
      {
	if ( scales[i*2 + 0] < min0 )
	  min0= scales[i*2 + 0];
	if ( scales[i*2 + 1] < min1 )
	  min1= scales[i*2 + 1];
      }
      rescale[k*2 + 0]= min0;
      rescale[k*2 + 1]= min1;
    }
     
    char* bad= cnalloc(tc);
    set(bad, tc, 0);
    
    int to;
    for ( k= 0; k < tc; ++k )
    {
      to= tile_owners + k*100;
      int cnt= tile_owner_sizes[k];
      char* f= cnalloc(cnt);
      
      real* max_vals_to= rnalloc(cnt);
      for ( j= 0; j < tile_owner_sizes[k]; ++j )
	max_vals_to[j]= max_vals[to[j]];
      
      equalsr(max_vals_to, cnt, cnt, f);
      int any= 0;
      for ( j= 0; j < cnt; ++j )
	any+= f[j];
      if ( !any )
	bad[k]= 1;
    }
    
    for ( k= 0; k < tc; ++k )
      if ( bad[k] )
      {
	tiles[k*4+0]= -1;
	tiles[k*4+1]= -1;
	tiles[k*4+2]= -1;
	tiles[k*4+3]= -1;
      }
     
    for ( k= 0; k < tc; ++k )
      if ( bad[k] )
      {
	rescale[k*2 + 0]= -1;
	rescale[k*2 + 1]= -1;
      }
      
    new_tiles= rnalloc(100*4);
    set(new_tiles, 400, 0);
    
    for ( k= 1; k < 100; ++k )
    {
      real x= (tiles[k][0] + tiles[k][2])/2;
      real y= (tiles[k][1] + tiles[k][3])/2;
      new_tiles[k*4 + 0]= x-rescale[k*2 + 0]/2.0;
      new_tiles[k*4 + 1]= y - rescale[k*2 + 1]/2.0;
      new_tiles[k*4 + 2]= x + rescale[k*2 + 0]/2.0;
      new_tiles[k*4 + 3]= y + rescale[k*2 + 1]/2.0;
    }
    
    real min0= new_tiles[0], min1= new_tiles[1], max2= new_tiles[2], max3= new_tiles[3];
    
    for ( i= 0; i < 100; ++i )
    {
      if ( min0 < new_tiles[i*4 + 0] )
	min0= new_tiles[i*4 + 0];
      if ( min1 < new_tiles[i*4 + 1] )
	min1= new_tiles[i*4 + 1];
      if ( max2 > new_tiles[i*4 + 2] )
	max2= new_tiles[i*4 + 2];
      if ( max3 > new_tiles[i*4 + 3] )
	max3= new_tiles[i*4 + 3];
    }
    
    real new_long= (min0 + max2)/2.0;
    real new_lat= (min1 + max3)/2.0;
    real new_long_unc= (max2 - min0)/2.0;
    real new_lat_unc= (max3 - min1)/2.0;
    
    real new_elev= FLT_MAX;
    real new_elev_unc= FLT_MAX;
    
    for ( i= 0; i < n; ++i )
      if ( elevation[i] != FLT_MAX )
      {
	if ( n == 1 )
	{
	  new_elev= elev[0];
	  new_elev_unc= elev_error[0];
	  break;
	}
	else
	{
	  real* elev_list= rnalloc(n*2);
	  for ( j= 0; j < n; ++j )
	  {
	    elev_list[j*2 + 0]= elev[j] - elev_error[j];
	    elev_list[j*2 + 1]= elev[j] + elev_error[j];
	  }
	  
	  real* elev_pts;
	  int elev_pts_n;
	  unique(elev_list, n, 2, &elev_pts, &elev_pts_n);
	  
	  char* usage= cnalloc((elev_pts_n-1)*n);
	  
	  for ( k= 0; k < n; ++k )
	    for ( j= 0; j < elev_pts_n-1; ++j )
	      if ( elev_pts[j] >= elev_list[k*2 + 0] && elev_pts[j+1] <= elev_pts[j+1] <= elev_list[k*2 + 1] )
		usage[j*n + k]= 1;
	  
	  real* usage_max= rnalloc(n);
	  set(usage_max, n, 0);
	  for ( j= 0; j < n; ++j )
	    for ( k= 0; k < elev_pts_n - 1; ++k )
	      usage_max[j]+= usage[j*(elev_pts_n-1)+k];
	    
	  char* used= cnalloc(n);
	  setc(used, n, 0);
	  
	  char* items= cnalloc(elev_pts - 1);
	  for ( k= 0; k < n; ++k )
	  {
	    for ( j= 0; j < n; ++j )
	      if ( usage[j*n + k] == 1 )
		items[j]= 1;
	      else
		items[j]= 0;
	      
	      int max= 0;
	      int l;
	      for ( l= 0; l < elev_pts - 1; ++l )
		if ( items[l] )
		  if ( usage_max[l] > max )
		    max= usage_max[l];
	      used[k]= max;
	      
	  }
	  char* needed= cnalloc(elev_pts_n - 1);
	  for ( k= 0; k < elev_pts_n - 1; ++k )
	  {
	    for ( j= 0; j < n; ++j )
	      if ( usage[j*n + k] == 1 )
		items[j]= 1;
	      else
		items[j]= 0;
	    char* f= cnalloc(elev_pts_n - 1);
	    for ( j= 0; j < elev_pts_n - 1; ++j )
	      if ( items[j] )
		if ( used[j] == useage_max[k] )
		  f[j]= 1;
		else
		  f[j]= 0;
	    int sum= 0;
	    for ( j= 0; j < elev_pts_n - 1; ++j )
	      sum+= f[j];
	    if ( f[j] )
	      needed[k]= 1;
	  }
	  
	  char* f;
	  int f_n;
	  find(needed, &f, &f_n);
	  
	  real min_elev= elev_pts[f[0]];
	  real max_elev= elev_pts[f[f_n-1]];
	  
	  real new_elev= (max_elev + min_elev)/2.0;
	  real new_elev_unc= (max_elev - min_elev)/2.0;
	  
	  real min_elev_error= elev_error[0];
	  for ( i= 0; i < n; ++i )
	    if ( min_elev_error > elev_error[i] )
	      min_elev_error= elev_error[i];
	  
	  if ( new_elev_unc < min_elev_error )
	    new_elev_unc= min_elev_error;
	}
      }
    geoPoint2* result= createGeoPoint2(new_lat, new_long - offset, new_elev, new_lat_unc, new_long_unc, new_elev_unc);
    
    return result;*/
  
    return NULL;
  }
  
  
    void breakOverlap(real* t1, real* t2, real** region1, real** region2, real** intersect, int* c1p, int* c2p, int* c3p)
  {
    real xs[4];
    real ys[4];
    int c1= 0, c2= 0, c3= 0;
    real new_tiles[36];
    int i, j, k;
    real* tile;
    bool in1, in2;
    
    xs[0]= t1[0]; xs[1]= t1[2]; xs[2]= t2[0]; xs[3]= t2[2];
    ys[0]= t1[1]; ys[1]= t1[3]; ys[2]= t2[1]; ys[3]= t2[3];
    
    quicksort(xs, 0, 3);
    quicksort(ys, 0, 3);
    
    new_tiles[0]= xs[0]; new_tiles[1]= ys[0]; new_tiles[2]= xs[1]; new_tiles[3]= ys[1];
    new_tiles[4]= xs[0]; new_tiles[5]= ys[1]; new_tiles[6]= xs[1]; new_tiles[7]= ys[2];
    new_tiles[8]= xs[0]; new_tiles[9]= ys[2]; new_tiles[10]= xs[1]; new_tiles[11]= ys[3];
    new_tiles[12]= xs[1]; new_tiles[13]= ys[0]; new_tiles[14]= xs[2]; new_tiles[15]= ys[1];
    new_tiles[16]= xs[1]; new_tiles[17]= ys[1]; new_tiles[18]= xs[2]; new_tiles[19]= ys[2];
    new_tiles[20]= xs[1]; new_tiles[21]= ys[2]; new_tiles[22]= xs[2]; new_tiles[23]= ys[3];
    new_tiles[24]= xs[2]; new_tiles[25]= ys[0]; new_tiles[26]= xs[3]; new_tiles[27]= ys[1];
    new_tiles[28]= xs[2]; new_tiles[29]= ys[1]; new_tiles[30]= xs[3]; new_tiles[31]= ys[2];
    new_tiles[32]= xs[2]; new_tiles[33]= ys[2]; new_tiles[34]= xs[3]; new_tiles[35]= ys[3];
    
    for ( k= 0; k < 9; ++k )
    {
      tile= new_tiles + k*4;
      
      if ( tile[0] == tile[2] || tile[1] == tile[3] )
	continue;
      
      if ( tile[0] >= t1[0] && tile[2] <= t1[2] && tile[1] >= t1[1] && tile[3] <= t1[3] )
	in1= true;
      else
	in1= false;
      
      if ( tile[0] >= t2[0] && tile[2] <= t2[2] && tile[1] >= t2[1] && tile[3] <= t2[3] )
	in2= true;
      else
	in2= false;
      
      if ( in1 && in2 )
	++c3;
      else if ( in1 )
	++c1;
      else if ( in2 )
	++c2;
      
    }
    
    *region1= rnalloc(4*c1);
    *region2= rnalloc(4*c2);
    *intersect= rnalloc(4*c3);
        
    set(*region1, c1*4, 0);
    set(*region2, c2*4, 0);
    set(*intersect, c3*4, 0);
    
    c1= c2= c3= 0;
    for ( k= 0; k < 9; ++k )
    {
      tile= new_tiles + k*4;
      
      if ( tile[0] == tile[2] || tile[1] == tile[3] )
	continue;
      
      if ( tile[0] >= t1[0] && tile[2] <= t1[2] && tile[1] >= t1[1] && tile[3] <= t1[3] )
	in1= true;
      else
	in1= false;
      
      if ( tile[0] >= t2[0] && tile[2] <= t2[2] && tile[1] >= t2[1] && tile[3] <= t2[3] )
	in2= true;
      else
	in2= false;
      
      if ( in1 && in2 )
      {
	++c3;
	(*intersect)[c3*4 + 0]= tile[0];
	(*intersect)[c3*4 + 1]= tile[1];
	(*intersect)[c3*4 + 2]= tile[2];
	(*intersect)[c3*4 + 3]= tile[3];
      }
      else if ( in1 )
      {
	++c1;
	(*region1)[c1*4 + 0]= tile[0];
	(*region1)[c1*4 + 1]= tile[1];
	(*region1)[c1*4 + 2]= tile[2];
	(*region1)[c1*4 + 3]= tile[3];
      }
      else if ( in2 )
      {
	++c2;
	(*region2)[c2*4 + 0]= tile[0];
	(*region2)[c2*4 + 1]= tile[1];
	(*region2)[c2*4 + 2]= tile[2];
	(*region2)[c2*4 + 3]= tile[3];
      }
    }
    
    *c1p= c1;
    *c2p= c2;
    *c3p= c3;
  }

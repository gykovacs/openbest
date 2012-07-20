#include <string.h>

#include "openbest-ds/config-ds.h"
#include "openbest-ds/primaryKeyTable.h"

valueToKeyTree* newVTKElement(char* key, int value)
{
  valueToKeyTree* tmp= (valueToKeyTree*)malloc(sizeof(valueToKeyTree));
  if ( tmp == NULL )
      eprintf("malloc failed in newVTKElement\n");
  tmp->keys= (char**)malloc(sizeof(char*)*2);
  if ( tmp->keys == NULL )
      eprintf("malloc failed in newVTKElement\n");
  tmp->keys[0]= (char*)cnalloc(strlen(key) + 1);
  strcpy(tmp->keys[0], key);
  tmp->keys[1]= NULL;
  tmp->n= 2;
  tmp->value= value;
  tmp->left= tmp->right= NULL;

  return tmp;
}

char* insertIntoVTK(valueToKeyTree** root, char* key, int value)
{
  if ( *root == NULL )
  {
    *root= newVTKElement(key, value);
    return (*root)->keys[0];
  }
  else
  {
    if ( (*root)->value > value )
      return insertIntoVTK(&((*root)->left), key, value);
    else if ( (*root)->value < value )
      return insertIntoVTK(&((*root)->right), key, value);
    else if ((*root)->value == value )
    {
      ((*root)->n)++;
      (*root)->keys= (char**)realloc((*root)->keys, sizeof(char*)*((*root)->n));
      ((*root)->keys)[(*root)->n-2]= cnalloc(strlen(key) + 1);
      strcpy(((*root)->keys)[(*root)->n-2], key);
      ((*root)->keys)[(*root)->n-1]= NULL;
      
      return (*root)->keys[(*root)->n-2];
    }
  }
}

keyToValueTree* newKTVElement(char* key, int value)
{
  keyToValueTree* tmp= (keyToValueTree*)malloc(sizeof(keyToValueTree));
  if ( tmp == NULL )
      eprintf("malloc failed in newKTVElement");
  tmp->key= key;
  tmp->value= value;
  tmp->left= tmp->right= NULL;
  
  return tmp;
}

void insertIntoKTV(keyToValueTree** root, char* key, int value)
{
  if ( *root == NULL )
  {
    *root= newKTVElement(key, value);
  }
  else
  {
    if ( strcmp((*root)->key, key) > 0 )
      insertIntoKTV(&((*root)->left), key, value);
    else if ( strcmp((*root)->key, key) < 0 )
      insertIntoKTV(&((*root)->right), key, value);
    else if ( strcmp((*root)->key, key) == 0 )
      (*root)->value= value;
  }
}

char** lookupKeysVTK(valueToKeyTree* root, int value)
{
  if ( root == NULL )
    return &unknown_value;
  if ( root->value == value )
    return root->keys;
  if ( root->value > value )
    return lookupKeysVTK(root->left, value);
  return lookupKeysVTK(root->right, value);
}

char** lookupKeysPKT(primaryKeyTable* pkt, int value)
{
  return lookupKeysVTK(pkt->vtk, value);
}

int lookupValueKTV(keyToValueTree* root, char* key)
{
  if ( root == NULL )
    return 0;
  else if ( strcmp(root->key, key) > 0 )
    return lookupValueKTV(root->left, key);
  else if ( strcmp(root->key, key) < 0 )
    return lookupValueKTV(root->right, key);
  return root->value;
}

int lookupValuePKT(primaryKeyTable* pkt, char* key)
{
  return lookupValueKTV(pkt->ktv, key);
}

primaryKeyTable createPrimaryKeyTableN()
{
  primaryKeyTable pkt;
  pkt.vtk= pkt.ktv= NULL;
  return pkt;
}

primaryKeyTable* createPrimaryKeyTableP()
{
  primaryKeyTable* pkt= (primaryKeyTable*)malloc(sizeof(primaryKeyTable));
  pkt->vtk= pkt->ktv= NULL;
  return pkt;
}

void insertIntoPKT(primaryKeyTable* pkt, char* key, int value)
{
  char* tmp= insertIntoVTK(&(pkt->vtk), key, value);
  insertIntoKTV(&(pkt->ktv), tmp, value);
}

void destroyVTK(valueToKeyTree** root)
{
  if ( *root == NULL )
    return;

  if ( (*root)->left != NULL )
    destroyVTK(&((*root)->left));
  
  if ( (*root)->right != NULL )
    destroyVTK(&((*root)->right));
  
  int i;
  for ( i= 0; i < (*root)->n - 1; ++i )
    free((*root)->keys[i]);
  free((*root)->keys);
  free((*root));
  *root= NULL;
}

void destroyKTV(keyToValueTree** root)
{
  if ( *root == NULL )
    return;
  
  if ( (*root)->left != NULL )
    destroyKTV(&((*root)->left));
  
  if ( (*root)->right != NULL )
    destroyKTV(&((*root)->right));
  
  if ( (*root)->left == NULL && (*root)->right == NULL )
  {
    free((*root));
    *root= NULL;
  }
}

void destroyPKT(primaryKeyTable* pkt)
{
  destroyVTK(&(pkt->vtk));
  destroyKTV(&(pkt->ktv));
  free(pkt);
}

void displayPKTbyValue(primaryKeyTable* pkt)
{
  displayVTK(pkt->vtk);
}

void displayPKTbyKeys(primaryKeyTable* pkt)
{
  displayKTV(pkt->ktv);
}

void displayVTK(valueToKeyTree* root)
{
  if ( root == NULL )
    return;
  if ( root->left != NULL )
    displayVTK(root->left);
  printf("%d - ", root->value);
  int i;
  printf("n: %d\n", root->n); fflush(stdout);
  for ( i= 0; i < root->n - 1; ++i )
    printf("%s, ", root->keys[i]);
  printf("\n");
  if ( root->right != NULL )
    displayVTK(root->right);
}

void displayKTV(keyToValueTree* root)
{
  if ( root == NULL )
    return;
  if ( root->left != NULL )
    displayKTV(root->left);
  printf("%s - %d\n", root->key, root->value);
  if ( root->right != NULL )
    displayKTV(root->right);
}

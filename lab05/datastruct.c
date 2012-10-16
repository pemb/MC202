#include <stdlib.h>
#include "balloc.h"

/* #define MALLOC malloc */
/* #define FREE free */

typedef struct NoArv 
{
  int dado;
  struct NoArv * pai;
  struct NoArv * esq, * dir;
} no;

void rotacaoDir( no * pai, no ** refAvo, no * avo)
{
  no * filho = pai->esq;
  if (refAvo != NULL)
    {
      *refAvo = filho;
      filho->pai = avo;
    }
  else
    filho->pai = NULL;
  pai->esq = filho->dir;
  if (filho->dir != NULL)
    filho->dir->pai = pai;
  filho->dir = pai;
  pai->pai = filho;  
}

void rotacaoEsq( no * pai, no ** refAvo, no * avo)
{
  no * filho = pai->dir;
  if (avo)
    {
      *refAvo = filho;
      filho->pai = avo;
    }
  else
    filho->pai = NULL;
  pai->dir = filho->esq;
  if (filho->esq != NULL)
  filho->esq->pai = pai;
  filho->esq = pai;
  pai->pai = filho;  
}

void afunila ( no * novo, no ** arvore)
{
  while (novo->pai != NULL)
    {
      /* se for filho esquerdo */
      if (novo->pai->esq == novo)
	{ /* se o pai é raiz */
	  if (novo->pai->pai == NULL)
	    rotacaoDir(novo->pai, arvore, NULL);
	  /* caso homogêneo esquerdo */
	  else if (novo->pai->pai->esq == novo->pai)
	    {
	      /* se o avô é raiz */
	      if (novo->pai->pai->pai == NULL)
		{
		  rotacaoDir(novo->pai->pai, arvore, NULL);
		  rotacaoDir(novo->pai, arvore, NULL);
		}
	      else 
		{
		  if (novo->pai->pai->pai->esq == novo->pai->pai)
		    rotacaoDir(novo->pai->pai, &(novo->pai->pai->pai->esq), novo->pai->pai->pai);
		  else
		    rotacaoDir(novo->pai->pai, &(novo->pai->pai->pai->dir), novo->pai->pai->pai);
		  rotacaoDir(novo->pai, &(novo->pai->pai->esq), novo->pai->pai);
		}
	    }
	  /* caso heterogêneo esquerdo */
	  else
	    {
	      rotacaoDir(novo->pai, &(novo->pai->pai->dir), novo->pai->pai);
	      /* se o novo pai é raiz */
	      if (novo->pai->pai == NULL)
		rotacaoEsq(novo->pai, arvore, NULL);
	      else if (novo->pai->pai->esq == novo->pai)
		rotacaoEsq(novo->pai, &(novo->pai->pai->esq), novo->pai->pai);
	      else
		rotacaoEsq(novo->pai, &(novo->pai->pai->dir), novo->pai->pai);
	    }
	}
      /* exatamente a mesma coisa, só que esq <-> dir */
      else
	{
	  if (novo->pai->pai == NULL)
	    rotacaoEsq(novo->pai, arvore, NULL);
	  else if (novo->pai->pai->dir == novo->pai)
	    {
	      if (novo->pai->pai->pai == NULL)
		{
		  rotacaoEsq(novo->pai->pai, arvore, NULL);
		  rotacaoEsq(novo->pai, arvore, NULL);
		}
	      else 
		{
		  if (novo->pai->pai->pai->dir == novo->pai->pai)
		    rotacaoEsq(novo->pai->pai, &(novo->pai->pai->pai->dir), novo->pai->pai->pai);
		  else
		    rotacaoEsq(novo->pai->pai, &(novo->pai->pai->pai->esq), novo->pai->pai->pai);
		  rotacaoEsq(novo->pai, &(novo->pai->pai->dir), novo->pai->pai);
		}
	    }
	  else
	    {
	      rotacaoEsq(novo->pai, &(novo->pai->pai->esq), novo->pai->pai);
	      if (novo->pai->pai == NULL)
		rotacaoDir(novo->pai, arvore, NULL);
	      else if (novo->pai->pai->dir == novo->pai)
		rotacaoDir(novo->pai, &(novo->pai->pai->dir), novo->pai->pai);
	      else
		rotacaoDir(novo->pai, &(novo->pai->pai->esq), novo->pai->pai);
	    }
	}
    } /* eu acho que isso deve cobrir todos os casos */  
}

no * busca (no * arvore, int dado )
{
  if (arvore == NULL)
    return NULL;
  while (1)
    {
      if (dado < arvore->dado && arvore->esq != NULL)
	arvore = arvore->esq;
      else if (dado > arvore->dado && arvore->dir != NULL)
	arvore = arvore->dir;
      else
	return arvore;
    }
}


void insere ( no ** arvore, int dado )
{
  no * novo;
  novo = MALLOC(sizeof(no));
  novo->dado = dado;
  novo->dir = novo->esq = NULL;
  if ((novo->pai = busca(*arvore, dado)) == NULL)
    {
      *arvore = novo;
      return;
    }
  if (novo->dado < novo->pai->dado)
    novo->pai->esq = novo;
  else
    novo->pai->dir = novo;
  afunila (novo, arvore);  
}

void retira (no ** arvore, int dado)
{
  no * chave = busca (*arvore, dado), *pai;
  int tmp;  
  if (chave == NULL)
    return;
  if (chave->dado != dado)
    {
      afunila(chave, arvore);
      return;
    }
  pai = chave->pai;
  if (chave->dir == NULL && chave->esq == NULL)
    {
      if (pai != NULL)
	{
	  if(pai->dir == chave)
	    pai->dir = NULL;
	  else
	    pai->esq = NULL;
	  afunila(pai, arvore);	  
	}
      else
	*arvore = NULL;
      FREE(chave);
    }
  else if (chave->esq == NULL)
    {
      if (pai != NULL)
	{
	  if (pai->dir == chave)
	    pai->dir = chave->dir;
	  else
	    pai->esq = chave->dir;
	  afunila(pai,arvore);	  
	}
      else
	*arvore = chave->dir;
      FREE(chave);      
    }
  else if (chave->dir == NULL)
    {
      if (pai != NULL)
	{
	  if (pai->dir == chave)
	    pai->dir = chave->esq;
	  else
	    pai->esq = chave->esq;
	  afunila(pai, arvore);	  
	}
      else
	*arvore = chave->esq;
      FREE(chave);      
    }
  else
    {
      pai = busca (chave->esq, dado);
      tmp = pai->dado;
      retira (arvore, tmp);
      chave->dado = tmp;
    }
}

int conta (no * arvore)
{
  if (arvore == NULL)
    return 0;
  return conta(arvore->dir) + conta(arvore->esq) + 1;
}

int raiz (no * arvore)
{
  if (arvore == NULL)
    return 0;
  return arvore->dado;
}

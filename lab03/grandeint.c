#include "grandeint.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

struct grandeintS{
  lista digitos;
  char sinal;
};

grandeint iniciagi(void)
{
  grandeint n = (grandeint) malloc(sizeof(struct grandeintS));
  if (NULL == n)
    return n;
  if (NULL == (n->digitos = inicial()))
    {
      free(n);
      return NULL;
    }
  n->sinal = 1;
  return n;
}

grandeint itogi(int init)
{
  grandeint n = iniciagi();  
    if (n == NULL)
      return NULL;
  if (init < 0)
    {
      init *= -1;
      n->sinal = -1;
    }
  while (init)
    {
      if (insereDir(n->digitos, init%10))
	{
	  libera(n->digitos);
	  free(n);
	  return NULL;
	}
      init = init/10;
    }
  return n;
}

grandeint atogi(char * string)
{
  grandeint n = iniciagi();
  if (n == NULL)
    return NULL;
  switch (*string)
    {
    case '+':
      string++;
      break;
    case '-':
      n->sinal = -1;
      string++;
      break;
    default:
      break;
    }
  while (*string == '0')
    string++;
  while (*string != '\0')
    {      
      if (insereEsq(n->digitos, *string - '0'))
	{
	  libera(n->digitos);
	  free(n);
	  return NULL;
	}
      string++;
    }
  return n;
}


void printgi(grandeint n)
{
  lista temp;
  if (n == NULL)
    return;
  temp = andaDir(n->digitos);
  if (n->sinal == -1)
    putchar('-');
  if (temp == n->digitos)
    putchar('0');
  while(temp != n->digitos)
    {
      putchar(pega(temp) + '0');
      temp = andaDir(temp);
    }
}

/* funciona só pra positivos */
grandeint mais(grandeint gi1, grandeint gi2)
{
  grandeint resultado = iniciagi();
  lista anda1 = andaEsq(gi1->digitos), anda2 = andaEsq(gi2->digitos);
  char parcial = 0;
  while (1)
    {
      if (anda1 != gi1->digitos)
	{
	  parcial += pega(anda1) * gi1->sinal;
	  anda1 = andaEsq(anda1);
	}
      if (anda2 != gi2->digitos)
	{
	  parcial += pega(anda2) * gi2->sinal;
	  anda2 = andaEsq(anda2);
	}
      if (!( parcial || anda1 != gi1->digitos || anda2 != gi2->digitos))
	break;
      insereDir(resultado->digitos, parcial%10);
      parcial /= 10;
    }
  resultado->sinal = pega(andaDir(resultado->digitos)) > 0 ? 1 : -1;
  parcial = 0;
  anda1 = andaEsq(resultado->digitos);
  /* começa loop do mal */
  while (anda1 != resultado->digitos)
    {
      while (pega(anda1) * resultado->sinal < 0)
	{	  
	  atribui(anda1,pega(anda1) + 10 * resultado->sinal);
	  atribui(andaEsq(anda1),pega(andaEsq(anda1)) - resultado->sinal);
	}
      atribui(anda1,abs(pega(anda1)));
      anda1 = andaEsq(anda1);
    }
  /* termina loop do mal */
  while (pega(andaDir(resultado->digitos)) == 0)
    deleta(andaDir(resultado->digitos));
  return resultado;
}

void liberagi(grandeint gi)
{
  libera(gi->digitos);
  free(gi);
};


grandeint vezes(grandeint gi1, grandeint gi2)
{
  grandeint resultado = iniciagi(), fator[10], temp;
  lista anda;
  int i;
  if (andaDir(gi1->digitos) == gi1->digitos || gi2->digitos == andaDir(gi2->digitos))
    return resultado;  
  fator[0] = iniciagi();
  for (i = 1; i < 10; i++)
    fator[i] = mais(fator[i-1],gi1);
  for (anda = andaDir(gi2->digitos); anda != gi2->digitos; anda = andaDir(anda))
    {
      insereEsq(resultado->digitos,0);
      temp = mais(resultado,fator[(int) pega(anda)]);
      liberagi(resultado);
      resultado = temp;
    }
  for (i = 0; i < 10; i++)
    liberagi(fator[i]);
  resultado->sinal = gi1->sinal * gi2->sinal;  
  return resultado;
}

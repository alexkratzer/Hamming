//send.h
/* es werden über die console mehrere zeichen eingegeben.
  jeweils 7 zeichen werden als binärcode in eine textdatei geschrieben. 
  dabei wird für jede zeile und jede spalte ein prüfbit berechnet
  um das ergebnis pro spalte und zeile als gerade auszugeben. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define inputnumber 500  /*die anzahl der möglichen einzugebenden zeichen*/

in_zeichen_out_intarray(char zeichen, int zab[7], int binaer_array[8][8], int i2);
in_binaerarray_out_Paritibit_line(int binaer_array[8][8], int i2);
in_binaerarray_out_Paritibit_column(int binaer_array[8][8], int i2);         
       

void send() 
{
  FILE *schreiben;
  char zeichen;
  char zeichen_array[7];
  char eingabe_array[inputnumber];
  int zab[7];
  int binaer_array[8][8];
  int i, i2, t, e=0;
  int laenge_eingabe;

  system("cls");  // die console wird geleert
  
  /* der eingegebene Text wird in eingabe_array gespeichert
  (darf nicht länger als "#define inputnumber" sein).
  die anzahl der eingegebenen zeichen wird unter "laenge_eingabe" gespeichert */
  printf("zeichen eingeben: \n");
  gets(eingabe_array);    
  laenge_eingabe = strlen(eingabe_array);

  /* damit nicht beschriebene zeichen am ende keinen zufallswert enthalten
  werden sie mit 0 vorbelegt (offset ist anzahl der eingegebenen zeichen
  und danach 7 zeichen) */
    for(t=0;t<7;t++)
    { 
      eingabe_array[laenge_eingabe + t] = 0; 
    }

  /*solange im eingabe_array noch zeichen vorhanden sind werden diese jeweils 
  auf 7 zeichen aufgeteilt und in den zeichen_array geschrieben. danach
  wird mit den 7 zeichen weitergearbeitet bis wieder von vorne begonnen wird */
 	while (eingabe_array[e] != '\0')
  {     
    for(t=0;t<7;t++)
    {
      zeichen_array[t] = eingabe_array[e+t];
    }

  /* e wird nach dem ersten durchlauf auf 7 gesetzt. somit wird .txt 
  beim ersten aufruf neu erstellt und danach jeweils ergänzt */
    if(e == 0)    
      schreiben = fopen("telegramm.txt", "w");
    else
      schreiben = fopen("telegramm.txt", "a");
          
    if (schreiben == NULL)
      printf("Fehler beim Schreiben der Datei !");
    else
    {
      fprintf(schreiben,"\n");  
  /* zeilenumbruch damit die nächsten 7 telegramme 
  sauber anschließen. ist für empfangen wichtig....*/
      
    	for(i2=0;i2<7;i2++)  //schleife für 7 telegramme
    	{
        zeichen = zeichen_array[i2];
          
  /*in_zeichen_out_intarray ändert das zeichen in binärcode um */    
        in_zeichen_out_intarray(zeichen, zab, binaer_array, i2);
        
  /*linien Paritibit wird berechnet*/         
        in_binaerarray_out_Paritibit_line(binaer_array, i2);

  /*alle 7 Telegramme (+ linenprüfbit an 8ter stelle) in die textdatei schrieben*/
        for(i=0;i<8;i++)
        {
          fprintf(schreiben," %i",binaer_array[i][i2]);
        }     
  /*zeilenumbruch nach jedem telegramm --> nicht notwendig aber übersichtlicher*/
        //fprintf(schreiben,"\n");
      }
       
  /*Spaltenprüfbit wird berechnet*/ 
      in_binaerarray_out_Paritibit_column(binaer_array, i2);
     
  /*columnnprüfbit wird als 8tes telegramm in die .txt geschrieben*/
      for(i=0;i<8;i++)
      {     
        fprintf(schreiben," %i",binaer_array[i][7]);  
      }   
      fclose(schreiben);
    }
    e = e + 7;
  }
  return;
}


in_zeichen_out_intarray(char zeichen, int zab[7], int binaer_array[8][8], int i2)
{
  /* bei dieser Funktion wird mit dem dezimal wert eines char gearbeitet.
  z.b kleines a hat den dezimalen wert 97 
  dieser dezimale wert wird in den binärcode umgewandelt 
  97 mod 2 = 48 R 1
  48 mod 2 = 24 R 0
  24 mod 2 = 12 R 0
  12 mod 2 = 6 R 0
  6 mod 2 = 3 R 0
  3 mod 2 = 1 R 1
  1 mod 2 = 0 R 1       
  somit wird für a  1 1 0 0 0 0 1 an die mainfunktion zurückgegeben */
  int i;
  
  for (i=6; i>=0; i--)
  {
    zab[i] = zeichen%2;
    zeichen = zeichen/2; 
  }
  
  for(i=0; i<7; i++)
  {
    binaer_array[i][i2] = zab[i];
  }
  return;
}


  /* "für Parität even muss bei gerader anzahl von 1 im telegramm das pariti 0 
  sein und bei ungerader anzahl von 1 im telgramm das pariti 1 sein"  
  linienprüfbit wird berechnet: es wird jedes 1-bit einer zeile addiert
  danach wird das ergebnis durch 2 diffidert und an letzte stelle der zeile
  gespeichert. wenn z.b. 3xbits auf 1 in einer zeile stehen --> 
  3 modulo 2 = irgendwas mit Rest 1 somit wäre das paritibit 1.
  wenn z.b. 4xbits auf 1 in einer zeile stehen --> 
  4 modulo 2 = irgendwas mit Rest 0 somit wäre das paritibit 0. */
in_binaerarray_out_Paritibit_line(int binaer_array[8][8], int i2)
{
  int i, paritibit_line = 0;
  
  for(i=0;i<7;i++)
  {
    paritibit_line = paritibit_line + binaer_array[i][i2];    
  }
  binaer_array[7][i2] = (paritibit_line%2);
  return;
} 


  /* "für Parität even muss bei gerader anzahl von 1 im telegramm das pariti 0
  sein und bei ungerader anzahl von 1 im telgramm das pariti 1 sein" 
  jede Spalte wird komplett berechnet bis zur nächsten gewechselt wird.
  verfahren ist gleich wie für linienprüfbit */
in_binaerarray_out_Paritibit_column(int binaer_array[8][8], int i2)
{   
  int i, paritibit_column; 
         
  for(i=0;i<8;i++)
    { 
    paritibit_column = 0;
      for(i2=0;i2<7;i2++)
      {
        paritibit_column = paritibit_column + binaer_array[i][i2];      
      }
    binaer_array[i][7] = (paritibit_column % 2); 
  }
  return;
}
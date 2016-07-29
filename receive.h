//receive.h
#include <stdio.h>
#include <stdlib.h>

in_line_out_binaer_array(int binaer_array[8][8], FILE *lesen);
int checknobool(int binaer_array[8][8]);
check_receive_line_err(int *fehlerZeile, int binaer_array[8][8], int *Fehlerzaehler);
check_receive_column_err(int *fehlerSpalte, int binaer_array[8][8], int *Fehlerzaehler);
if_err_correct(int *fehlerSpalte, int *fehlerZeile, int *Korrektur, int *Fehlerzaehler, int binaer_array[8][8]);
in_binaerarray_out_console(int binaer_array[8][8]);  

void receive()
{
  FILE *lesen;
  int binaer_array[8][8];
  int ispalte, izeile, i;
  int fehlerZeile, fehlerSpalte;  
  int Fehlerzaehler = 0;
  int Korrektur = 0;
  int tmp;

  system("cls");
    /* telegramm.txt wird geöffnet. falls txt nicht vorhanden ist bzw öffnen nicht 
    funktioniert wird printf ausgegeben*/
  lesen = fopen("telegramm.txt", "r");
  if (lesen == NULL)
  {
    printf("Fehler beim Lesen der Datei !\n");
    printf("-> telegramm.txt muss im gleichen Ordner wie menue.c vorhanden sein");
  }
  else
  {
    while(feof(lesen) == 0)       /*binärcode wird komplett aus .txt ausgelesen*/
    {
  
    /*jeweils 7 zeichen aus .txt wird in binaer_array gepackt*/
      in_line_out_binaer_array(binaer_array, lesen);
          
    /*überprüfung ob eine Zahl ungleich 0 oder 1 in telegramm.txt vorhanden ist*/  
      tmp = checknobool(binaer_array);
      if(tmp == 1)
      goto kein_bool;
           
    /*überprüfung ob sich in der zeile ein bit gedreht hat*/       
      check_receive_line_err(&fehlerZeile, binaer_array, &Fehlerzaehler);  
              
    /*überprüfung ob sich in der spalte ein bit gedreht hat*/
      check_receive_column_err(&fehlerSpalte, binaer_array, &Fehlerzaehler); 
      
    /*falls ein fehler aufgetreten ist wird er hier wieder korrigiert*/   
      if_err_correct(&fehlerSpalte, &fehlerZeile, &Korrektur, &Fehlerzaehler, binaer_array);
                   
    /*der binärarray wird in zeichen umgewandelt und an der console ausgegeben*/  
      in_binaerarray_out_console(binaer_array);  
    }         	           
  }         
  fclose(lesen);   

  /*hier wird ausgegeben ob ein Fehler aufgetreten ist und ob er korrigiert wurde*/
  if(Korrektur == 1)  
    printf("\n\n\nes ist %i Fehler aufgetreten der korrigiert wurde", Korrektur);
  if(Korrektur > 1)  
    printf("\n\n\nes sind %i Fehler aufgetreten die korrigiert wurden", Korrektur);    
        
  if(Fehlerzaehler > 0)
    printf("\n\nes sind maximal %i Fehler aufgetreten die nicht korrigiert wurden\n",Fehlerzaehler); 
          
  printf("\n");    
  system("pause");
  return;
  
  /*wenn ein zeichen ungleich 0 oder 1 aus der .txt gelesen wurde wird hierhin gesprungen*/
  kein_bool:    
    fclose(lesen); 
    printf("\nZeichen ungleich 0 oder 1 werden nicht akzeptiert\n");  
  system("pause");      
  return; 
}


  /* aus der textdatei wird ein block von 8 x 8 binärzeichen eingelesen.
  d.h. 7 zeichen + jeweils 1 paritibit für die zeile und ein paritibit für die 
  spalte mit diesen binärzeichen wird der binaer_array gefüllt*/
in_line_out_binaer_array(int binaer_array[8][8], FILE *lesen)
{  
  int ispalte;
  int z, i;
    
  for(ispalte = 0; ispalte < 8; ispalte++)
  {
    for(i=0; i<8; i++)
    {
      fscanf (lesen, "%i", &z);   
      binaer_array[ispalte][i] = z;  
    }
  }      
  return;
}


  /*wenn Zahl != 0 oder 1 ausgelesen, wird Fehlercode 1 an main zurückgegeben*/
int checknobool(int binaer_array[8][8])      
{
  int x, y;
  
  for(x = 0; x < 8; x++)
  {
    for(y = 0; y < 8; y++)
    {
      if((binaer_array[x][y] != 0) && (binaer_array[x][y] != 1))
      return 1;
    }      
  }
  return 0;
}


  /* überprüfung ob sich in der zeile ein bit gedreht hat. es müssen alle 1er 
  zusammen eine gerade anzahl ergeben */
check_receive_line_err(int *fehlerZeile, int binaer_array[8][8], int *Fehlerzaehler)
{ 
  int ispalte, pariti_zeile, i;
  
  *fehlerZeile = 10; 
  for(ispalte = 0; ispalte < 8; ispalte++)
  {
    pariti_zeile = 0;
    for(i = 0; i < 8; i++)
    {
      pariti_zeile += binaer_array[ispalte][i];
    }
    if(pariti_zeile%2 == 1)   /* überprüfung ob 1er ungerade anzahl haben*/
    { /*fehlerhafte zeile wird für eine evtl korrektur gespeichert*/
      *fehlerZeile = ispalte;    
      (*Fehlerzaehler)++;  /* da zeile fehlerhaft wird fehlerzähler erhöht*/
    }
  } 
  return;
}


  /* überprüfung ob sich in der Spalte ein bit gedreht hat.
  gleiches prinzip wie bei Zeile */
check_receive_column_err(int *fehlerSpalte, int binaer_array[8][8], int *Fehlerzaehler)
{
  int izeile, pariti_spalte, i;
  
  *fehlerSpalte = 10;
  for(izeile = 0; izeile < 8; izeile++)
  {
    pariti_spalte = 0;   
    for(i = 0; i < 8; i++)
    {
      pariti_spalte = pariti_spalte + binaer_array[i][izeile];
    }
    if(pariti_spalte%2 == 1)  
    { 
      *fehlerSpalte = izeile;
      (*Fehlerzaehler)++;
    }
  }
  return;
}


  /* falls sich ein bit gedreht hat wird es hier wieder korrigiert.
  wenn fehlerSpalte und fehlerZeile nicht mehr 10 ist wird der korrekturzähler
  um 1 erhöhrt und der Fehlerzaehler um 2 verringert
  falls nur in einer Zeile oder Spalte ein Fehler festgestellt wurde, wird 
  die Korrektur nicht durchgeführt da das Fehlerhafte Bit nicht lokalisiert 
  werden kann. in diesem Fall wird der Fehlerzaehler nicht verringert */ 
if_err_correct(int *fehlerSpalte, int *fehlerZeile, int *Korrektur, int *Fehlerzaehler, int binaer_array[8][8])
{
  if((*fehlerSpalte != 10) && (*fehlerZeile != 10))
  {
    (*Korrektur) ++;
    (*Fehlerzaehler) -= 2;
    if(binaer_array[*fehlerZeile][*fehlerSpalte] == 0)
    {
      binaer_array[*fehlerZeile][*fehlerSpalte] = 1;
    }
    else
    {
      binaer_array[*fehlerZeile][*fehlerSpalte] = 0;
    }
  }
  return;
}


  /* der binaer_array wird zeilenweise wieder in char umgewandelt jeweils eines 
  der 7 zeichen wird als integer behandelt und je nach wertigkeit der 
  entsprechenden stelle bei ausgelesener 1 der wert addiert. z.b. bei eingegebenen
  zeichen a würde 1 1 0 0 0 0 1 [1] aus der .txt ausgelesen werden. die [] stelle 
  wird (da nur prüfbit) ignoriert also wird für  
  1 an 6ter stelle -> 1 addiert
  folgend immer 0 addiert
  1 an 1ter stelle -> 32 addiert
  1 an 0ter stelle -> 64 addiert
  ergiebt 97 --> nach ascii tabelle ==> kleines a */
in_binaerarray_out_console(int binaer_array[8][8])
{
  int ispalte;
  int zeichen;
   
  for(ispalte = 0; ispalte < 7; ispalte++)
  {
    zeichen = 0;

    if(binaer_array[ispalte][6] == 1)
    zeichen = zeichen + 1;
    if(binaer_array[ispalte][5] == 1)
    zeichen = zeichen + 2;
    if(binaer_array[ispalte][4] == 1)
    zeichen = zeichen + 4;
    if(binaer_array[ispalte][3] == 1)
    zeichen = zeichen + 8;
    if(binaer_array[ispalte][2] == 1)
    zeichen = zeichen + 16;
    if(binaer_array[ispalte][1] == 1)
    zeichen = zeichen + 32;
    if(binaer_array[ispalte][0] == 1)
    zeichen = zeichen + 64;

    printf("%c", zeichen); // der string wird an der console ausgegeben
  }  
  return;
}
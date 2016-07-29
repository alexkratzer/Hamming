//menue.c
/*menü für send.h und receive.h */
#include <stdio.h>
#include "send.h"
#include "receive.h"

void main()
{
char auswahl = 'x';
int status;

  do
  {
    while((auswahl != 's') && (auswahl != 'e'))
    {
    /* Auswahldialog bei erstem Durchlauf
       bei Eingabe unterschiedlich zu s oder e wird das Programm nicht vortgesetzt
       und der Dialog erneut angezeigt mit erneutem Einlesen der Eingabe */
      
      printf("\n[s]enden oder [e]mpfangen? \n");
      printf("Eingabe mit Enter bestaetigen\n");
      printf("\nEingabe: "); 
      scanf("%c", &auswahl);
      fflush(stdin);    // notwendig damit bei Eingabe unglich s oder e der Dialog nicht 2x angezeigt wird
    }

    if(auswahl == 's')
      send();   // aufruf der "send" Bibliothek
      
    if(auswahl == 'e')
      receive();   // aufruf der "receive" Bibliothek
    
    system("cls");  

    /* Auswahldialog bei allen nachfolgenden Durchläufen bei Eingabe unterschiedlich 
       zu s oder e wird das Programm beendet */  
    printf("\n[s]enden oder [e]mpfangen? \n");
    printf("Eingabe mit Enter bestaetigen\n");
    printf("(zum Beenden --> andere oder keine Eingabe)\n");
    printf("\nEingabe: ");
    scanf("%c", &auswahl);    
  }while((auswahl == 's') || (auswahl == 'e')) ;

return;
}
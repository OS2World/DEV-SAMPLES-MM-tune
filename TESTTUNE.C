/*************************************************************************
 * Testtune.c  Source to demonstrate how to play the tune files created  *
 * with the OS/2 PMTUNE program.  The tune files are described by the    *
 * note below from IBM.  Each element of the tune is delimited by a pipe *
 * symbol "|" .                                                          *
 *                                                                       *
 * Provided by Bryan Walker, WalkerWerks with the permission of IBM.     *
 *                                                                       *
 * WARNING: This is a simple test file so it does not do error trapping  *
 *          be sure and enter a Tempo on the command line.               *
 *************************************************************************

 There are two ways of enabling other programs to "play" the alarm tunes,
 either by "printing" a Tune out (appears in DosBeep 'c' code format) and
 building the code into a program or by coding up a special "play" tune
 routine to operate on the tune file(s) directly.

 Tune file consists of the following fields - repeated 36 times:-

  1) Tune Title.
  2) Tune Tempo.
  3) Tune Notes ( 20 pairs of numbers representing Note Type and Pitch ).
                 Note Types less than 10 are real notes. Note Types 10 or
                 more are rests.

 File read into a series of arrays:-
      Titles,
      Tempo   = SHORT  g_sTuneTempo[ 36 ]
      Notes   = SHORT  g_sTuneNote[ 36 ][ 20 ]
      Pitch   = SHORT  g_sTunePitch[ 36 ][ 20 ]

 Hope this is what you wanted,

 Jeff Kerr
*/


#define INCL_DOS
#include <os2.h>
main(int argc, char *argv[], char *envp[])
{

/* Default Tune */

SHORT  g_sTunePitch[ 20 ] = {18,17,16,15,16,15,14,13,14,13,12,11,12,13,12,11,6,6,6,6} ;
SHORT  g_sTuneNote[ 20 ] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,2,10,10,10,10} ;

/*Blue Danube*/
/*
SHORT  g_sTunePitch[ 20 ] = {17,17,57,13,13,6,6,6,6,50,50,6,6,6,6,6,6,6,6,6} ;
SHORT  g_sTuneNote[ 20 ] = {4,4,4,4,4,12,4,4,12,4,4,10,10,10,10,10,10,10,10,10} ;
*/
SHORT  g_sTuneTempo ;
static  SHORT    ls_sDuration[ 16 ] =
         {
           1024, 768, 512, 384, 256, 192, 128, 96, 64, 32,
           1024, 512, 256, 128, 64,  32
         };

static  SHORT    ls_sFrequency[ 63 ] =
         {
           784, 698, 660, 588, 523, 494, 440, 392, 349, 330,
           294, 262, 247, 220, 196, 175, 165, 147, 131, 123,
           110, 740, 660, 622, 554, 494, 466, 415, 370, 330,
           311, 277, 247, 233, 208, 185, 165, 156, 139, 123,
           117, 104, 830, 740, 698, 622, 554, 523, 466, 415,
           370, 349, 311, 277, 262, 233, 208, 185, 175, 156,
           139, 131, 117
         };

SHORT      l_sI;
SHORT      l_sJ;
double tempo_multiplier = 1;

g_sTuneTempo = atoi(argv[1]) ;

if(g_sTuneTempo >= 300)
    g_sTuneTempo = 299 ;
else if (g_sTuneTempo < 1)
    g_sTuneTempo = 1 ;

/**********************************************************************
 * ls_sDuration above gives the duration for a tempo of 200.  Also    *
 * The Tune Editor assumes each move of 100 is                        *
 * an equivelant 100% change in the tempo (i.e. 100 is twice as slow  *
 * as 200 and 300 is twice as fast).  To calculate this I altered the *
 * original Tempo formula provided by IBM from the duration of the    *
 * selected note times the tempo divided by 100 to the below formula  *
 * that starts with a baseline 200 assumption.                        *
 * Bryan Walker WalkerWerks                                           *
 **********************************************************************/

if(g_sTuneTempo < 200)
   tempo_multiplier += (((200.00 - (double)g_sTuneTempo) / 100.00));
else if (g_sTuneTempo > 200)
   tempo_multiplier -= ((((double)g_sTuneTempo - 200.00)/100.00));


/************************************************************************/
/* Search backwards along tune looking for last none "rest" type note.  */
/************************************************************************/
for ( l_sI=19; l_sI>0; l_sI-- )
   {
   if ( g_sTuneNote[ l_sI ] < 10 )
     break;
   }

/************************************************************************/
/* Loop to "play" tune.                                                 */
/************************************************************************/
for ( l_sJ=0; l_sJ<= l_sI; l_sJ++ )
   {
     /**********************************************************************/
     /* if note type is greater than 9 (ie: it's a rest) then use duration */
     /* with a very high frequency - results in silence.                   */
     /* Otherwise play note as normal.                                     */
     /**********************************************************************/
    if ( g_sTuneNote[ l_sJ ] > 9 )
       {
       /*
         Changed from DosBeep(0x7FFF, duration) to DosSleep because long
         pauses allowed the speaker transition from off to on to interject
         a click.
       */
       DosSleep((USHORT)((double)ls_sDuration[ g_sTuneNote[ l_sJ ] ] * tempo_multiplier));
       }
    else
       {
       DosBeep( ls_sFrequency[ g_sTunePitch[ l_sJ ] ],
          (USHORT)((double)ls_sDuration[ g_sTuneNote[ l_sJ ] ] * tempo_multiplier));
       }
    /**********************************************************************/
    /* Insert a short silence in between notes                            */
    /**********************************************************************/
    DosBeep( 0x7FFF, (USHORT) (8 * tempo_multiplier)) ;
   }
}          

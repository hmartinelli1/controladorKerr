/*
  EN EL CALCULO DEL COMPLEMENTO A 2, EN LUGAR DE SUMAR 256, QUE SERÍA 2^8, SUMO 257, ESTO NO ESTA BIEN DESDE EL PUNTO DE VISTA
  MATEMÁTICO NI CONICIDE CON LO QUE HACE EL PROGRAMA, SIN EMBARGO, EL OSCILOSCOPIO DE OTRO MODO SALTEA EL VALOR BINARIO -1
  LO CUAL DEJA UN GAP EN LAS CURVAS Y HACE QUE LOS VOLTAJES CERCANOS A 0 TENGAN UN SESGO A VALORES NEGATIVOS. SI SE GRAFICA UNA
  CURVA EXTRAIDA CON EL PROGRAMA DEL OSCILOSCOPIO, SE NOTARÁ FACILMENTE UN SALTO INMEDIATAMENTE DEBAJO DE 0, UN VALOR QUE NO EXISTE.
*/

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <cstdarg>
#include <dirent.h>
//#include <direct.h>
#include <stdlib.h>
//#include<iostream>

#define LOGNAME_FORMAT "log/%Y%m%d_%H%M%S"
#define LOGNAME_SIZE 20


int seguir=0;
int medida=0;
char fecha[32];
char hora[32];
char nombre_muestra[64];
char nombre_archivo_info_mediciones[]=".\\Datos\\";
char nombre_carpeta_info_mediciones[64];
char nombre_archivo_individual[64];


char codigo_samplerate[4];
char codigo_samplerate_corto[4];
float ancho_u_segundos;

HANDLE com_osc;
BOOL   Status_osc;
BOOL   Status;
char TempChar;
char datos_recibidos_1[10000000];
char datos_recibidos_2[10000000];
char encabezado_1[18];
char encabezado_2[18];
char encabezado_corto[16];
int primer_bit1[5000000];
int segundo_bit1[5000000];
int primer_bit2[5000000];
int segundo_bit2[5000000];
int datos_en16_1[5000000];
int datos_en16_2[5000000];
float valores_1[5000000];
float valores_2[5000000];

char datos_recibidos_1_corto[10000000];
char datos_recibidos_2_corto[10000000];
int primer_bit1_corto[5000000];
int segundo_bit1_corto[5000000];
int primer_bit2_corto[5000000];
int segundo_bit2_corto[5000000];
int datos_en16_1_corto[5000000];
int datos_en16_2_corto[5000000];
float valores_1_cortos[5000];
float valores_2_cortos[5000];
char n_puerto_osc[5];
char nombre_archivo[32];
char nombre_archivo1[32]="salida1";
char nombre_archivo2[32]="salida2";
char orden[32];
char param_osc[32];
float escalaV1;
float escalaV2;
float offset1;
float offset2;

float angulo_alfa;

int tpi=0;
int tpf=0;
int delta_t;
int t0, t1, t2, t3, t4, t5, t6, t7;
float factor=25;

float Vapl0, Vapl0cuadrado, varVapl0, desvestVapl0, errVapl0;
float Vapl, Vaplcuadrado, varVapl, desvestVapl, errVapl;
float Vapl_final;
float V0, V0cuadrado, varV0, desvestV0, errV0;
float V1, V1cuadrado, varV1, desvestV1, errV1;
float V2, V2cuadrado, varV2, desvestV2, errV2;
float V3, V3cuadrado, varV3, desvestV3, errV3;
float V4, V4cuadrado, varV4, desvestV4, errV4;
float V5, V5cuadrado, varV5, desvestV5, errV5;
float deltaV1, errdeltaV1;
float deltaV2, errdeltaV2;
float deltaV3, errdeltaV3;
float deltaV4, errdeltaV4;
float deltaV5, errdeltaV5;
float deltaV1_V, errdeltaV1_V;
float deltaV2_V, errdeltaV2_V;
float deltaV3_V, errdeltaV3_V;
float deltaV4_V, errdeltaV4_V;
float deltaV5_V, errdeltaV5_V;
float delta1, delta2, delta3, delta4, delta5;

float dist_t[512]={4.30265273,3.182446305,2.776445105,2.570581836,2.446911851,2.364624252,2.306004135,2.262157163,2.228138852,2.20098516,2.17881283,2.160368656,2.144786688,2.131449546,2.119905299,2.109815578,2.10092204,2.093024054,2.085963447,2.079613845,2.073873068,2.06865761,2.063898562,2.059538553,2.055529439,2.051830516,2.048407142,2.045229642,2.042272456,2.039513446,2.036933343,2.034515297,2.032244509,2.030107928,2.028094001,2.026192463,2.024394164,2.02269092,2.02107539,2.01954097,2.018081703,2.016692199,2.015367574,2.014103389,2.012895599,2.011740514,2.010634758,2.009575237,2.008559112,2.00758377,2.006646805,2.005745995,2.004879288,2.004044783,2.003240719,2.002465459,2.001717484,2.000995378,2.000297822,1.999623585,1.998971517,1.998340543,1.997729654,1.997137908,1.996564419,1.996008354,1.995468931,1.994945415,1.994437112,1.993943368,1.993463567,1.992997126,1.992543495,1.992102154,1.99167261,1.991254395,1.990847069,1.99045021,1.990063421,1.989686323,1.989318557,1.98895978,1.988609667,1.988267907,1.987934206,1.987608282,1.987289865,1.9869787,1.986674541,1.986377154,1.986086317,1.985801814,1.985523442,1.985251004,1.984984312,1.984723186,1.984467455,1.984216952,1.983971519,1.983731003,1.983495259,1.983264145,1.983037526,1.982815274,1.982597262,1.98238337,1.982173483,1.98196749,1.981765282,1.981566757,1.981371815,1.981180359,1.980992298,1.980807541,1.980626002,1.980447599,1.980272249,1.980099876,1.979930405,1.979763763,1.979599878,1.979438685,1.979280117,1.979124109,1.978970602,1.978819535,1.97867085,1.978524491,1.978380405,1.978238539,1.978098842,1.977961264,1.977825758,1.977692277,1.977560777,1.977431212,1.977303542,1.977177724,1.97705372,1.976931489,1.976810994,1.976692198,1.976575066,1.976459563,1.976345655,1.976233309,1.976122494,1.976013178,1.975905331,1.975798924,1.975693928,1.975590315,1.975488058,1.975387131,1.975287508,1.975189163,1.975092073,1.974996213,1.97490156,1.974808092,1.974715786,1.974624621,1.974534576,1.97444563,1.974357764,1.974270957,1.974185191,1.974100447,1.974016708,1.973933954,1.973852169,1.973771337,1.97369144,1.973612462,1.973534388,1.973457202,1.973380889,1.973305434,1.973230823,1.973157042,1.973084077,1.973011915,1.972940542,1.972869946,1.972800114,1.972731033,1.972662692,1.972595079,1.972528182,1.97246199,1.972396491,1.972331676,1.972267533,1.972204051,1.972141222,1.972079034,1.972017478,1.971956544,1.971896224,1.971836507,1.971777385,1.971718848,1.971660889,1.971603499,1.971546669,1.971490392,1.971434659,1.971379462,1.971324793,1.971270646,1.971217013,1.971163885,1.971111258,1.971059122,1.971007472,1.970956301,1.970905601,1.970855367,1.970805592,1.97075627,1.970707395,1.970658961,1.970610961,1.97056339,1.970516243,1.970469513,1.970423195,1.970377283,1.970331773,1.970286659,1.970241936,1.970197599,1.970153643,1.970110062,1.970066853,1.97002401,1.96998153,1.969939406,1.969897635,1.969856213,1.969815134,1.969774395,1.969733992,1.969693921,1.969654176,1.969614755,1.969575654,1.969536868,1.969498393,1.969460227,1.969422365,1.969384804,1.96934754,1.96931057,1.96927389,1.969237496,1.969201386,1.969165556,1.969130003,1.969094724,1.969059715,1.969024974,1.968990497,1.968956281,1.968922324,1.968888622,1.968855173,1.968821974,1.968789022,1.968756314,1.968723847,1.96869162,1.968659628,1.968627871,1.968596344,1.968565046,1.968533975,1.968503127,1.9684725,1.968442092,1.968411901,1.968381923,1.968352158,1.968322603,1.968293255,1.968264113,1.968235174,1.968206436,1.968177896,1.968149554,1.968121407,1.968093453,1.968065689,1.968038115,1.968010728,1.967983525,1.967956506,1.967929669,1.967903011,1.967876531,1.967850227,1.967824098,1.967798141,1.967772355,1.967746738,1.967721288,1.967696005,1.967670885,1.967645929,1.967621133,1.967596497,1.967572019,1.967547698,1.967523532,1.967499519,1.967475658,1.967451948,1.967428387,1.967404974,1.967381707,1.967358585,1.967335607,1.967312772,1.967290077,1.967267522,1.967245106,1.967222827,1.967200683,1.967178675,1.9671568,1.967135057,1.967113445,1.967091963,1.96707061,1.967049384,1.967028285,1.967007311,1.966986461,1.966965734,1.966945129,1.966924645,1.966904281,1.966884036,1.966863909,1.966843898,1.966824003,1.966804223,1.966784557,1.966765003,1.966745561,1.96672623,1.966707009,1.966687896,1.966668892,1.966649995,1.966631204,1.966612519,1.966593938,1.96657546,1.966557085,1.966538813,1.966520641,1.966502569,1.966484596,1.966466722,1.966448946,1.966431267,1.966413684,1.966396196,1.966378803,1.966361504,1.966344297,1.966327183,1.966310161,1.966293229,1.966276388,1.966259636,1.966242972,1.966226397,1.966209908,1.966193507,1.966177191,1.966160961,1.966144815,1.966128753,1.966112774,1.966096878,1.966081064,1.966065331,1.966049679,1.966034107,1.966018615,1.966003201,1.965987866,1.965972608,1.965957428,1.965942324,1.965927296,1.965912343,1.965897465,1.965882662,1.965867932,1.965853275,1.965838691,1.965824178,1.965809738,1.965795368,1.965781069,1.96576684,1.96575268,1.965738589,1.965724567,1.965710612,1.965696725,1.965682905,1.965669151,1.965655464,1.965641842,1.965628284,1.965614792,1.965601364,1.965587999,1.965574698,1.965561459,1.965548282,1.965535168,1.965522115,1.965509123,1.965496192,1.96548332,1.965470509,1.965457757,1.965445064,1.965432429,1.965419852,1.965407333,1.965394872,1.965382467,1.965370119,1.965357827,1.965345591,1.96533341,1.965321285,1.965309213,1.965297197,1.965285234,1.965273324,1.965261468,1.965249665,1.965237914,1.965226215,1.965214568,1.965202973,1.965191428,1.965179935,1.965168491,1.965157098,1.965145755,1.965134461,1.965123216,1.96511202,1.965100873,1.965089774,1.965078722,1.965067718,1.965056762,1.965045852,1.965034989,1.965024172,1.965013401,1.965002676,1.964991997,1.964981363,1.964970773,1.964960229,1.964949728,1.964939272,1.964928859,1.96491849,1.964908164,1.964897881,1.964887641,1.964877443,1.964867287,1.964857173,1.964847101,1.96483707,1.96482708,1.964817132,1.964807223,1.964797356,1.964787528,1.96477774,1.964767992,1.964758283,1.964748614,1.964738983,1.964729391,1.964719837,1.964710322,1.964700845,1.964691405,1.964682003,1.964672639,1.964663311,1.964654021,1.964644767,1.964635549,1.964626368,1.964617222,1.964608113
};
//distribucion t para alfa=0.05 arrancando en n=3,
//o sea si tengo 3 muestras tengo que usar el valor dist_t[0], si tengo 4 muestras el valor dist_t[1] etc




float IEEE754()
{  // for (int k=0; k<4; k++) {printf("en el codigo de samplerate hay: %i %i",k,codigo_samplerate[k]);}
    int signo;
    signo=pow(-1,codigo_samplerate[0]/128);
//    printf("\n signo: %i",signo);
    int exponente;
    exponente=codigo_samplerate[0]*2 + codigo_samplerate[1]/128;
 //    printf("\n exponente: %i \n",exponente);
    int exponentes[23];
    for (int i=0; i<7; i++)
    { //  printf("%i ",(int)pow(2,6-i));
        exponentes[i]=((int)codigo_samplerate[1] >> 6-i) & 1;
//        printf("exponente [%i]: %i\n",i,exponentes[i]);
    }
    for (int i=7;i<15; i++)
    { //   printf("%i ",(int)pow(2,14-i));
        exponentes[i]=((int)codigo_samplerate[2] >> 14-i) &1;
 //       printf("exponente [%i]: %i\n",i,exponentes[i]);
    }
    for (int i=15; i<23; i++)
    {//   printf("%i ",(int)pow(2,22-i));
        exponentes[i]=((int) codigo_samplerate[3] >> 22-i) & 1;
 //       printf("exponente [%i]: %i\n",i ,exponentes[i]);
    }
    float mantisa;
    for(int j=0; j<23; j++)
    {
     mantisa=mantisa + (float)exponentes[j]*pow(2,-j-1);
 //    printf("mantisa %i : %f\n",j,mantisa);
    }
    float resultado= signo*(1+mantisa)*(pow(2,exponente-127));
  //  printf("%4.2e",resultado);
    return resultado;
}

float calcular_delta(float a, float b)
{ //a = angulo b= deltaI/I
  //calculo el seno cuadrado
 //   printf("\nEL ANGULO ALFA ES EN GRADOS %f\n",angulo_alfa);
  //  printf("\n el dV/V es: %f\n",b);
  float angulo_rad=3.141592654*a/180;
   // printf("\nEANGLO EN RADIANES ES: %F",angulo_rad);
  float senocuadrado=sin(angulo_rad)*sin(angulo_rad);
 //   printf("\n el seno cuadrado es: %f\n",senocuadrado);
  float raizcuadrada=sqrt(b*senocuadrado+senocuadrado);
 //   printf("\nla raiz cuadrada es: %f\n",raizcuadrada);
  float resultado_delta=2*asin(raizcuadrada);
//  printf("\el resultado es: %f\n",resultado_delta);
    return resultado_delta;

}

void preguntar_nombre()
{
    printf(" Nombre de la muestra: ");
    scanf( "%s", &nombre_muestra);
    strcat(nombre_archivo_info_mediciones,nombre_muestra);

}

void elegir_puerto_osc()
{
        printf("Puerto del osciloscopio: ");
        scanf( "%s", &n_puerto_osc); //espera un string del teclado y lo guarda como n_puerto_osc

}

void abrir_puerto_osc() {

                  com_osc = CreateFile( n_puerto_osc,                       // Name of the Port to be Opened
                                GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
                                0,                                 // No Sharing, ports cant be shared
                                NULL,                              // No Security
                                OPEN_EXISTING,                     // Open existing port only
                                0,                                 // Non Overlapped I/O
                                NULL);                             // Null for Comm Devices

                   if (com_osc == INVALID_HANDLE_VALUE)
                               {
                               printf("\n   Error! -   El puerto %s no se pudo abrir, verifique configuracion y conexiones.\n Saliendo...\n", n_puerto_osc);
                               exit (EXIT_FAILURE);
                               }

                           else
                               {
                               printf(" Puerto %s abierto\n", n_puerto_osc);
                               }


                   /*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

                   DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
                   dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

                   Status_osc = GetCommState(com_osc, &dcbSerialParams);     //retreives  the current settings

                   if (Status_osc == FALSE)
                           printf("\n   Error! in GetCommState()");

                   dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 9600
                   dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
                   dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
                   dcbSerialParams.Parity   = NOPARITY;      // Setting Parity = None

                   Status_osc = SetCommState(com_osc, &dcbSerialParams);  //Configuring the port according to settings in DCB

                   if (Status_osc == FALSE)
                           {
                                   printf("\n   Error! in Setting DCB Structure");
                           }
                   else
                           {/*
                                   printf("\n   Setting DCB Structure Successfull\n");
                                    printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
                                   printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
                                   printf("\n       StopBits = %d", dcbSerialParams.StopBits);
                                   printf("\n       Parity   = %d", dcbSerialParams.Parity);*/
                           }

                   /*------------------------------------ Setting Timeouts --------------------------------------------------*/

                   COMMTIMEOUTS timeouts = { 0 };

                   timeouts.ReadIntervalTimeout         = 50;
                   timeouts.ReadTotalTimeoutConstant    = 50;
                   timeouts.ReadTotalTimeoutMultiplier  = 10;
                   timeouts.WriteTotalTimeoutConstant   = 50;
                   timeouts.WriteTotalTimeoutMultiplier = 10;

                   if (SetCommTimeouts(com_osc, &timeouts) == FALSE)
                           printf("\n   Error! in Setting Time Outs");
                    else
                   {
                   // printf("\n\n   Setting Serial Port Timeouts Successfull");
                   }



                   Status = EscapeCommFunction(com_osc, SETRTS); //Setting the RTS line using EscapeCommFunction();

                   if (Status == FALSE)
                           printf("\n   Error! RTS not Set");
                   else
                   {
                       //    printf("\n\n   RTS Seteado\n\n ");
                   }


                   Status = EscapeCommFunction(com_osc, SETDTR); //Setting the DTR line using EscapeCommFunction();

                                   if (Status == FALSE)
                                           printf("\n   Error! DTR not Set");
                                   else
                                         {
                                           //  printf("\n   DTR Seteado,\n  ");
                                         }

                                  // _getch();



}

void leer_datos_osc_1()
{   
   
    DWORD dwEventMask;
    DWORD NoBytesRead;

    int i = 0;
                            /*------------------------------------ Setting Receive Mask ----------------------------------------------*/

                            Status_osc = SetCommMask(com_osc, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

                    //	if (Status_osc == FALSE)
                    //		printf("\n\n    Error! in Setting CommMask");
                    //	else
                    //		printf("\n\n    Setting CommMask successfull");


               /*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/

                            printf(" Leyendo canal 1 ");

                            Status_osc = WaitCommEvent(com_osc, &dwEventMask, NULL); //Wait for the character to be received

                            /*-------------------------- Program will Wait here till a Character is received ------------------------*/

                            if (Status_osc == FALSE)
                                    {
                                            printf("\n    Error! in Setting WaitCommEvent()");
                                    }
                            else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
                                    {
                                            //printf("\n\n    Characters Received");
                                            do
                                                    {
                                                            Status_osc = ReadFile(com_osc, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
                                                            datos_recibidos_1[i] = TempChar;
                                                           //  printf("%i",TempChar);
                                                            // printf("\n");
                                                            i++;
                                                            if(i%100000==0) {printf(".");}
                                                }
                                            while (NoBytesRead > 0);



                                            /*------------Printing the RXed String to Console----------------------*/



                                            int j =0;
                                            int k=0;
                                            int l=0;
                                            printf(" %i",i); printf(" bytes.");
                                          /*
                                            for (j=0; j<18; j++)
                                            {
                                                encabezado_1[j]=datos_recibidos_1[j];
                                            }
                                           */
                                            for (j=9; j<13; j++)
                                            {
                                                codigo_samplerate[j-9]=datos_recibidos_1[j];
                                            }

                                            for (j = 17; j < i-1; j++) {
                                            // fprintf(archivo, "%c", datos_recibidos_1[j] );  // guardar todos los datos
                                                    if (j % 2 != 0)
                                                    {
                                                     primer_bit1[k]=datos_recibidos_1[j];
                                                    }
                                                    else
                                                    {
                                                     segundo_bit1[k]=datos_recibidos_1[j];
                                                     if (segundo_bit1[k] < 0){
                                                         segundo_bit1[k]=segundo_bit1[k]+257;
                                                     }
                                                     datos_en16_1[k]=primer_bit1[k]*256+segundo_bit1[k];
                                                     k++;
                                                    }

                                            }

                                                                                        printf(" Cantidad de puntos: %i\n",k);
											
										


                                    }


}

void leer_datos_osc_2()
{
    DWORD dwEventMask;
    DWORD NoBytesRead;

    int i = 0;
                            /*------------------------------------ Setting Receive Mask ----------------------------------------------*/

                            Status_osc = SetCommMask(com_osc, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

                    //	if (Status_osc == FALSE)
                    //		printf("\n\n    Error! in Setting CommMask");
                    //	else
                    //		printf("\n\n    Setting CommMask successfull");


               /*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/

                            printf(" Leyendo canal 2 ");

                            Status_osc = WaitCommEvent(com_osc, &dwEventMask, NULL); //Wait for the character to be received

                            /*-------------------------- Program will Wait here till a Character is received ------------------------*/

                            if (Status_osc == FALSE)
                                    {
                                            printf("\n    Error! in Setting WaitCommEvent()");
                                    }
                            else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
                                    {
                                            //printf("\n\n    Characters Received");
                                            do
                                                    {
                                                            Status_osc = ReadFile(com_osc, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
                                                            datos_recibidos_2[i] = TempChar;
                                                            i++;
                                                            if(i%100000==0) {printf(".");}
                                                }
                                            while (NoBytesRead > 0);



                                            /*------------Printing the RXed String to Console----------------------*/



                                            int j =0;
                                            int k=0;
                                            int l=0;
                                            printf(" %i",i); printf(" bytes.");

                                            for (j=0; j<18; j++)
                                            {
                                                encabezado_2[j]=datos_recibidos_2[j];
                                            }


                                            
                                            for (j = 17; j < i-1; j++)
                                            {
                                          //  fprintf(archivo, "%c", datos_recibidos_2[j] );  // guardar todos los datos
                                           if (j % 2 != 0)
                                               {
                                               primer_bit2[k]=datos_recibidos_2[j];

                                                }
                                                 else
                                                 {
                                                 segundo_bit2[k]=datos_recibidos_2[j];

                                                    if (segundo_bit2[k] < 0){

                                                        segundo_bit2[k]=segundo_bit2[k]+257;
                                                    }


                                                 datos_en16_2[k]=primer_bit2[k]*256+segundo_bit2[k];

                                                 k++;
                                                 }
																																									
                                            }
                                                printf(" Cantidad de puntos: %i\n",k);
											
                                                                                                        }


}

void leer_osc_corto_1()
{

    DWORD dwEventMask;
    DWORD NoBytesRead;

    int i = 0;
                            /*------------------------------------ Setting Receive Mask ----------------------------------------------*/

                            Status_osc = SetCommMask(com_osc, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

                    //	if (Status_osc == FALSE)
                    //		printf("\n\n    Error! in Setting CommMask");
                    //	else
                    //		printf("\n\n    Setting CommMask successfull");


               /*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/

                            printf(" Leyendo canal 1 ");

                            Status_osc = WaitCommEvent(com_osc, &dwEventMask, NULL); //Wait for the character to be received

                            /*-------------------------- Program will Wait here till a Character is received ------------------------*/

                            if (Status_osc == FALSE)
                                    {
                                            printf("\n    Error! in Setting WaitCommEvent()");
                                    }
                            else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
                                    {
                                            //printf("\n\n    Characters Received");
                                            do
                                                    {
                                                            Status_osc = ReadFile(com_osc, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
                                                            datos_recibidos_1_corto[i] = TempChar;
                                                           //  printf("%i",TempChar);
                                                            // printf("\n");
                                                            i++;
                                                            if(i%400==0) {printf(".");}
                                                }
                                            while (NoBytesRead > 0);



                                            /*------------Printing the RXed String to Console----------------------*/



                                            int j =0;
                                            int k=0;
                                            int l=0;
                                            printf(" %i",i); printf(" bytes.");

                                              for (j=0; j<15; j++)
                                              {
                                                  encabezado_corto[j]=datos_recibidos_1_corto[j];
                                              }

                                              for (j=6; j<10; j++)
                                              {
                                                  codigo_samplerate[j-6]=datos_recibidos_1[j];
                                              }



                                            for (j = 14; j < i-1; j++) {



                                                if (j % 2 == 0)
                                                {
                                                 primer_bit1_corto[k]=datos_recibidos_1_corto[j];
                                                }
                                                else
                                                {
                                                 segundo_bit1_corto[k]=datos_recibidos_1_corto[j];
                                                 if (segundo_bit1_corto[k] < 0){
                                                     segundo_bit1_corto[k]=segundo_bit1_corto[k]+257;
                                                 }
                                                 datos_en16_1_corto[k]=primer_bit1_corto[k]*256+segundo_bit1_corto[k];
                                                 k++;
                                                }





                                            }

                                                                                        printf(" Cantidad de puntos: %i\n",k);




                                    }


}

void leer_osc_corto_2()
{

    DWORD dwEventMask;
    DWORD NoBytesRead;

    int i = 0;
                            /*------------------------------------ Setting Receive Mask ----------------------------------------------*/

                            Status_osc = SetCommMask(com_osc, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

                    //	if (Status_osc == FALSE)
                    //		printf("\n\n    Error! in Setting CommMask");
                    //	else
                    //		printf("\n\n    Setting CommMask successfull");


               /*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/

                            printf(" Leyendo canal 2 ");

                            Status_osc = WaitCommEvent(com_osc, &dwEventMask, NULL); //Wait for the character to be received

                            /*-------------------------- Program will Wait here till a Character is received ------------------------*/

                            if (Status_osc == FALSE)
                                    {
                                            printf("\n    Error! in Setting WaitCommEvent()");
                                    }
                            else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
                                    {
                                            //printf("\n\n    Characters Received");
                                            do
                                                    {
                                                            Status_osc = ReadFile(com_osc, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
                                                            datos_recibidos_2_corto[i] = TempChar;
                                                           //  printf("%i",TempChar);
                                                            // printf("\n");
                                                            i++;
                                                            if(i%400==0) {printf(".");}
                                                }
                                            while (NoBytesRead > 0);



                                            /*------------Printing the RXed String to Console----------------------*/



                                            int j =0;
                                            int k=0;
                                            int l=0;
                                            printf(" %i",i); printf(" bytes.");

                                            for (j = 14; j < i-1; j++) {
                                            // fprintf(archivo, "%c", datos_recibidos_1[j] );  // guardar todos los datos
                                                    if (j % 2 == 0)
                                                    {
                                                     primer_bit2_corto[k]=datos_recibidos_2_corto[j];
                                                    }
                                                    else
                                                    {
                                                     segundo_bit2_corto[k]=datos_recibidos_2_corto[j];
                                                     if (segundo_bit2_corto[k] < 0){
                                                         segundo_bit2_corto[k]=segundo_bit2_corto[k]+257;
                                                     }
                                                     datos_en16_2_corto[k]=primer_bit2_corto[k]*256+segundo_bit2_corto[k];
                                                     k++;
                                                    }

                                            }

                                                                                        printf(" Cantidad de puntos: %i\n",k);




                                    }


}

float leer_param_osc()
{  
    DWORD dwEventMask;
    DWORD NoBytesRead;
    float n;
    int i = 0;
                            /*------------------------------------ Setting Receive Mask ----------------------------------------------*/

                            Status_osc = SetCommMask(com_osc, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

                    //	if (Status_osc == FALSE)
                    //		printf("\n\n    Error! in Setting CommMask");
                    //	else
                    //		printf("\n\n    Setting CommMask successfull");


               /*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/

                       //     printf("lectura puerto: ");

                            Status_osc = WaitCommEvent(com_osc, &dwEventMask, NULL); //Wait for the character to be received

                            /*-------------------------- Program will Wait here till a Character is received ------------------------*/

                            if (Status_osc == FALSE)
                                    {
                                            printf("\n    Error! in Setting WaitCommEvent()");
                                    }
                            else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
                                    {
                                            //printf("\n\n    Characters Received");
                                            do
                                                    {
                                                            Status_osc = ReadFile(com_osc, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
                                                            param_osc[i] = TempChar;
                                                            i++;
                                                }
                                            while (NoBytesRead > 0);



                                            /*------------Printing the RXed String to Console----------------------*/



                                            int j =0;
											int k=0;
											for (j = 0; j < i-1; j++) 
											    {
                                                 printf("%c", param_osc[j] ); 
										  		 																						
                                                 }
											n=atof(param_osc);		
											for (j = 0; j < i-1; j++) 
											    {
                                                  param_osc[j]=NULL; 
										  																			
                                                }
											
												


                                    }

                               
								  
								  return n;
}

void ejecute(const char* command)
{
strcpy(orden, command);

/*----------------------------- ESCRIBIR TIMEBASE SCALE; EL VALOR LO PONGO DESPUES----------------------------------------*/


DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port

dNoOFBytestoWrite = strlen(orden); // Calculating the no of bytes to write into the port

Status_osc = WriteFile(com_osc,               // Handle to the Serialport
                                   orden,            // Data to be written to the port
                                   dNoOFBytestoWrite,   // No of bytes to write into the port
                                   &dNoOfBytesWritten,  // No of bytes written to the port
                                   NULL);



if (Status_osc == TRUE) {
// printf("  %s - escrito a %s\n", orden, n_puerto_osc); //return 1;
}

else {
printf("\n\n   Error %d Al escribir",GetLastError()); //return 0;
}



}

void escalasV ()
{
	ejecute(":chan1:scal?\n");
        printf(" Escala del canal 1: ");
        escalaV1=leer_param_osc();
        //printf("\nel nro en string es %s y en float es: %f",param_osc,escalaV1);
	ejecute(":chan2:scal?\n");
        printf(" Escala del canal 2: ");
	escalaV2=leer_param_osc();
        //printf("\nel nro en string es %s y en float es: %f",param_osc,escalaV2);
		
}

void calculo_voltajes()
{   printf(" Calculando voltajes ");
  /*  FILE * archivo;
        archivo=fopen("salida.dat","a+"); */

        float correccion=65536;  // corrimiento por el complemento a 2

        for( int i=0; i< 1000000; i++)
            {
             valores_1[i]=(float)datos_en16_1[i]*escalaV1/factor;			//*escalaV1/factor;
             valores_2[i]=(float)datos_en16_2[i]*escalaV2/factor;			//*escalaV2/factor;
        /*     fprintf(archivo,"%f",valores_1[i]);
             fprintf(archivo,"%c",9);
             fprintf(archivo,"%f",valores_2[i]);
             fprintf(archivo,"%c",10); */
             if(i%62500==0) { printf("."); }
            }
printf(" hecho\n");
}

void detectar_subida()
{  //detectar cuando el canal 2 tiene el pulso
    int a=30000;
    bool est_pulso_act=0;
    bool pulso_detectado=0;
    int i=0;
    printf(" Detectando pulso ");
    while (i<1000000 && pulso_detectado==0)
    {
        if (valores_2[i]>0.8 && est_pulso_act==0 )
        {
            tpi=i;
            est_pulso_act=1;
            i++;
            if(i%a==0) { printf("."); }
         }
         else if (valores_2[i]<0.8 && est_pulso_act==1)
         {
             tpf=i;
             est_pulso_act=0;
             if (tpf-tpi> 1000) { pulso_detectado=1;}
             i++;
             if(i%a==0) { printf("."); }
         }
         else i++;
         if(i%a==0) { printf("."); }


    }

    printf(" ");
    delta_t=tpf-tpi;
    t0=tpi-delta_t;
    t1=tpi-delta_t/10;
    t2=tpi+delta_t/10; // margen de tiempo porque el foto no responde inmediatamente
    t3=tpi+delta_t/5; // me tomo un margen porque el pulso no decae inmediatamente
    t4=tpi+2*delta_t/5;
    t5=tpi+3*delta_t/5;
    t6=tpi+4*delta_t/5;
    t7=tpf-delta_t/10;
    float sample_time_largo=IEEE754();
    ancho_u_segundos=1000000*(float)delta_t*sample_time_largo;
    printf(" inicio: %i fin: %i ancho: %i puntos = %4.2e us.\n",tpi,tpf, delta_t,ancho_u_segundos);
   // printf("t0: %i t1: %i t2: %i t3: %i t4: %i t5: %i t6: %i t7: %i\n",t0,t1,t2,t3,t4,t5,t6,t7);
}

void calculo_promedios()
{   printf("\n calculando promedios...\n");
    //calculo V aplicado
    int suma_baseVapl=0;
    int suma_baseVapl2=0;
    int sumaVapl=0;
    int sumaVaplcuadrado=0;

    float divisor1= (float)t1*25-(float)t0*25;
    float divisor2= (float)t3*25-(float)t2*25;
    float divisor3= (float)t4*25-(float)t3*25;
    float divisor4= (float)t5*25-(float)t4*25;
    float divisor5= (float)t6*25-(float)t5*25;
    float divisor6= (float)t7*25-(float)t6*25;

    for (int x=t0; x<t1; x++)
    {
        suma_baseVapl=suma_baseVapl+datos_en16_2[x];
        suma_baseVapl2=suma_baseVapl2+datos_en16_2[x]*datos_en16_2[x];
     }
    Vapl0=escalaV2*(float)suma_baseVapl/divisor1;
    Vapl0cuadrado=escalaV2*escalaV2*(float)suma_baseVapl2/(25*divisor1);
   // printf("base al cuadrado promedio %f\n",Vapl0cuadrado);
    varVapl0=Vapl0cuadrado-Vapl0*Vapl0;
    desvestVapl0=pow(varVapl0,0.5);
    if (t1-t0-3<512) {errVapl0=dist_t[t1-t0-3]*desvestVapl0*pow(t1-t0,-0.5);}
    else errVapl0=1.96*desvestVapl0*pow(t1-t0,-0.5);

   // printf("\n el voltaje base del campo aplicado es: %f +/-%f V N: %i s: %f",Vapl0,errVapl0,t1-t0,desvestVapl0);
    for (int l= t2; l<t3; l++)
    {
        sumaVapl=sumaVapl+datos_en16_2[l];
        sumaVaplcuadrado=sumaVaplcuadrado+datos_en16_2[l]*datos_en16_2[l];
    }

  Vapl=escalaV2*((float)sumaVapl/divisor2);
  Vaplcuadrado=escalaV2*escalaV2*((float)sumaVaplcuadrado/divisor2)/25;
  varVapl=Vaplcuadrado-Vapl*Vapl;
  desvestVapl=pow(varVapl,0.5);

  if (t1-t0-3<512) {errVapl=dist_t[t3-t2-3]*desvestVapl*pow(t3-t2,-0.5);}
  else errVapl=1.96*desvestVapl*pow(t3-t2,-0.5);
 // printf("el valor de Vapl sin restar base es %f%c%f la SD: %f",Vapl,241,errVapl,desvestVapl);
  float err_finalVapl=100*pow(errVapl*errVapl+errVapl0*errVapl0,0.5);
  Vapl_final=100*Vapl-100*Vapl0;


  int sumaV0=0;
  int sumaV1=0;
  int sumaV2=0;
  int sumaV3=0;
  int sumaV4=0;
  int sumaV5=0;
  int sumaV0cuadrado=0;
  int sumaV1cuadrado=0;
  int sumaV2cuadrado=0;
  int sumaV3cuadrado=0;
  int sumaV4cuadrado=0;
  int sumaV5cuadrado=0;

  for (int k=t0; k<t1; k++)
    {
        sumaV0=sumaV0+datos_en16_1[k];
        sumaV0cuadrado=sumaV0cuadrado+datos_en16_1[k]*datos_en16_1[k];
     }

     V0=escalaV1*(float)sumaV0/divisor1;
     V0cuadrado=escalaV1*escalaV1*(float)sumaV0cuadrado/(divisor1*25);
     varV0=V0cuadrado-V0*V0;
     desvestV0=pow(varV0,0.5);
     if (t1-t0<512)
     {
         errV0=dist_t[t1-t0-3]*desvestV0*pow(t1-t0,-0.5);
     }
     else errV0=1.96*desvestV0*pow(t1-t0,-0.5);
    // printf("\n  El valor del V0 es: %f%c%f la SD es: %f N: %i", 1000*V0,241,1000*errV0,1000*desvestV0,t1-t0);


     for (int i= t2; i<t3; i++)
     {
         sumaV1=sumaV1+datos_en16_1[i];
         sumaV1cuadrado=sumaV1cuadrado+datos_en16_1[i]*datos_en16_1[i];
     }
    V1=escalaV1*(float)sumaV1/divisor2;
    V1cuadrado=escalaV1*escalaV1*sumaV1cuadrado/(divisor2*25);
    varV1=V1cuadrado-V1*V1;
    desvestV1=pow(varV1,0.5);
    if (t3-t2<512)
    {
    errV1=dist_t[t3-t2-3]*desvestV1*pow(t3-t2,-0.5);
    }
    else errV1=1.96*desvestV1*pow(t3-t2,-0.5);

  // printf("\nEl valor del V1 es: %f%c%f la SD es: %f N: %i", V1,241,errV1,desvestV1,t3-t2);
    deltaV1=V1-V0;
    errdeltaV1=pow(errV0*errV0+errV1*errV1,0.5);
 //   printf("\nEl valor del deltaV es: %f%c%f", deltaV1,241,errdeltaV1);
   deltaV1_V=(V1-V0)/V0;
   errdeltaV1_V=pow((errdeltaV1/deltaV1)*(errdeltaV1/deltaV1)+(errV0/V0)*(errV0/V0),0.5)*deltaV1_V;
  //  printf("\nEl valor del deltaV/V0 es: %f%c%f", deltaV1_V,241,errdeltaV1_V);
// SEGUNDO DELTA V / V
    for (int i= t3; i<t4; i++)
    {
        sumaV2=sumaV2+datos_en16_1[i];
        sumaV2cuadrado=sumaV2cuadrado+datos_en16_1[i]*datos_en16_1[i];
    }
 //   printf("\nHALAAAAAAAAA %i\n %f",sumaV2,divisor3);

   V2=escalaV1*(float)sumaV2/divisor3;
   V2cuadrado=escalaV1*escalaV1*(float)sumaV2cuadrado/(divisor3*25);
//    printf("\n adfjhsjdfa %i %f %f\n",sumaV2cuadrado,divisor3,V2cuadrado);
   varV2=V2cuadrado-V2*V2;
//    printf("\n adfjhsjdfa %i %f %f %f\n",sumaV2cuadrado,divisor3,V2cuadrado,V2*V2);
   desvestV2=pow(varV2,0.5);
   if (t4-t3<512)
   {
   errV2=dist_t[t4-t3-3]*desvestV2*pow(t4-t3,-0.5);
   }
   else errV2=1.96*desvestV2*pow(t4-t3,-0.5);

//  printf("\nEl valor del V2 es: %f%c%f la SD es: %f N: %i", V2,241,errV2,desvestV2,t4-t3);
   deltaV2=V2-V0;
   errdeltaV2=pow(errV0*errV0+errV2*errV2,0.5);
  // printf("\nEl valor del deltaV es: %f%c%f", deltaV2,241,errdeltaV2);
  deltaV2_V=(V2-V0)/V0;
  errdeltaV2_V=pow((errdeltaV2/deltaV2)*(errdeltaV2/deltaV2)+(errV0/V0)*(errV0/V0),0.5)*deltaV2_V;
 // printf("\nEl valor del deltaV/V0 es: %f%c%f", deltaV2_V,241,errdeltaV2_V);

// TERCER DV/V

   for (int i= t4; i<t5; i++)
   {
       sumaV3=sumaV3+datos_en16_1[i];
       sumaV3cuadrado=sumaV3cuadrado+datos_en16_1[i]*datos_en16_1[i];
   }
//   printf("\nHALAAAAAAAAA %i\n %f",sumaV3,divisor3);

  V3=escalaV1*(float)sumaV3/divisor4;
  V3cuadrado=escalaV1*escalaV1*(float)sumaV3cuadrado/(divisor4*25);
//   printf("\n adfjhsjdfa %i %f %f\n",sumaV3cuadrado,divisor4,V2cuadrado);
  varV3=V3cuadrado-V3*V3;
//   printf("\n adfjhsjdfa %i %f %f %f\n",sumaV3cuadrado,divisor4,V3cuadrado,V3*V3);
  desvestV3=pow(varV3,0.5);
  if (t5-t4<512)
  {
  errV3=dist_t[t5-t4-3]*desvestV3*pow(t5-t4,-0.5);
  }
  else errV3=1.96*desvestV3*pow(t5-t4,-0.5);

 // printf("\nEl valor del V3 es: %f%c%f la SD es: %f N: %i", V3,241,errV3,desvestV3,t5-t4);
  deltaV3=V3-V0;
  errdeltaV3=pow(errV0*errV0+errV3*errV3,0.5);
 // printf("\nEl valor del deltaV es: %f%c%f", deltaV3,241,errdeltaV3);
 deltaV3_V=(V3-V0)/V0;
 errdeltaV3_V=pow((errdeltaV3/deltaV3)*(errdeltaV3/deltaV3)+(errV0/V0)*(errV0/V0),0.5)*deltaV3_V;
// printf("\nEl valor del deltaV/V0 es: %f%c%f", deltaV3_V,241,errdeltaV3_V);

//cuarto deltaV
  for (int i= t5; i<t6; i++)
  {
      sumaV4=sumaV4+datos_en16_1[i];
      sumaV4cuadrado=sumaV4cuadrado+datos_en16_1[i]*datos_en16_1[i];
  }
//   printf("\nHALAAAAAAAAA %i\n %f",sumaV3,divisor3);

 V4=escalaV1*(float)sumaV4/divisor5;
 V4cuadrado=escalaV1*escalaV1*(float)sumaV4cuadrado/(divisor5*25);
 // printf("\n adfjhsjdfa %i %f %f\n",sumaV4cuadrado,divisor5,V4cuadrado);
 varV4=V4cuadrado-V4*V4;
 // printf("\n adfjhsjdfa %i %f %f %f\n",sumaV4cuadrado,divisor5,V4cuadrado,V4*V4);
 desvestV4=pow(varV4,0.5);
 if (t6-t5<512)
 {
 errV4=dist_t[t6-t5-3]*desvestV4*pow(t6-t5,-0.5);
 }
 else errV4=1.96*desvestV4*pow(t6-t5,-0.5);

// printf("\nEl valor del V4 es: %f%c%f la SD es: %f N: %i", V4,241,errV4,desvestV4,t6-t5);
 deltaV4=V4-V0;
 errdeltaV4=pow(errV0*errV0+errV4*errV4,0.5);
// printf("\nEl valor del deltaV es: %f%c%f", deltaV4,241,errdeltaV4);
deltaV4_V=(V4-V0)/V0;
errdeltaV4_V=pow((errdeltaV4/deltaV4)*(errdeltaV4/deltaV4)+(errV0/V0)*(errV0/V0),0.5)*deltaV4_V;
// printf("\nEl valor del deltaV/V0 es: %f%c%f", deltaV4_V,241,errdeltaV4_V);


 //quinto deltaV
   for (int i= t6; i<t7; i++)
   {
       sumaV5=sumaV5+datos_en16_1[i];
       sumaV5cuadrado=sumaV5cuadrado+datos_en16_1[i]*datos_en16_1[i];
   }
 //   printf("\nHALAAAAAAAAA %i\n %f",sumaV3,divisor3);

  V5=escalaV1*(float)sumaV5/divisor6;
  V5cuadrado=escalaV1*escalaV1*(float)sumaV5cuadrado/(divisor6*25);
 //  printf("\n adfjhsjdfa %i %f %f\n",sumaV5cuadrado,divisor6,V5cuadrado);
  varV5=V5cuadrado-V5*V5;
 //  printf("\n adfjhsjdfa %i %f %f %f\n",sumaV5cuadrado,divisor5,V5cuadrado,V5*V5);
  desvestV5=pow(varV5,0.5);
  if (t7-t6<512)
  {
  errV5=dist_t[t7-t6-3]*desvestV5*pow(t7-t6,-0.5);
  }
  else errV5=1.96*desvestV5*pow(t7-t6,-0.5);

//printf("\nEl valor del V5 es: %f%c%f la SD es: %f N: %i", V5,241,errV5,desvestV5,t7-t6);
  deltaV5=V5-V0;
  errdeltaV5=pow(errV0*errV0+errV5*errV5,0.5); // ver arriba
 // printf("\nEl valor del deltaV es: %f%c%f", deltaV5,241,errdeltaV5);
 deltaV5_V=(V5-V0)/V0;
 errdeltaV5_V=pow((errdeltaV5/deltaV5)*(errdeltaV5/deltaV5)+(errV0/V0)*(errV0/V0),0.5)*deltaV5_V;
 //  printf("\n El valor del deltaV5/V0 es: %f%c%f", deltaV5_V,241,errdeltaV5_V);

// calculo de deltas



delta1=calcular_delta(angulo_alfa,deltaV1_V);
delta2=calcular_delta(angulo_alfa,deltaV2_V);
delta3=calcular_delta(angulo_alfa,deltaV3_V);
delta4=calcular_delta(angulo_alfa,deltaV4_V);
delta5=calcular_delta(angulo_alfa,deltaV5_V);

float V1promedio=(V2+V3+V4)/3;
float deltaVpromedio=(deltaV2+deltaV3+deltaV4)/3;
float deltaVVpromedio=(deltaV2_V+deltaV3_V+deltaV4_V)/3;
float deltapromedio=(delta2+delta3+delta4)/3;

printf("\n Valor <V aplicado>: %c%4.2f+/-%4.2f V",9,Vapl_final,err_finalVapl);
printf("\n Valor <deltaV234/V0>: %c%4.5f\n",9,deltaVVpromedio);
printf(" Valor <deltaV234>: %c%4.2f mV\n",9,1000*deltaVpromedio);
printf(" Valor de V0 es: %c%4.2f mV \n",9,1000*V0);
printf(" Valor <V234>: %c%c%4.2f mV\n",9,9,1000*V1promedio);
printf(" Valor <delta234>: %c%f\n",9,deltapromedio);

// printf("angulo: %f deltaV/V: %f delta1: %f\n",angulo_alfa,deltaV1_V,delta1);
  printf("\n V1 %c  V2 %c  V3 %c  V4 %c  V5\n",9,9,9,9,9);
  printf(" %4.2f %c %4.2f %c %4.2f %c %4.2f %c %4.2f\n",1000*V1,9,1000*V2,9,1000*V3,9,1000*V4,9,1000*V5);
  printf(" DV1 %c  DV2 %c  DV3 %c  DV4 %c  DV5\n",9,9,9,9,9);
  printf(" %4.2f %c %4.2f %c %4.2f %c %4.2f %c %4.2f\n",1000*deltaV1,9,1000*deltaV2,9,1000*deltaV3,9,1000*deltaV4,9,1000*deltaV5);
  printf(" dV1/V0%cdV2/V0%cdV3/V0%cdV4/V0%cdV5/V0\n",9,9,9,9);
  printf(" %4.4f%c%4.4f%c%4.4f%c%4.4f%c%4.4f\n",deltaV1_V,9,deltaV2_V,9,deltaV3_V,9,deltaV4_V,9,deltaV5_V);
  printf(" delta1%cdelta2%cdelta3%cdelta4%cdelta5\n",9,9,9,9);
  printf(" %4.4f%c%4.4f%c%4.4f%c%4.4f%c%4.4f\n\n",delta1,9,delta2,9,delta3,9,delta4,9,delta5);



}

void obtener_fecha()
{
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (fecha,80,"%Y-%m-%d_%H-%M-%S%p",timeinfo);
    strftime (hora,80,"%H.%M.%S %p",timeinfo);
  }

void guardar_info()
{   strcat(nombre_archivo_info_mediciones,"_");
    strcat(nombre_archivo_info_mediciones,fecha);
    mkdir(nombre_archivo_info_mediciones); //hago el directorio antes q conicida con el archivo
    //printf("AHAHHA");

  //  printf("%s",nombre_archivo_individual);
    strcpy(nombre_carpeta_info_mediciones,nombre_archivo_info_mediciones);
    strcat(nombre_archivo_info_mediciones,".txt");
    FILE * archivo_info;
    archivo_info=fopen(nombre_archivo_info_mediciones,"w");
    fprintf(archivo_info,"%s", "MUESTRA: ");
    fprintf(archivo_info,"%s",nombre_muestra);
    fprintf(archivo_info, "%c",10);
    fprintf(archivo_info,"%s", "FECHA: ");
    fprintf(archivo_info,"%s",fecha);
    fprintf(archivo_info, "%c",10);
    fprintf(archivo_info,"%s", "ALFA: ");
    fprintf(archivo_info,"%f",angulo_alfa);
    fprintf(archivo_info, "%c",10);
    fprintf(archivo_info,"%s","N medida"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","Vapl(V)"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","V^2"); fprintf(archivo_info, "%c",9);
     fprintf(archivo_info,"%s","delta1"); fprintf(archivo_info, "%c",9);
     fprintf(archivo_info,"%s","delta2"); fprintf(archivo_info, "%c",9);
     fprintf(archivo_info,"%s","delta3"); fprintf(archivo_info, "%c",9);
     fprintf(archivo_info,"%s","delta4"); fprintf(archivo_info, "%c",9);
     fprintf(archivo_info,"%s","delta5"); fprintf(archivo_info, "%c",9);

    fprintf(archivo_info,"%s","V0 (mV)"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","V1"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","V2"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","V3"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","V4"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","V5"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","deltaV1/V0"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","deltaV2/V0"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","deltaV3/V0"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","deltaV4/V0"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","deltaV5/V0"); fprintf(archivo_info, "%c",9);
    fprintf(archivo_info,"%s","ancho de pulso (s)"); fprintf(archivo_info, "%c",10);

    fclose(archivo_info);
}

void pedir_offsets()
{
	ejecute(":chan1:offset?\n");
        printf(" Offset del canal 1: ");
	offset1=leer_param_osc();
	ejecute(":chan2:offset?\n");
        printf(" Offset del canal 2: ");
	offset2=leer_param_osc();
}

void leer_configuracion()
{
char contenido_archivo[4096];
char puerto[32];
char angle[32];
for (int y=0; y<32; y++) {puerto[y]=NULL; angle[y]=NULL;}

FILE *archivo;
archivo= fopen("config.txt","r");
if (archivo==NULL){printf("\nError al abrir archivo de configuracion!! Saliendo...\n");exit(EXIT_FAILURE);}
fseek (archivo, 0, SEEK_END);
int tamano_archivo=ftell(archivo);
//printf("el archivo tiene %i bytes\n",tamano_archivo);
int m=0;
int n=0;
fseek (archivo, 0, SEEK_SET);
int lineas=0;
int comienzos_lineas[64];
while (m<tamano_archivo)
{
    contenido_archivo[m]=getc(archivo);
  //  printf("%c",contenido_archivo[m]);
    if (contenido_archivo[m]==10) {lineas++;}
    m++;
    comienzos_lineas[lineas]=m;
  }
//printf("\nel archivo tiene %i lineas\n",lineas);
//printf("las lineas comienzan en: ");
//for (int i=0; i<lineas; i++) { printf("%i; ",comienzos_lineas[i]);}
//printf(" \n");
for (int i=0; i<lineas; i++)
{
 // printf("\n %i %i %c",i+1,comienzos_lineas[i],contenido_archivo[comienzos_lineas[i]+1]);
  if (contenido_archivo[comienzos_lineas[i]+1]==112) // si la linea arranca con una p
  {   if (contenido_archivo[comienzos_lineas[i]+7]!=61) {printf("\nConfig puerto en el archivo erronea\n");  exit(EXIT_FAILURE);}
     //caracter inicial de la linea
  //    printf("\n caracter inicial de la linea %c\n",contenido_archivo[comienzos_lineas[i]+1]);
     //caractger final de la linea
   //   printf("\n caracter final de la linea %c\n",contenido_archivo[comienzos_lineas[i+1]-1]);
      int a=comienzos_lineas[i]+8;
      int b=comienzos_lineas[i+1];
      for (int u=a;u<b; u++) {puerto[u-a]=contenido_archivo[u];}

  }
  else if (contenido_archivo[comienzos_lineas[i]+1]==97) //si la linea comienza con una a
  {    if (contenido_archivo[comienzos_lineas[i]+7]!=61) {printf("\nConfig angulo en el archivo erronea\n");  exit(EXIT_FAILURE);}
      //caracter inicial de la linea
   //    printf("\n caracter inicial de la linea %c\n",contenido_archivo[comienzos_lineas[i]+1]);
      //caractger final de la linea
    //   printf("\n caracter final de la linea %c\n",contenido_archivo[comienzos_lineas[i+1]-1]);
       int c=comienzos_lineas[i]+8;
       int d=comienzos_lineas[i+1];
       for (int t=c;t<d; t++) {angle[t-c]=contenido_archivo[t];}

  }
}
//printf("\n el puerto es: %s\n",puerto);
//printf("\n el angle es %s, y convertido a float es: %f",angle,atof(angle));
strcat(n_puerto_osc,puerto);
angulo_alfa=atof(angle);

}

void cerrar_puerto()
{
    CloseHandle(com_osc);//Closing the Serial Port
 }

void crear_directorios()
{
 char directorio[]="Datos";
 mkdir(directorio);

}

void guardar_encabezado()
{

    FILE * archivo_info3;
    archivo_info3=fopen("encabezado1.txt","w");
    for (int i=0;i<18; i++) fprintf(archivo_info3,"%c",encabezado_1[i]);
    fclose(archivo_info3);
    FILE * archivo_info4;
    archivo_info4=fopen("encabezado2.txt","w");
    for (int i=0;i<18; i++) fprintf(archivo_info4,"%c",encabezado_2[i]);
    fclose(archivo_info4);

    FILE * encabezado_corto_archivo;
    encabezado_corto_archivo=fopen("encabezado_corto.txt","w");
    for (int i=0; i<15; i++) fprintf(encabezado_corto_archivo,"%c",encabezado_corto[i]);
    fclose(encabezado_corto_archivo);



}

void guardar_curvas()
{   float escala_tiempo;
    ejecute("timebase:scale?\n");
    printf(" Tiempo por division: ");
    escala_tiempo=leer_param_osc();
    float sample_time=escala_tiempo/250;
    printf(" Tiempo por muestra: %e\n", sample_time);

    FILE * archivo_individual;
    archivo_individual=fopen(nombre_archivo_individual,"w");
    printf("%s",nombre_archivo_individual);
    if(archivo_individual==NULL) {printf("ERROR AL GUARDAR ARCHIVO INDIVIDUAL");}
           float correccion=65536;  // corrimiento por el complemento a 2
           float tiempo_muestra;
           for( int i=0; i< 4000; i++)
               { tiempo_muestra=(float)i*sample_time;
                valores_1_cortos[i]=(float)datos_en16_1_corto[i]*escalaV1/25;			//*escalaV1/factor;
                valores_2_cortos[i]=(float)datos_en16_2_corto[i]*escalaV2/25;			//*escalaV2/factor;

                fprintf(archivo_individual,"%4.5e",tiempo_muestra);
                 fprintf(archivo_individual,"%c",9);
                fprintf(archivo_individual,"%f",valores_1_cortos[i]);
                fprintf(archivo_individual,"%c",9);
                fprintf(archivo_individual,"%f",valores_2_cortos[i]);
                fprintf(archivo_individual,"%c",10);
                //if(i%200==0) { printf("."); }
               }
 //  printf(" hecho\n");
}

void guardar_bruto_corto()
{
    FILE * archivo_corto_bruto;
    archivo_corto_bruto=fopen("CANAL_CORTO.dat","w");

for (int i=0; i<8015; i++)
        {
            fprintf(archivo_corto_bruto,"%c",datos_recibidos_2_corto[i]);
        }

   FILE * archivo_corto_concatenado;
   archivo_corto_concatenado=fopen("canal_concatenado.dat","w");
   for (int i=0; i<4000; i++)
           {
               fprintf(archivo_corto_concatenado,"%i",datos_en16_1_corto[i]); fprintf(archivo_corto_concatenado,"%c",10);
           }


}

void confirmar_alfa()
{   int k=0;
    char respuesta_alfa[32];
    printf(" El alfa elegido es: %f;  s/n ... ",angulo_alfa);
    scanf("%s",&respuesta_alfa);
    while (k==0)
    {

        char si1[]="s";
        char si2[]="S";
        char si3[]="Y";
        char si4[]="y";
        char no1[]="n";
        char no2[]="N";
        if (strcmp(si1,respuesta_alfa) == 0  || strcmp(si2,respuesta_alfa) == 0 )
        {    k++;
            printf(" OK\n");

                }
        else if ( strcmp(si3,respuesta_alfa) == 0 || strcmp(si4,respuesta_alfa) == 0)

        {   k++;
            printf(" OK\n");

         }

        else if (strcmp(no1,respuesta_alfa) == 0  || strcmp(no2,respuesta_alfa) == 0)
        {   k++;
            printf("\nCorrija la configuracion, saliendo... \n");
           exit(EXIT_FAILURE);

        }
        else { printf(" El alfa elegido es: %f;  S/N ... ",angulo_alfa);
        scanf("%s",&respuesta_alfa);}

    }


}

void preguntar_si_seguir()
{   int seguir_respondido=0;
    while(seguir_respondido==0)
        {


    printf("\n Quiere adquirir otra medida? s/n ...\n ");
    char respuesta_seguir[32];
    char seguir1[]="s";
    char seguir2[]="S";
    char seguir3[]="Y";
    char seguir4[]="y";
    char frenar1[]="n";
    char frenar2[]="N";
    scanf("%s",respuesta_seguir);
    if (strcmp(respuesta_seguir,seguir1)==0 || strcmp(respuesta_seguir,seguir2)==0 || strcmp(respuesta_seguir,seguir3)==0 || strcmp(respuesta_seguir,seguir4)==0)
    {
        printf("\n Adquiriendo siguiente... \n");
        seguir_respondido=1;
    }
    else if(strcmp(respuesta_seguir,frenar1)==0 || strcmp(respuesta_seguir,frenar2)==0)
    {
        printf("\n terminando... \n");
        seguir=1;
        seguir_respondido=1;
    }
    else
    {
        printf("\n Quiere adquirir otra medida? S/N ...\n ");
        scanf("%s",respuesta_seguir);
    }
}
}



int main()
{  // elegir_puerto_osc();

    char n_medida[8];
    crear_directorios();
    obtener_fecha();
    leer_configuracion();
    abrir_puerto_osc();
    PurgeComm(com_osc,0x0001);
    PurgeComm(com_osc,0x0002);
    PurgeComm(com_osc,0x0004);
    PurgeComm(com_osc,0x0008);
    confirmar_alfa();
    pedir_offsets();
    escalasV();
    preguntar_nombre();
    guardar_info();
    cerrar_puerto();
    printf(" Cuando la medida este lista presione cualquier tecla para continuar... \n");
    getch();
    while (seguir==0) {
    abrir_puerto_osc();
     escalasV();
    ejecute(":acq1:lmem?\n");
    leer_datos_osc_1();
    ejecute(":acq2:lmem?\n");
    leer_datos_osc_2();
    calculo_voltajes();
    detectar_subida();
    calculo_promedios();
    ejecute(":acquire1:memory?\n");
    leer_osc_corto_1();
    ejecute(":acquire2:memory?\n");
    leer_osc_corto_2();
     guardar_encabezado();
     obtener_fecha();
     char Vapl_string[32];
     strcpy(nombre_archivo_individual,nombre_carpeta_info_mediciones); //meto la carpeta en el nombre
     strcat(nombre_archivo_individual,"\\");
     strcat(nombre_archivo_individual,nombre_muestra);
     strcat(nombre_archivo_individual,"_");
     int q=sprintf(Vapl_string,"%4.2f",Vapl_final);
     strcat(nombre_archivo_individual,Vapl_string);
     strcat(nombre_archivo_individual,"V_");
     int r=sprintf(n_medida,"%i",medida);
     strcat(nombre_archivo_individual,n_medida);
     strcat(nombre_archivo_individual,".dat");
     guardar_curvas();

     FILE * archivo_info;
      archivo_info=fopen(nombre_archivo_info_mediciones,"a");
      fprintf(archivo_info,"%s",n_medida); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3e",Vapl_final); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3e",Vapl_final*Vapl_final*10000); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3e",delta1); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3e",delta2); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3e",delta3); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3e",delta4); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3e",delta5); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3f",V0*1000); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3f",V1*1000); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3f",V2*1000); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3f",V3*1000); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3f",V4*1000); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%4.3f",V5*1000); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%f",deltaV1_V); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%f",deltaV2_V); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%f",deltaV3_V); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%f",deltaV4_V); fprintf(archivo_info, "%c",9);
      fprintf(archivo_info,"%f",deltaV5_V); fprintf(archivo_info, "%c",9);
       fprintf(archivo_info,"%4.3e",ancho_u_segundos); fprintf(archivo_info, "%c",10);
      fclose(archivo_info);
     cerrar_puerto();
     preguntar_si_seguir();
     medida++;

    }
    return 0;
}


    





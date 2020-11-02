/**
 * Подсистема памяти. Предусмотреть возможность указать подсистему для проверки
производительности: RAM (оперативная память), HDD/SSD и flash.
 * Размер блока данных в байтах, Кб или Мб. Если размерность не указана, то в байтах, если указана,
то соответственно в Кбайтах или Мбайтах.
 * Число испытаний, т.е. число раз повторений измерений.
Пример вызова программы: ./memory_test –m RAM –b 1024|1Kb –l 10
 * На выходе программы в одну строку CSV файла со следующей структурой:

MemoryType – тип памяти (RAM|HDD|SSD|flash) или модель устройства, на котором проводятся испытания; +

BlockSize – размер блока данных для записи и чтения на каждом испытании; +

ElementType – тип элементов используемых для заполнения массива данных; +

BufferSize – размер буфера, т.е. порции данных для выполнения одно операции записи или чтения; +

LaunchNum – порядковый номер испытания; +

Timer – название функции обращения к таймеру (для измерения времени); +

WriteTime – время выполнения отдельного испытания с номером LaunchNum [секунды]; +

AverageWriteTime – среднее время записи из LaunchNum испытаний [секунды]; +

WriteBandwidth – пропускная способность памяти (BLOCK_SIZE/AverageWriteTime) * 10 6 [Mb/s]

AbsError(write) – абсолютная погрешность измерения времени записи или СКО [секунды];

RelError(write) – относительная погрешность измерения времени [%];

ReadTime – время выполнения отдельного испытания LaunchNum [секунды];+

AverageReadTime – среднее время записи из LaunchNum испытаний [секунды]; + 

ReadBandwidth – пропускная способность памяти (BLOCK_SIZE/AverageReadTime) * 10 6 [Mб/сек.]

AbsError(read) – абсолютная погрешность измерения времени чтения или СКО [секунды];+

RelError(read) – относительная погрешность измерения времени [%]. +
   
   * Оценить пропускную способность оперативной памяти при
работе с блоками данных равными объёму кэш-линии, кэш-памяти L1, L2 и L3 уровня и превышающего
его. Для HDD|SSD и flash провести серию из 20 испытаний с блоками данных начиная с 4 Мб с шагом
4Мб. Результаты всех испытаний сохранить в один CSV файл со структурой, описанной в п.1.

**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>


//вычисление работы оперативной памяти
int RAMw(long int number_of_elements)
{
    double write_time = 0;
    struct timespec ts2, ts1;
    int buffer;

    srand(time(0));
    
    int *A;
    A = malloc( sizeof(*A) * number_of_elements);

    for (int i = 0; i < number_of_elements; i++)
    {
        buffer = rand() % 1000;
        clock_gettime(CLOCK_REALTIME, &ts1);
        A[i] = buffer;
        clock_gettime(CLOCK_REALTIME, &ts2);
        write_time += 1000000000 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);
    }
    return write_time;

}

int RAMr(long int number_of_elements)
{
    double read_time = 0;
    struct timespec ts2, ts1;

    srand(time(0));
    
    int *A;
    A = malloc( sizeof(*A) * number_of_elements);

    for (int i = 0; i < number_of_elements; i++)
    {
        clock_gettime(CLOCK_REALTIME, &ts1);
        A[i];
        clock_gettime(CLOCK_REALTIME, &ts2);
        read_time += 1000000000 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);
    }

    return read_time;
}




//работа памятью
int memoryR(long int number_of_elements, char *path)
{
    double read_time = 0;
    srand(time(0));
    int ch;
   

    struct timespec ts2, ts1;
   
    FILE *fp;

    fp = fopen(path, "r");

	if ( ! fp )
        return 1;


  clock_gettime(CLOCK_REALTIME, &ts1);
	while ( ( ch = fgetc(fp) ) != EOF );
	clock_gettime(CLOCK_REALTIME, &ts2);
    read_time += 1000000000 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);
   
 	
    fclose(fp);
    
    return read_time;
}



int memoryW(long int number_of_elements, char *path)
{
    double write_time = 0;
    int *buffer;
    buffer = malloc( sizeof(*buffer) * number_of_elements);
    struct timespec ts2, ts1;
    FILE *fp;

	for (int i = 0; i < number_of_elements; i++)
        buffer[i] = rand() % 1000;
    
    fp = fopen(path, "w");
	if ( ! fp )
        return 1;



    for (int i = 0; i < number_of_elements; i++)
	{
		clock_gettime(CLOCK_REALTIME, &ts1);
        fprintf(fp, "%d", buffer[i]);
 		clock_gettime(CLOCK_REALTIME, &ts2);
    	write_time += 1000000000 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);     
    }  
   
    
    fclose(fp);
    return write_time;
}


// вычисления данных
void Math(int n, char *operation, long int number_of_elements)
{
    double averageW = 0,  absErrorW = 0, relErrorW = 0, writeBandwidth = 0; 
    double read_time = 0, write_time = 0;
	
	double dispersion = 0;
	double *testArR;
	double *testArW;
	testArR = malloc( sizeof(*testArR) * n);
	testArW = malloc( sizeof(*testArW) * n);
    double averageR = 0, absErrorR = 0, relErrorR = 0;
    double readBandwidth = 0; 

    if(strcmp (operation, "RAM") == 0)
    {
		write_time = RAMw(number_of_elements);
		read_time = RAMr(number_of_elements);
		
		for(int i = 0; i < n; i++)
		{	
			averageW += RAMw(number_of_elements);		
			testArW[i] = RAMw(number_of_elements);
			averageR += RAMr(number_of_elements);
			testArR[i] = RAMr(number_of_elements);				
				
        } 
		averageR /= n; averageW /= n;
		
		for(int i = 0; i < n; i++) 
			dispersion += pow(testArR[i] - averageR, 2) ;
            
		absErrorR = sqrt(dispersion / n);
		relErrorR = (absErrorR/ averageR) * 100;		
		readBandwidth = (number_of_elements / averageR) * pow(10, 6);
		dispersion = 0;
		for(int i = 0; i < n; i++) 
			dispersion += pow(testArW[i] - averageW, 2);
		
		absErrorW = sqrt(dispersion / n);
		relErrorW = (absErrorW/ averageW) * 100;
		writeBandwidth = (number_of_elements / averageW) * pow(10, 6);
		dispersion = 0;
	}   

        else if(strcmp (operation, "SSD") == 0)
        {

            write_time = memoryW(number_of_elements, "txt.txt");
			read_time = memoryR(number_of_elements, "txt.txt");            
			
			remove ("txt.txt");
            for(int i = 0; i < n; i++)
            {
				testArW[i] = memoryW(number_of_elements, "txt.txt");
                averageW += memoryW(number_of_elements, "txt.txt");
			    
                averageR += memoryR(number_of_elements, "txt.txt");
				testArR[i] = memoryR(number_of_elements, "txt.txt");
			
				remove ("txt.txt");
            } 

           averageR /= n; averageW /= n;

            for(int i = 0; i < n; i++) 
                dispersion += pow(testArR[i] - averageR, 2) ;
           
            
			absErrorR = sqrt(dispersion / n);
            relErrorR = (absErrorR/ averageR) * 100;		

			readBandwidth = (number_of_elements / averageR) * pow(10, 6);
			
			dispersion = 0;
			
			for(int i = 0; i < n; i++) 
                dispersion += pow(testArW[i] - averageW, 2) ;

            absErrorW = sqrt(dispersion / n);
            relErrorW = (absErrorW/ averageW) * 100;
          
            writeBandwidth = (number_of_elements / averageW) * pow(10, 6);
			dispersion = 0;
		
        }

        else if(strcmp (operation, "flash") == 0)
        {
			write_time = memoryW(number_of_elements, "/run/media/fuki/'ADATA UFD'/txt.txt");
            read_time = memoryR(number_of_elements, "/run/media/fuki/'ADATA UFD'/txt.txt");
           	remove ("/run/media/fuki/'ADATA UFD'/txt.txt");
            for(int i = 0; i < n; i++)
            {
				
				testArW[i]  = memoryW(number_of_elements, "/run/media/fuki/'ADATA UFD'/txt.txt");
                averageW += memoryW(number_of_elements, "/run/media/fuki/'ADATA UFD'/txt.txt");

                averageR += memoryR(number_of_elements, "/run/media/fuki/'ADATA UFD'/txt.txt");
				testArR[i] =  memoryR(number_of_elements, "/run/media/fuki/'ADATA UFD'/txt.txt");
				remove ("/run/media/fuki/'ADATA UFD'/txt.txt");
            } 

            averageR /= n; averageW /= n;

            for(int i = 0; i < n; i++) 
                dispersion += pow(testArR[i] - averageR, 2);
                        
			absErrorR = sqrt(dispersion / n);
            relErrorR = (absErrorR/ averageR) * 100;		
			readBandwidth = (number_of_elements / averageR) * pow(10, 6);
			
			dispersion = 0;
	
			for(int i = 0; i < n; i++) 
                dispersion += pow(testArW[i] - averageW, 2);

            absErrorW = sqrt(dispersion / n);
            relErrorW = (absErrorW/ averageW) * 100;
          
            writeBandwidth = (number_of_elements / averageW) * pow(10, 6);
			dispersion = 0;

        }
    

    FILE  *file;
	free(testArR);
	free(testArW);
	file = fopen("itog1.cvs", "a"); 
    
    fprintf(file, "\nMemoryType: %s , BlockSize: %ld, ElementType: int, BufferSize: , Timer: clock_gettime(),  ", operation, number_of_elements);
	fprintf(file, "WriteTime: %.3f , AverageWriteTime: %.3f , WriteBandwidth: %.3f , AbsError(write): %.3f , RelError(write): %.3f  ", write_time, averageW, writeBandwidth, absErrorW, relErrorW);
	fprintf(file, "ReadTime: %.3f , AverageReadTime: %.3f , ReadBandwidth: %.3f , AbsError(read): %.3f , RelError(read): %.3f ", read_time, averageR, readBandwidth, absErrorR, relErrorR);

    fclose(file);


}



/*
    "-m"
    "-b"
    "-l"
    "RAM"
    "SSD"
    "flash"
    "Kb"
    "Mb"
*/

int main(int argc, char **argv)
{
    long int number_of_elements;
    if (argc >= 4)
    {
        if (strcmp (argv[1], "m") == 0)
        {
            if(strcmp (argv[2], "RAM") == 0)
                {
                    if (strcmp (argv[4], "Kb") == 0)
                        number_of_elements = atoi(argv[3]) * 1024;
                    else if (strcmp (argv[4], "Mb") == 0)
                        number_of_elements = atoi(argv[3]) * 1024 * 1024;
                    else
                        number_of_elements = atoi(argv[3]);
                    
                    int n;
                    if (strcmp (argv[5], "l") == 0)
                        n = atoi(argv[6]);
                    else
                        n = atoi(argv[5]);
                    printf("%ld\n", number_of_elements);
 					printf("%d\n", n);
                    Math(n, "RAM", number_of_elements);  
                                      
                }

                else if(strcmp (argv[2], "SDD") == 0)
                {
                    if (strcmp (argv[4], "Kb") == 0)
                        number_of_elements = atoi(argv[3]) * 1024;
                    else if (strcmp (argv[4], "Mb") == 0)
                        number_of_elements = atoi(argv[3]) * 1024 * 1024;
                    else
                        number_of_elements = atoi(argv[3]);
                    
                     int n;
                    if (strcmp (argv[5], "l") == 0)
                        n = atoi(argv[6]);
                    else
                        n = atoi(argv[5]);
					printf("%ld\n", number_of_elements);
 					printf("%d\n", n);
                    Math(n, "SSD", number_of_elements);                       
                }

                else if(strcmp (argv[2], "flash") == 0)
                {
                    if (strcmp (argv[4], "Kb") == 0)
                        number_of_elements = atoi(argv[3]) * 1024;
                    else if (strcmp (argv[4], "Mb") == 0)
                        number_of_elements = atoi(argv[3]) * 1024 * 1024;
                    else
                        number_of_elements = atoi(argv[3]);
                    
                     int n;
                    if (strcmp (argv[5], "l") == 0)
                        n = atoi(argv[6]);
                    else
                        n = atoi(argv[5]);

                    Math(n, "flash", number_of_elements);
                   
                }

                else
                    printf("Неправильный ввод.\n" );
                   
            }
        }  

    return 0;

}

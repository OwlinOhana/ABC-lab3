# ABC-lab3
# Оценка производительности подсистемы памяти

* Подсистема памяти. Предусмотреть возможность указать подсистему для проверки
производительности: RAM (оперативная память), HDD/SSD и flash.
 * Размер блока данных в байтах, Кб или Мб. Если размерность не указана, то в байтах, если указана,
то соответственно в Кбайтах или Мбайтах.
 * Число испытаний, т.е. число раз повторений измерений.
Пример вызова программы: ./memory_test –m RAM –b 1024|1Kb –l 10
 * На выходе программы в одну строку CSV файла со следующей структурой:

* Оценить пропускную способность оперативной памяти при
работе с блоками данных равными объёму кэш-линии, кэш-памяти L1, L2 и L3 уровня и превышающего
его. Для HDD|SSD и flash провести серию из 20 испытаний с блоками данных начиная с 4 Мб с шагом
4Мб. Результаты всех испытаний сохранить в один CSV файл со структурой, описанной в п.1.

                                                      ──────── • ✤ • ────────
                                                             Вывод в файл
                                                      ──────── • ✤ • ───────
✻MemoryType – тип памяти (RAM|HDD|SSD|flash) или модель устройства, на котором проводятся испытания; 

✻BlockSize – размер блока данных для записи и чтения на каждом испытании; 

✻ElementType – тип элементов используемых для заполнения массива данных; 

✻BufferSize – размер буфера, т.е. порции данных для выполнения одно операции записи или чтения; 

✻LaunchNum – порядковый номер испытания; 

✻Timer – название функции обращения к таймеру (для измерения времени); 

✻WriteTime – время выполнения отдельного испытания с номером LaunchNum [секунды]; 

✻AverageWriteTime – среднее время записи из LaunchNum испытаний [секунды]; 

✻WriteBandwidth – пропускная способность памяти (BLOCK_SIZE/AverageWriteTime) * 10 6 [Mb/s]

✻AbsError(write) – абсолютная погрешность измерения времени записи или СКО [секунды];

✻RelError(write) – относительная погрешность измерения времени [%];

✻ReadTime – время выполнения отдельного испытания LaunchNum [секунды];

✻AverageReadTime – среднее время записи из LaunchNum испытаний [секунды]; 

✻ReadBandwidth – пропускная способность памяти (BLOCK_SIZE/AverageReadTime) * 10 6 [Mб/сек.]

✻AbsError(read) – абсолютная погрешность измерения времени чтения или СКО [секунды];

✻RelError(read) – относительная погрешность измерения времени [%]. 

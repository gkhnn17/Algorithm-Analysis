//
// Created by gqhan on 12/9/25.
//


#include  <stdio.h>
#include  <stdlib.h>

#define EMPTY -1

/*
 * Yapi:  Node

 *  k deneme sonrasi chain'e eklenen elemanlar icin
 *           bagli liste dugumu
 *
 * Uyeler:
 *   userId - Kullanici ID'si
 *   next   - Sonraki dugumun adresi
 */

typedef struct Node {
    int userId ;
    struct Node *next;

}Node;

/*
 * Yapi: HashCell
 *  Hash tablosunun her bir hucresi 
 *
 * Uyeler:
 *   userId - Tabloya yerlesen eleman (-1 ise bos)
 *   head   - k deneme sonrasi eklenen elemanlarin listesi basi 
 */
typedef struct HashCell  {
    int userId ;
    Node *head;
}HashCell;

/*
 * Fonksiyon: isPrime
 *  Verilen sayinin  asal olup olmadigini kontrol eder
 *
 * Parametreler :
 *   n - Kontrol edilecek sayi
 *
 * Return:  
 *   1 -  Sayi asal ise
 *   0 - Sayi asal degil ise
 */
int isPrime(int n) 
{
    int i ;


    if (n % 2 == 0 || n % 3 == 0)
    {
         return 0;
    }
    // karekköğne kadar kontrol etmesi yeterli
    i = 5;
    while (i * i <=  n)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return 0;
        }
        i = i + 6;
    }

    return 1;
}

/*
 * Fonksiyon: findNextPrime
 *  Verilen  sayiya esit veya ondan buyuk
 *           en kucuk asal sayiyi bulur
 *
 * Parametre:
 *   n - Baslangic  sayisi
 *
 * Return:  n'e esit veya buyuk en kucuk asal sayi
 
 */
int findNextPrime(int  n)
{
    if (isPrime(n))
    {
        return n;
    }

    if (n % 2 ==  0)
    {
        n = n +  1;
    }

    while (isPrime(n) == 0)
    {
        n  = n + 2;
    }

    return n ;
}

/*
 * Fonksiyon: hashFunction
 *   Verilen  userId icin hash degerini hesaplar
 *           h(t)  =t mod m
 *
 * Parametreler: userId - Kullanici ID'si
 * m      - Hash  tablo boyutu
 *
 * Return:  Hash degeri  (0 ile m-1 arasi)
 */
int hashFunction(int userId ,  int m)
{
    return userId  % m;
}


/*
 * Fonksiyon:  createHashTable
 *  Dinamik bellek kullanarak hash tablosu olusturur
 *           ve tum hucreler i bos olarak isaretler
 *
 * Parametreler:
 *   m - Hash tablo boyutu
 *
 * Return: Olusturulan  hash tablosunun adresi
 */

HashCell* createHashTable(int m) {
    HashCell *table;
    int  i;

    //Dinamik bellek
    table = (HashCell *)malloc(m * sizeof(HashCell));
    if (table == NULL) {
        printf("Bellek  hatasi");
        
        return NULL;
    }

    i = 0;
    while (i < m) {
        table[i].userId =  EMPTY; // -1 bos
        table[i].head = NULL;
        i = i+1 ;

    }
    return  table ;
}

/*
 * Fonksiyon: freeChai
 * Bir hucredeki bagli listeyi (zinciri) serbest birakir
 *
 * Parametreler: 
 *   head - Zincirin  bas dugumu
 *
 */

void freeChain(Node * head) {
    Node *current;
    Node * next;
    current = head;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}


/*

 * Fonksiyon: freeHashTable
 *   Hash tablosunu ve tum zincirleri bellekten siler 
 *
 * Parametreler:
 *   table - Hash tablosu
 *   m     - Hash tablo boyutu 
 *
 * Reutn:
 */

void freeHashTable(HashCell *table, int m) { 
    int i;
    i = 0;
    while (i < m) {
        if (table[i].head != NULL) {
        	
            freeChain(table[i].head);
        }
        i = i + 1;
    }
    free(table);
}

/*
 * Fonksiyon: createNode
 *   Yeni bir bagli liste dugumu olusturur 
 *
 * Parametreler:
 *   userId - Kullanici ID'si 
 *
 * Return: Olusturulan dugumun  adresi
 */

Node * createNode(int userId) {
    Node * newNode;
    newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Bellek hatasi");
        return NULL;
    }
    newNode->userId = userId;
    newNode->next = NULL; 
    
    return newNode;
}


/*
 * Fonksiyon: addToChain 
 * : Bir hucrenin zincirinin sonuna yeni eleman ekler
 *
 * Parametreler:
 *   cell   - Ekleme yapilacak hash hucresi
 *   userId - Eklenecek kullanici ID'si 
 *
 * Return Yok
 */

void addToChain(HashCell *cell , int userId) {
    Node * current; 
    Node * newNode;
    newNode = createNode(userId);
    if (cell -> head == NULL) {
        cell-> head = newNode;
    } 
    else {
        current = cell -> head;
        while (current->next != NULL) {
            current = current->next; 

        }
        current->next = newNode;
    }
}


/*
 * Fonksiyon: insertHybrid
 *   Hibrit yontem ile eleman ekler
 *           1.  Oncelikle Linear Probing dener (maksimum k deneme)
 *           2. k deneme sonrasinda basarisiz olursa 
 *              orijinal indekse Separate Chaining ile ekler
 *
 * Parametreler:
 
 *   table  - Hash  
 *   m      - Hash tablo boyutu
 *   userId - Eklenecek kullanici ID'si
 *   k      - Maksimum Linear Probing deneme sayisi
 *
 *Return Ekleme icin yapilan deneme sayisi
 */

int insertHybrid(HashCell *table , int m , int userId, int k) {
    int originalIndex;
    int currentIndex ;
    int probeCount;
    

    //original hash 
    originalIndex =  hashFunction(userId, m);
    currentIndex = originalIndex ;
    probeCount = 0;

    // ASAMA ! :  Linear prbing k deneme
    while (probeCount < k) {
        probeCount = probeCount + 1 ;
        if (table[currentIndex].userId == EMPTY) {
            table[currentIndex].userId = userId;
            return probeCount;
            
        }
        /* Bir sonraki hucreye gec (dairesel) */
        currentIndex = (currentIndex + 1) % m; 
    }

    ///ASAMA 2Ç k denem doldu Chain yap
    addToChain(&table[originalIndex], userId); 

    //k deneme
    return k;
}

/*
 * Fonksiyon:  readInputFile
 * *   Input dosyasini okur ve kullanici ID'lerini
 *           dinamik bir diziye yukler
 *
 * Parametreler:
 *   fileName  - Okunacak dosyanin adi 
 *   dataCount - Okunan eleman sayisini dondurmek icin pointer
 *
 * Return Kullanici ID'lerini iceren dinamik dizi
 */

int* readInputFile(char *fileName, int *dataCount)
{
    FILE *file;
    int *data;
    int count;
    
    int i;

    /* Dosyayi ac */
    file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("Hata:  Dosya acilamadi:  %s\n", fileName);
        return NULL;
    } 
    

    /* Ilk satir:  eleman sayisi */
    fscanf(file, "%d", &count);


    /* Dinamik bellek ayir */
    data = (int*)malloc(count * sizeof(int));

    if (data == NULL)
    {
        printf("Hata:  Bellek ayrilamadi!\n");
        fclose(file);
        return NULL;
    }

    /* Verileri oku */
    i = 0;
    while (i < count)
    {
        fscanf(file, "%d", &data[i]);
        i = i + 1;
    }

    /* Dosyayi kapat */
    fclose(file);


    /* Eleman sayisini dondur */
    *dataCount = count;

    return data;
}
/*
 * Fonksiyon: runTestForK
 *  Belirli bir k degeri icin hash tablosunu olusturur,
 *           tum verileri ekler ve istatistikleri hesaplar
 *
 * Parametrler:
 *   data      - Kullanici ID'leri dizisi
 *   dataCount - Dizi eleman sayisi
 *   m         - Hash tablo boyutu
 *   k         - Linear Probing deneme sayisi
 *   minProbe  - Minimum deneme sayisini dondurmek icin pointer
 *   avgProbe  - Ortalama deneme sayisini dondurmek icin pointer
 *   maxProbe  - Maksimum deneme sayisini dondurmek icin pointer
 *
 * return
 */

void runTestFork(int *data, int dataCount, int m, int k ,
    int *minProbe, double *avgProbe, int *maxProbe) {
    HashCell *table;
    int *probeCounts;
    int probeResult;
    int i;

    int min;
    int max;
    double total;

    //hash tablosu
    table = createHashTable(m);


    probeCounts = (int*)malloc(dataCount * sizeof(int));
    if (probeCounts == NULL) {
        printf("Hata:  Bellek ayrilamadi!\n");
        freeHashTable(table, m);
        return;
    }



    i = 0;
    while (i < dataCount) {
        probeResult = insertHybrid(table, m , data[i] , k); //INSERT
        probeCounts[i] = probeResult;
        i = i + 1;
    }

    min = probeCounts[0];
    max = probeCounts[0];
    total = 0;

    i = 0;
    while (i < dataCount) {
        // max
        if (probeCounts[i] > max) {
            max = probeCounts[i];
        }
        if (probeCounts[i] < min) {
            min = probeCounts[i];
        }
        total += probeCounts[i];
        i = i + 1;

    }

    *minProbe = min;
    *avgProbe = total / dataCount;
    *maxProbe = max ;

    free(probeCounts);
    freeHashTable(table, m);

}

/*
 * Fonksiyon: printResults
 
 *  Bir k degeri icin sonuclari ekrana yazdirir
 *Parametreler: k        -Linear Probing deneme sayisi
 *   minProbe -Minimum deneme sayisi
 *   avgProbe -Ortalama deneme sayisi
 *   maxProbe -
 *   Maksimum deneme sayisi
 *
 * Return
 */


void printResults(int k , int minProbe , double avgProbe , int maxProbe) {
    printf("k = %d icin \n", k);
    
	//printf("Min : %d\n",minProbe);
    printf("ortalama  %.2f\n", avgProbe);
    printf("Max : %d\n", maxProbe);
    printf("\n");
}





/*
 * Fonksiyon:  main
 *   Programin ana fonksiyonu
 *           - Kullanicidan k degerini alir
 *           - Input dosyasini okur
 *           - Hash tablo boyutunu hesaplar
 *           - Tum k degerleri icin test yapar
 *           - Sonuclari ekrana yazdirir
 *
 * Parametreler:  Yok
 *
 * Donen Deger: 0 (basarili), -1 (hata)
 */
int main()
{
    /* Degisken tanimlari - ANSI-C uyumlu */
    int *data;
    int dataCount;
    int m;
    int k;
    int kValues[7];
    int minProbe;
    int maxProbe;
    double avgProbe;
    
    int i;

    /* Test edilecek k degerleri */
    kValues[0] = 1;
    kValues[1] = 5;
    kValues[2] = 10;
    kValues[3] = 25;
    kValues[4] = 100;
    kValues[5] = 500;
    kValues[6] = 1000;

    printf("k değeri giriniz: ");
    scanf("%d", &k);

    /* Input dosyasini oku */
    data = readInputFile("input2.txt", &dataCount);

    if (data == NULL)
    {
        return -1 ;
    }

    /* Hash tablo boyutunu hesaplama */
    /*  Kullanici sayisinin 10'a bolumune en yakin asal sayi */
    m = findNextPrime(dataCount / 10);


    printf("\n");
    printf("========================================\n");
    printf("HASH TABLOSU ANALIZI SONUCLARI\n");
    printf("================================\n");
    printf("Toplam kullanici sayisi: %d\n", dataCount);
    printf("Hash tablo boyutu (m):   %d\n", m);
    printf("=====================================\n\n");

    /*  k degeri testi */
    printf("--- Girilen k = %d icin Sonuc ---\n\n", k);
    runTestFork(data, dataCount, m, k, &minProbe, &avgProbe, &maxProbe) ;
    printResults(k, minProbe, avgProbe,maxProbe) ;

    /* Tum k degerleri icin test yapma */
    printf("-- Tum k Degerleri Icin Sonuclar --\n\n") ;


    i = 0;
    while (i < 7)
    {
        runTestFork(data, dataCount, m, kValues[i], &minProbe, &avgProbe, &maxProbe);
        printResults(kValues[i], minProbe, avgProbe, maxProbe);
        i = i + 1;
    }

    /* Bellegi temizle */
    free(data);

    printf("=================================\n");
    printf("Program basariyla tamamlandi.\n");
    printf("===============================\n");

    return 0;
}

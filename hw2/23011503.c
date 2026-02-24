//
// Created by gqhan on 1/9/26.
//

#include <stdio.h>
#include <stdlib.h>



typedef struct {
    int power;
    int heat;
}Server;


typedef struct {
    int totalPower;
    int totalHeat;
    int *servers;
    int serversNum;
}Cabin;


/*
* Fonksiyon: checkCapacity
* Amac: Sunucu eklenince kapasite asilir mi kontrol eder
* Parametreler:
*   - cabin: Kontrol edilecek kabin
*   - server: Eklenecek sunucu
*   - maxCapacity: Maksimum kapasite
* return: 1 = uyar , 0 = uymaz
*/
int checkCapacity(Cabin *cabin , Server *server , int maxCapacity) {

    int newTotal;
    newTotal = cabin->totalPower + server-> power;
    if (newTotal <= maxCapacity) {
        return 1;
    }

    return 0;
}


/*
 * Fonksiyon: checkHeatRatio
 * Amac:  Sunucu eklenince isi orani K degerini asir mi kontrol eder
 * Parametreler:
 *   - cabin: Kontrol edilecek kabin
 *   - server: Eklenecek sunucu
 *   - kValue: Sogutma verimliligi esigi
 * return: 1 = uygun,  0 = uygun degil
 */
//
// int checkHeatRatio(Cabin *cabin , Server *server, float kValue) {
//     int newTotalHeat = 0;
//     int newTotalPower = 0;
//     float ratio;
//
//     newTotalPower = cabin->totalPower + server->power;
//     newTotalHeat = cabin->totalHeat + server-> heat;
//
//     if (newTotalPower == 0) {
//         return 1;  /* Kabin boş, sunucu eklenebilir */
//     }
//
//     ratio = (float)newTotalHeat/ newTotalPower;
//
//     if (ratio <= kValue) {
//         return 1;
//
//     }
//     return 0;
// }

/*
 * Fonksiyon: checkConflict
 * Amac:  Sunucu kabindeki diger sunucularla cakisiyor mu kontrol eder
 * Parametreler:
 *   - serverIdx: Eklenecek sunucunun indeksi
 *   - cabin: Kontrol edilecek kabin
 *   - conflictMatrix: Cakisma matrisi
 * Return: 1 = cakisma yok , 0 = cakisma var
 */
int checkConflict(int serverIdx , Cabin *cabin, int **conflictMatrix) {
    int i;
    int existingServer;

    for (i = 0; i < cabin->serversNum; i++) {
        existingServer = cabin->servers[i];
        if (conflictMatrix[serverIdx][existingServer] ==1)
            return 0;
    }
    return 1;
}

/*
 * Fonksiyon:  addServer
 * Amac: Sunucuyu kabine ekler
 * Parametreler:
 *   - cabin:  Sunucunun eklenecegi kabin
 *   - server:  Eklenecek sunucu
 *   - serverIdx: Sunucunun indeksi
 * Return: void
 */

void addServer(Cabin *cabin , Server *server , int serverIdx) {
    cabin->totalPower += server-> power;
    cabin->totalHeat += server-> heat;
    cabin->servers[cabin->serversNum++] = serverIdx;
}

/*
 * Fonksiyon: removeServer
 * Amac:  Sunucuyu kabinden cikarir (backtrack icin)
 * Parametreler:
 *   - cabin: Sunucunun cikarilacagi kabin
 *   - server: Cikarilacak sunucu
 * return: void
 */

void removeServer(Cabin *cabin , Server *server) {
    cabin->totalPower -= server-> power;
    cabin->totalHeat -= server-> heat;
    cabin->serversNum--;

}



/*
 * Fonksiyon:  printSolution
 * Amac:  Bulunan cozumu ekrana yazdirir
 * Parametreler:
 *   - cabins:  Kabin dizisi
 *   - cabinNum: Kabin sayisi
 * return: void
 */
void printSolution(Cabin *cabins, int cabinNum) {
    int i, j;
    for (i= 0 ; i < cabinNum ; i++) {
        printf("Cabin%d: " , i+1);
        if (cabins[i].serversNum == 0) {
            printf("Empty\n");
        }else {
            for (j = 0 ; j < cabins[i].serversNum ; j++) {
                printf("S%d", cabins[i].servers[j] + 1);
            }

        }
        printf("\n");
    }


}




/*
 * Fonksiyon: backtrack
 * Amac:  Tum sunuculari kabinlere yerlestirmeye calisir
 * Parametreler:
 *   - serverIdx: Siradaki sunucu indeksi
 *   - servers: Sunucu dizisi
 *   - serverNum: Toplam sunucu sayisi
 *   - cabins: Kabin dizisi
 *   - cabinNum: Kabin sayisi
 *   - conflictMatrix: Cakisma matrisi
 *   - maxCapacity: Kabin kapasitesi
 *   - kValue: Isi orani esigi
 * return : void
 */

void backtrack(int serverIdx, Server *servers, int serverNum, Cabin *cabins,
    int cabinNum, int **conflictMatrix, int maxCapacity, float kValue,int *found)
{
    int i ,j ;
    int valid =0;
    float ratio;

    if (*found) {
        return;
    }

    if (serverIdx == serverNum) {
        valid = 1;
        for (j = 0 ; j < cabinNum ; j++) {
            if (cabins[j].serversNum > 0) {

                ratio = (float)cabins[j].totalHeat / cabins[j].totalPower;
                if (ratio > kValue) {
                    valid = 0;
                }
            }
        }
        if (valid) {
            printf("*******Answer is founded******\n");
            printSolution(cabins,cabinNum);
            *found = 1;

        }
        return;
    }
    for (i = 0; i < cabinNum; i++) {
        //3 control capacity , heat , 
        if (checkCapacity(&cabins[i], &servers[serverIdx], maxCapacity) &&
            checkConflict(serverIdx,&cabins[i], conflictMatrix))
        {

            addServer(&cabins[i] , &servers[serverIdx], serverIdx);

            //backtrack
            backtrack(serverIdx +1, servers, serverNum, cabins, cabinNum,conflictMatrix,
                maxCapacity, kValue,found);


            removeServer(&cabins[i], &servers[serverIdx]);
        }
    }
    // bu sunucuyu atla
    backtrack(serverIdx + 1, servers, serverNum, cabins, cabinNum,
              conflictMatrix, maxCapacity, kValue,found);
}






int main(void) {
    int serverNum;
    int cabinNum;


    int maxCapacity;
    float kValue;

    int **conflictMatrix;
    int conflictNum;
    int s1, s2;

    int found = 0;
    int i,j;

    Server *servers;
    Cabin *cabins;

    printf("SERVERS ALLOCATION\n");


    printf("Servers number:");
    scanf("%d", &serverNum);

    printf("Cabin number:");
    scanf("%d", &cabinNum);

    printf("Cabin watt Capacity:");
    scanf("%d", &maxCapacity);


    printf("k value:");
    scanf("%f", &kValue);


    /*SERVERS part*/
    servers = (Server *)malloc(sizeof(Server) * serverNum);
    if (servers == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    /*Get the server informantion */
    printf("\n Server infos: \n");
    // servers[0].power = 60;
    // servers[0].heat = 120;
    // servers[1].power = 50;
    // servers[1].heat = 40;
    // servers[2].power = 40;
    // servers[2].heat = 30;
    // servers[3].power = 30;
    // servers[3].heat = 60;
    for ( i = 0; i < serverNum; i++) {
        printf("\n Server[%d] Power: ",i+1);
        scanf("%d", &servers[i].power);
        printf("Server[%d] Heat: ",i+1);
        scanf("%d", &servers[i].heat);

    }


    //CABİNS
    cabins = (Cabin *)malloc(sizeof(Cabin) * cabinNum);
    if (cabins == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    /*Get the cabin informantion */
    for ( i = 0; i < cabinNum; i++) {
        cabins[i].totalHeat = 0;
        cabins[i].totalPower = 0;
        cabins[i].serversNum = 0;

        cabins[i].servers = (int*)malloc(sizeof(int) * serverNum);
            if (cabins[i].servers == NULL) {
                printf("Bellek ayirma hatasi!\n");
                free(cabins);
                free(servers);
                return 1;

            }
    }

    // Conflick matris 2D
    conflictMatrix = (int**)malloc(serverNum * sizeof(int*));
    if (conflictMatrix == NULL) {
        printf("Bellek ayirma hatasi!\n");
        free(cabins);
        free(servers);
        return 1;
    }
    for (i = 0; i < serverNum; i++) {
        conflictMatrix[i] = (int*)calloc(serverNum, sizeof(int));
        if (conflictMatrix[i] == NULL) {
            printf("Bellek ayirma hatasi!\n");
            return 1;
        }
    }

    /* Cakisma bilgilerini al */
    printf("\n--- Conflickt info ---\n");
    printf("Conflict num: ");
    scanf("%d", &conflictNum);

    for (i = 0; i < conflictNum; i++) {
        printf("Conflict %d (S1 S2): ", i + 1);
        scanf("%d %d", &s1, &s2);

        /* Matrise kaydet - simetrik */
        conflictMatrix[s1-1][s2-1] = 1;
        conflictMatrix[s2-1][s1-1] = 1;
    }

	printf("\n ---Conflict Matrix ---\n");
	printf("  ");
	for(j = 0 ; j < serverNum ; j++){
		printf("S%d ", j+1);
		
	}
	printf("\n");
	for(i = 0; i < serverNum ; i++){
		printf("S%d", i+1);
		for(j = 0; j < serverNum; j++){
			printf(" %d ", conflictMatrix[i][j]);
		}
		printf("\n");
	}

    printf("\n --- Answers  --- \n");
    backtrack(0 , servers, serverNum , cabins, cabinNum,
        conflictMatrix, maxCapacity, kValue, &found);



    /* Bellek temizle */
    for (i = 0; i < serverNum; i++) {
        free(conflictMatrix[i]);
    }
    free(conflictMatrix);

    for (i = 0; i < cabinNum; i++) {
        free(cabins[i]. servers);
    }
    free(cabins);

    free(servers);




    return 0;
}

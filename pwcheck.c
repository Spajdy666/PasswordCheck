/** @file pwcheck.c */
/** @author Filip Spacek, xspace38 */
/** @date 27.10.2021 */

#include <stdio.h> //knihovna umoznujici nam pracovat se stdin a stdout
#include <stdlib.h> //knihovna, diky ktere se muze alokovat pamet, kontrolovat procesy a konvertovat string na jiny datovy typ
#include <stdbool.h> //knihovna, diky ktere muzeme pouzivat bool, ktery ma navratovou hodnotu true/false

/** @brief funkce pro detekci velkeho pismene */
bool isUpper(char ch) {
    return (ch >= 'A' && ch <= 'Z'); //vraci velka pismena
}

/** @brief funkce pro detekci maleho pismene */
bool isLower(char ch) {
    return (ch >= 'a' && ch <= 'z'); //vraci mala pismena
}

/** @brief funkce pro detekci cislice */
bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9'); //vraci cislice
}

/** @brief funkce pro detekci vytisknutelnych ASCII znaku s ordinalni hodnotou 32 (mezera) - 126 (vlnovka), mimo velka, mala pismena a cisla */
bool isAscii(char ch) {
    return(ch >= ' ' && ch <= '~') && !isUpper(ch) && !isLower(ch) && !isDigit(ch); //vraci ascii znaky (mimo isUpper, isLower, isDigit, protoze v ukolu dva takto mohla vzniknout kolize skupin)
}

/** @brief funkce pomocna pro detekci maleho pismene ve stringu */
bool ContainsStringLower(char buffer[]) {
    for (int i = 0; buffer[i] != '\0'; i++) { // cyklus pro pruchod celym stringem
        if (isLower(buffer[i])) { //pokud plati podminka isLower pro buffer[i], vrati true
            return true;
        }
    }
    return false;
}

/** @brief funkce pomocna pro detekci velkeho pismene ve stringu */
bool ContainsStringUpper(char buffer[]) {
    for (int i = 0; buffer[i] != '\0'; i++) { // cyklus pro pruchod celym stringem
        if (isUpper(buffer[i])) { //pokud plati podminka isUpper pro buffer[i], vrati true
            return true;
        }
    }
    return false;
}

/** @brief funkce pomocna pro detekci cislice ve stringu */
bool ContainsStringDigit(char buffer[]) {
    for (int i = 0; buffer[i] != '\0'; i++) { // cyklus pro pruchod celym stringem
        if (isDigit(buffer[i])) { //pokud plati podminka isDigit pro buffer[i], vrati true
            return true;
        }
    }
    return false;
}

/** @brief funkce pomocna pro detekci ASCII znaku ve stringu, vraci true pokud nalezne ascii znak, v opacnem pripade false */
bool ContainsStringASCII(char buffer[]) {
    for (int i = 0; buffer[i] != '\0'; i++) { // cyklus pro pruchod celym stringem
        if (isAscii(buffer[i])) { //pokud plati podminka isAscii pro buffer[i], vrati true
            return true;
        }
    }
    return false;
}


/** @brief funkce pro uroven bezpecnosti 1, vraci pravdivostni hodnotu ze dvou pomocnych funkci na detekci maleho a velkeho pismene */
bool Level1Check(char buffer[], int param) {
    return ContainsStringLower(buffer) && ContainsStringUpper(buffer);
}


/** @brief funkce pro uroven bezpecnosti 2, rozhodovaci tabulka, vraci level_ret, ktery musi byt vetsi nebo roven parametru */
/**pricita jednicku do level_ret (v pripade ze obsahuje znak z 1 skupiny), vraci level_ret, ktery musi byt vice nez param*/
bool Level2Check(char buffer[], int param) {
    int level_ret = 0;
    level_ret += ContainsStringLower(buffer) ? 1 : 0; //pravda/nepravda o obsahu maleho pismene
    level_ret += ContainsStringUpper(buffer) ? 1 : 0; //pravda/nepravda o obsahu velkeho pismene
    level_ret += ContainsStringDigit(buffer) ? 1 : 0; //pravda/nepravda o obsahu cislice
    level_ret += ContainsStringASCII(buffer) ? 1 : 0; //pravda/nepravda o obsahu ASCII znaku
    return level_ret >= param;
}

//FIXME
/** @brief funkce pro uroven bezpecnosti 3*/
bool Level3Check(char buffer[], int param) {
    int count_same = 1; //pomocna promenna pro pocet stejnych znaku
    char last = 0; //pomocna promenna pro posledni znak
    for (int i = 0; buffer[i] != '\0'; i++) {
        char ch = buffer[i];
        if (ch == last) {
            ++count_same; //pocitadlo stejnych znaku
        }
        else {
            count_same = 1; //pocet jednoho stejneho znaku, v pripade ze nenajde shodu s jinym
        }
        if (count_same >= param) { //pokud najde shodu param a vice znaku, vrati false
            return false;
        }
        last = ch;
    }
    return true; //pokud nenajde shodu (stejnych) param a vice znaku, vrati true
}

/** @brief funkce nahrazujici strlen z knihovny string.h, dokud se index ve stringu nerovna '\0', pocita, vraci index (pocet znaku) */
unsigned int myStrlen(char string[]) {
    int index = 0; //ve stringu zaciname pozici nula
    while (string[index] != '\0') { //cyklus kontrolujici chod, dokud se string nerovna '\0'
        index++; //pocitadlo indexu, pricita se dokud nenarazi na konec stringu
    }
    return index;
}


/** @brief pomocna funkce pro kontrolu rovnosti stringu, pokud plati, vraci true */
bool myStrEqual(char string1[], int string1_length, char string2[], int string2_length) {
    //podminka, kontrolujici delku dvou stringu, pokud se nerovnaji, vrati false
    if (string1_length != string2_length) {
        return false;
    }
    //cyklus prochazejici stringem po pozicich i, za predpokladu, ze se delka stringu rovna, pokud se indexy stringu nerovnaji, vraci false
    for (int i = 0; i < string1_length; i++) {
        if (string1[i] != string2[i]) {
            return false;
        }
    }
    return true;
}

/** @brief funkce pro uroven bezpecnosti 4, kontorluje jestli substringy o delce param a vice se nerovnaji (true), pokud se rovnaji vraci false*/
bool Level4Check(char buffer[], int param) {
    int length = myStrlen(buffer); //pomocna promenna hlidajici delku bufferu
    int length_max = length - param; //pomocna promenna pro max delku
    char *ptr = 0; //pointer
    for (int i = 0; i < length_max; i++) { //cyklus pro posun posouvani 'i' po znacich v bufferu do maximalni delky
        ptr = buffer + i; //posouvani pointeru pomoci bufferu a promenne 'i'
        for (int y = i + param; y < length_max; y++) { //posouva pointer
            if (myStrEqual(ptr, param, buffer + y, param)) { //volani funkce pro kontrolu substringu ptr a buffer + y o delce param, vrati false pri rovnosti
                return false;
            }
        }

    }
    return true;
}


/** @brief pomocna funkce pro pruchod urovnemi bezpecnosti, na konci tiskne vysledek na std output */
void FinalLevelCheck(char buffer[], int level, int param) {
    bool bPass = true; //pomocna promenna nastavena na true
    if (level < 1 && level > 4) { //podminka kontrolujici spravne zadani levelu
        fprintf(stderr, "Spatne zadana uroven bezpecnosti (level).\n");
    }else {
        /** postupny pruchod urovnemi */
        if (level >= 1 && param >= 1) {
            bPass &= Level1Check(buffer, param);
        }
        if (bPass && (level >= 2 && param >= 1)) {
            bPass &= Level2Check(buffer, param);
        }
        if (bPass && (level >= 3 && param >= 1)) {
            bPass &= Level3Check(buffer, param);
        }
        if (bPass && (level == 4 && param >= 1)) {
        bPass &= Level4Check(buffer, param);
        }
        /**tisk vysledku na vystup */
        if (bPass)
            fprintf(stdout, "%s\n", buffer);
    }
}

/** @brief funkce main, vraci -1 pri erroru vstupu, vraci nulu pri uspesnem dokonceni */
int main(int argc, char* argv[]) {
    int level = -1;
    int param = -1;
    char buffer[101];
    //kontrola argumentu
    if (argc == 3) {
        //konverze stringu na datovy typ integer za pomoci atoi funkce
        //argv[0] zadavame spusteni souboru, argv[1] zadavame uroven bezpecnosti, argv[2] zadavame parametr
        level = atoi(argv[1]);
        param = atoi(argv[2]);
        //kontrola zadavani levelu
        if (level <= 0 || level >= 5) {
            fprintf(stderr, "Spatne zadany level.\n");
            return -1;
        }
        //kontrola zadavani parametru
        if (param <= 0) {
            fprintf(stderr, "Spatne zadany parametr.\n");
            return -1;
        }

    //zavedeni programu s hesly na stdin
        while (fgets(buffer, 101, stdin) != NULL) {
            //kontrola zadavani validniho stringu
            int length = myStrlen(buffer);
            if (buffer[length - 1] == '\n') {
                buffer[length - 1] = '\0';
            }
            else if (!feof(stdin)) {
                fprintf(stderr, "Zadan nevalidni string.\n");
                return -1;
            }
            //volani funkce pro kontrolu urovni a tisku vysledku na vystup
            FinalLevelCheck(buffer, level, param);
        }
        } else {
        //chybove hlaseni pri spatnem zadani argumentu
        fprintf(stderr, "Chyba, neplatny pocet argumentu.\n");
        return -1;
        }
    return 0;
}
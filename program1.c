#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

void readCompleteCardLog(char file_name[]);
void readCatalog(char file_name[], char output_file_name[]);
void computeMaxProfit(char output_file_name[]);

typedef struct {
    char card_name[200];
    int market_price;
}card;

typedef struct {
    char card_name[200];
    int given_price;
}available_card;

card cards[200] = {};
available_card available_cards[200] = {};
int numberOfTotalCard;
int numberOfTotalCardinCatalog;
int amountAvailable;
bool overflow = false;
int currentPrice = 0;
clock_t start, end;
double cpu_time_used;
int totalPurchasedCards;
FILE * fp;

// Taking argument as command line
int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("enter 3 arguments only");
        return 0;
    }

    char market_price[200];
    char price_list[200];
    char output_file[200];
    strcpy(market_price, argv[1]);
    strcpy(price_list, argv[2]);
    strcpy(output_file, argv[3]);

    //printf("Market price file : %s \n", market_price);
    //printf("Price list file : %s \n", price_list);

    readCompleteCardLog(market_price);
    //printf("------------------------------------------------------\n");
    readCatalog(price_list, output_file);
    //printf("------------------------------------------------------\n");

    fclose (fp);
    return 0;
}

void readCompleteCardLog(char file_name[])
{
    FILE *ptr_File = fopen (file_name,"r");

    char line[255];
    fgets (line, 255, ptr_File);
    numberOfTotalCard = atoi(line);
    //printf("Number of cards in deck : %d\n", numberOfTotalCard);

    while( feof( ptr_File ) == 0)
    {
      for (int i = 0; i < numberOfTotalCard; i++)
      {
          fgets (line, 255, ptr_File);
          char *token = strtok(line, " ");

          strcpy(cards[i].card_name, token);
          token = strtok(NULL, " ");
          cards[i].market_price = atoi(token);

          //printf("Card name : %s\t", cards[i].card_name);
          //printf("price : %d\n", cards[i].market_price);
      }

    }
    fclose(ptr_File);
}

void readCatalog(char file_name[], char output_file_name[])
{
    FILE *ptr_File = fopen (file_name,"r");

    char line[255];

    while( feof( ptr_File ) == 0)
    {
      fgets (line, 255, ptr_File);

      char *token = strtok(line, " ");
      numberOfTotalCardinCatalog = atoi(token);

      token = strtok(NULL, " ");
      amountAvailable = atoi(token);

      //printf("Number of cards in catalog : %d\n", numberOfTotalCardinCatalog);
      //printf("Amount available to purchase : %d\n", amountAvailable);

      for (int i = 0; i < numberOfTotalCardinCatalog; i++)
      {
          fgets (line, 255, ptr_File);
          char *token = strtok(line, " ");

          strcpy(available_cards[i].card_name, token);
          token = strtok(NULL, " ");
          available_cards[i].given_price = atoi(token);

          //printf("Card name : %s\t", available_cards[i].card_name);
          //printf("price : %d\n", available_cards[i].given_price);
      }

      computeMaxProfit(output_file_name);

      printf("------------------------------------------------------\n");

    }
    fclose(ptr_File);
}

void computeMaxProfit(char output_file_name[])
{
    fp = fopen (output_file_name,"a");
    start = clock();
    //printf("Processing start :%s\n", output_file_name);

    //printf("Amount available : %d\n", amountAvailable);
    fprintf (fp, "Number of cards in price list : %d\n", numberOfTotalCardinCatalog);
    printf("Number of cards in price list : %d\n", numberOfTotalCardinCatalog);
    int profit = 0;
    int permutable[numberOfTotalCardinCatalog];
    int card_details[numberOfTotalCardinCatalog];
    int purchasedCards = 0;
    int maxValue = 0;
    totalPurchasedCards = 0;

    /* 1st check*/
    int sum = 0;

    for(int i = 0; i < numberOfTotalCardinCatalog; i++){
      sum = sum + available_cards[i].given_price;
    }

    if(sum < amountAvailable){
      for(int i = 0; i < numberOfTotalCardinCatalog; i++){
        for(int j = 0; j < numberOfTotalCard; j++){
          if(!strcmp(cards[j].card_name,available_cards[i].card_name)){
            maxValue = maxValue + (cards[j].market_price - available_cards[i].given_price);
            totalPurchasedCards++;
            card_details[i] = 1;
          }
        }
      }
    }else{
      /*2nd check*/
      //reset(permutable);
      int temp = 0;
      overflow = false;
      while (temp < numberOfTotalCardinCatalog)
      {
        permutable[temp] = 0;
        temp++;
      }


      while (!overflow)
      {
        for (int i = 0; i < numberOfTotalCardinCatalog; i++){
          if(permutable[i]  == 1){
              currentPrice = currentPrice + available_cards[i].given_price;

              if (currentPrice <= amountAvailable){

                for(int j = 0; j < numberOfTotalCard; j++){
                  if(!strcmp(cards[j].card_name, available_cards[i].card_name)){
                    profit = profit + (cards[j].market_price - available_cards[i].given_price);
                  }
                }

                if(profit > maxValue)
                {
                    maxValue = profit;
                }
              }
              else{
                profit = 0;
              }

          }
        }
        if(profit == maxValue){
          for(int i = 0; i < numberOfTotalCardinCatalog; i++){
            if(permutable[i] == 1) {
              purchasedCards++;
              card_details[i] = 1;
            }
          }
          totalPurchasedCards = purchasedCards;
        }

        currentPrice = 0;
        profit = 0;
        purchasedCards = 0;

        if (!overflow) {
          int i = numberOfTotalCardinCatalog - 1;
          overflow = true;
          while ((i >= 0) && (overflow))
          {
              if (permutable[i] == 1)
              {
                  permutable[i] = 0;
              }
              else
              {
                  permutable[i] = 1;
                  overflow = false;
              }
              i--;
          }
        }
      }
    }
    end = clock();
    cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
    fprintf (fp,"Total purchase cards : %d \n", totalPurchasedCards);
    printf("Total purchase cards : %d \n", totalPurchasedCards);
    fprintf (fp,"profit : %d \n", maxValue);
    printf("profit : %d \n", maxValue);
    fprintf (fp,"Time taken : %f \n", cpu_time_used);
    printf("Time taken : %f \n", cpu_time_used);
    for(int i = 0; i < numberOfTotalCardinCatalog; i++){
      if(card_details[i] == 1) {
        printf("%s\n", available_cards[i].card_name);
        fprintf (fp, "%s\n", available_cards[i].card_name);
      }
    }
    fprintf (fp, "-----------------------------------\n");
}

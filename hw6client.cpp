
// ecs36b first program

#include <iostream>
#include "ecs36b_Exception.h"
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string>
#include <cstring>

// JSON RPC part
#include "hw6client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

#define WORD_LENGTH 5
#define ALPHA_SIZE 27


FILE * fpAll,  * fpSol;

//Selected word from solution list
char word[WORD_LENGTH + 1] = {0};

//the word from list.
char eachWord[WORD_LENGTH + 1] = {0};
//correct letter with wrong position.
pair<char, int> wrong[WORD_LENGTH + 1];
// 0 wrong letter, 1 possible right letter.
int exist[ALPHA_SIZE];






int
main()
{
  // distributed wordle demo for 11/23/2022
  // HttpClient httpclient("http://127.0.0.1:8384");
  //HttpClient httpclient("https://2ada-73-66-168-157.ngrok.io");
  //HttpClient httpclient("https://5f43-2601-200-c001-9ff0-d5cd-ca2a-e2c4-2fcc.ngrok.io");
  HttpClient httpclient("https://ce56646b42df.ngrok.io");

  hw6Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value myv;
  Json::Value jv_list;
  
  jv_list[0] = "921425191";
  jv_list[1] = "919378742";
  jv_list[2] = "919628476";
  
  try {
    myv = myClient.set_name("set_name", jv_list, "24H Study Room");
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
  // std::cout << myv.toStyledString() << std::endl;



  // Start Game
  int numberOfSucceed = 0;

  for(int Z = 0; Z < INT_MAX; Z ++)
  {

    // obtain and new
    try {
      myv = myClient.obtain("obtain", "Wordle", "00000000");
    } catch (JsonRpcException &e) {
      cerr << e.what() << endl;
    }
    //std::cout << myv.toStyledString() << std::endl;

    // First three guess
    if ((myv["game_id"].isNull() == false) && (myv["game_id"].isString() == true))
    {
      try {
      // earth
      myv = myClient.guess("guess", "Wordle", myv["game_id"].asString(), "earth");
      } 
      catch (JsonRpcException &e) {
      cerr << e.what() << endl;
      }

      try {
      // mound
      myv = myClient.guess("guess", "Wordle", myv["game_id"].asString(), "mound");
      } 
      catch (JsonRpcException &e) {
      cerr << e.what() << endl;
      }

      try {
      // sicky
      myv = myClient.guess("guess", "Wordle", myv["game_id"].asString(), "sicky");
      } 
      catch (JsonRpcException &e) {
      cerr << e.what() << endl;
      }
      //std::cout << myv.toStyledString() << std::endl;
    }


    fpAll = fopen("lists/ALL.TXT", "r");

    map<string, int> allWords;
    map<char, int> rightLetter;

    fseek(fpAll, 0, SEEK_SET);
    while(fgets(eachWord, sizeof(eachWord), fpAll) != NULL)
    {
      allWords[eachWord] = 1;
      fgetc(fpAll);
    }

    //puts("1. stop here !!");

    // ###Get all words in a map;
    // int i = 1;
    // for(auto x: allWords)
    // {
    //   cout << i << " .  :    " <<  x.first << "     :      " << x.second << endl;
    //   i ++;
    // }

    for(int i = 0; i < ALPHA_SIZE; i ++) exist[i] = 1;
    for(int i = 0; i < 5; i ++) wrong[i] = {'?', -1};

    string ans = "?????";
    string result;
    string guessedWord;


    for(int i = 0; i < 3; i ++)
    {
      result = myv["guesses"]["data"][i]["result"].asString();
      guessedWord = myv["guesses"]["data"][i]["guess"].asString();
      allWords[guessedWord] = 2;

      if(result == "#####")
      {
        try {
          myv = myClient.submit("submit", myv["game_id"].asString(), "24H Study Room");
        } catch (JsonRpcException &e) {
        cerr << e.what() << endl;
        }
        // std::cout << myv.toStyledString() << std::endl;
      }

      for(int j = 0; j < 5; j ++)
      {
        if(result[j] == '#') 
        {
          ans[j] = guessedWord[j];
          rightLetter[guessedWord[j]] = 1;
        }

        if(result[j] == 'o') 
        {
          for(int ii = 0; ii < 5; ii ++)
          {
            if(wrong[ii].first == '?')
            {
              wrong[ii] = {guessedWord[j], j};
            }
          }
        }

        if(result[j] == '_') exist[guessedWord[j] - 'a'] = 0;
      }
      // cout << result << " " << guessedWord << endl;
    }

    // puts("2. stop here !!");
    int k = 3;
    while(k != 6 && result != "#####")
    {  

      //remove the words with letter that is not exist.
      //remove the words with wrong letter at confirmed position.
      //remove the words with right letter at wrong position.
      for(auto &x: allWords) 
      {
        for(int i = 0; i < 5; i ++)
        {
          if(x.second == 1)
          {
            if(exist[x.first[i] - 'a'] == 0 && !rightLetter.count(x.first[i])) x.second = 2;
            if(ans[i] != '?' && x.first[i] != ans[i] && !rightLetter.count(x.first[i])) x.second = 2;
          }
        }

        for(int i = 0; i < 5; i ++)
        {
          if(wrong[i].first != '?' && wrong[i].first == x.first[wrong[i].second]) x.second = 2;
        }

        //if(x.second == 2) cout << x.first << endl;
      }

      //Start guessing
      for(auto &x: allWords)
      {
        if(x.second == 1)
        {
          x.second = 2;
          //cout << "???????  " << x.first << " " << x.second << endl;

          try {
          myv = myClient.guess("guess", "Wordle", myv["game_id"].asString(), x.first);
          } 
          catch (JsonRpcException &e) {
          cerr << e.what() << endl;
          }

          break;
        }
      }


      //cout << "### " << k << endl;
      //std::cout << myv.toStyledString() << std::endl;

      result = myv["guesses"]["data"][k]["result"].asString();
      guessedWord = myv["guesses"]["data"][k]["guess"].asString();
      for(int j = 0; j < 5; j ++)
      {
        if(result[j] == '#') 
        {
          ans[j] = guessedWord[j];
          rightLetter[guessedWord[j]] = 1;
        }

        if(result[j] == 'o') 
        {
          for(int ii = 0; ii < 5; ii ++)
          {
            if(wrong[ii].first == '?')
            {
              wrong[ii] = {guessedWord[j], j};
            }
          }
        }

        if(result[j] == '_') exist[guessedWord[j] - 'a'] = 0;
      }

      // cout << "find words left in map: " << endl;
      // int idx = 1;
      // for(auto x: allWords)
      // {
      //   if(x.second == 1)
      //   {  
      //     cout << idx << " .  :    " <<  x.first << "     :      " << x.second << endl;
      //     idx ++;
      //   }
      // }
      // puts(" ");
      
      k ++;
    }

    //std::cout << myv.toStyledString() << std::endl;

    // for(int i = 0; i < 27; i ++) cout << i << " exist: " << exist[i] << endl;
    cout << "game: " << Z << ". Stop here !!" << endl;
    // cout << "Number of guess: " << k << endl;
    // cout << "Result: " << result << " <==> " << ans << endl;
    // puts("");

    if(k < 7 && result == "#####")
    {
      try {
        myv = myClient.submit("submit", myv["game_id"].asString(), "24H Study Room");
      } catch (JsonRpcException &e) {
      cerr << e.what() << endl;
      }
      std::cout << myv.toStyledString() << std::endl;

      ++ numberOfSucceed;
      //puts(" Succeed !!!");
      //cout << "Number of succeed: " << numberOfSucceed << endl;
    }
  }

  //cout << endl << "Total " << numberOfSucceed << " times SUCCEEDED in 6 guesses!!!\n" << endl;

  return 0;
  
}



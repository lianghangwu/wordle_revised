

############
My program uses a map to store all words in ALL.txt. Set all value equals 1.
After the first three guesses, it will:
1. remove the words with letter that is not existing.
2. remove the words with right letter at wrong position.
3. remove the words with wrong letter at confirmed position.
Then set the value of those wrong words equals 2 in the map.
After that, it keeps guessing and updating the map. 
When find the right word and the number of guesses is under 6, it will sumbmit the game.

All of procedures above will loop 10 times. At the end the program will tell you how
many times it successfully guesses the right word in 6 guesses.








This is the reference implementation for HW#6. Please see the ecs36b_hw6.json for the input arguments for all four functions --
(1) obtain    - obtain a new or existing game (and obtain a valid game_id)
(2) guess     - provide a guess to a particular game_id
(3) set_name  - select a name for your group (input: student IDs)
(4) submit    - after completing a game, submit the game_id with your team name

IMPORTANT:
During our classes, we will use the following ngrok link --
https://2ada-73-66-168-157.ngrok.io

https://5f43-2601-200-c001-9ff0-d5cd-ca2a-e2c4-2fcc.ngrok.io

I.e., in hw6client.cpp, you should have --
HttpClient httpclient("https://5f43-2601-200-c001-9ff0-d5cd-ca2a-e2c4-2fcc.ngrok.io");

If you have any question about the examples, please feel free to ask the instructor.





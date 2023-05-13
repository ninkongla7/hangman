//CSCI1300
//Author: Nin Kongla
//Recitation 313
//Final Project


/*This code is for building a Hangman Game.
Player 1(computer) generates a word from a text 
file for player 2(user) to guess. Player 2 has
7 wrong guesses to make before the game is over.
In each wrong guess the diagram of the hangman
is being drew. If the player repeats the same wrong 
letter more than once, that wouldn't count as a wrong guess.
The program will print out a warning message instead.
If the diagram is being completed before the
player guesses right the game is over.
The program gives a little hint for a player 
by representing each word as a dash line, so
the player knows how many letters are in the word.
If the enter letter is in the word, it will replace
the blank dash line.
Either the player is at the state of winning or losing, the 
program will then ask if they want to play again. 
*/


//Credits (Book References):
//Dawson, Michael. Beginning C++ through Game Programming. Cengage Learning, 2010. 
//Rao, Siddhartha. Sams Teach Yourself C++ in One Hour a Day. 
//McGrath, Mike. C++ Programming C++ essentials demystified! 5th Edition.

//Youtube Video: 
//“[C++ Tutorial] Hangman Tutorial (in under an Hour).” YouTube, YouTube, 27 June 2019, www.youtube.com/watch?v=afBUqeSeb-I&t=504s. 



#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

class DrawShape //Class Draw Diagram
{
private: 
	int misses;
public:
	DrawShape()                               //create a constructor
	{
		misses = 0;
	}
	int getMisses()                          //method to get number of misses
	{
		return misses;
	}
	void setMisses(int n)					 //method to set misses
	{
		if (n > 7 )
			cout << "\nMisses cannot be greater than 7\n";
		else
			misses = n;
	}
	void draw()								 //method to draw hangman
	{
		cout << "\n\n";
		switch (misses)
		{
		case 1:
			cout << "__________________" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|________________|" << endl;
			break; 							//prevent fall through, it keeps going until it hits a break
		case 2:
			cout << "__________________" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       ( )      |" << endl;
        	cout << "|________________|" << endl;
			break;
		case 3:
			cout << "__________________" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       ( )      |" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|________________|" << endl;
			break;
		case 4:
			cout << "__________________" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       ( )      |" << endl;
        	cout << "|       /|       |" << endl;
        	cout << "|      / |       |" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|________________|" << endl;
			break;
		case 5:
			cout << "__________________" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       ( )      |" << endl;
        	cout << "|       /|\\      |" << endl;
        	cout << "|      / | \\     |" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|________________|" << endl;
			break;
		case 6:
			cout << "__________________" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       ( )      |" << endl;
        	cout << "|       /|\\      |" << endl;
        	cout << "|      / | \\     |" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       /        |" << endl;
        	cout << "|      /         |" << endl;
        	cout << "|________________|" << endl;
			break;
		case 7:
			cout << "__________________" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       ( )      |" << endl;
        	cout << "|       /|\\      |" << endl;
        	cout << "|      / | \\     |" << endl;
        	cout << "|        |       |" << endl;
        	cout << "|       / \\      |" << endl;
        	cout << "|      /   \\     |" << endl;
        	cout << "|________________|" << endl;
        	cout << "    Game Over!    " << endl;
			break;
		default:
			break;
		}
	}
};
class Word //class to store a word to be guessed
{
private:
	string SecretWord; 									//variable that holds the word to be guessed
public:
	Word(string secretword)								//Create a word initialize the value in the constructor
	{
		setStr(secretword);
	}
	void setStr(string secretword)						//method to set the word to be guessed
	{
		SecretWord = secretword;
	}
	string getStr()										//method to get the word to be guessed
	{
		return SecretWord;
	}
	bool findLetter(char l)                             //method to check whether the letter exists in the word to be guessed
	{
		for (unsigned i = 0; i < SecretWord.size(); i++)
			if (SecretWord[i] == l)
				return true;
		return false;
	}
};

class Blanks //Blank class
{
private:
	int numBlanks;
	char blanks[20];
public: 
	Blanks(int numBlanks)						 //Create the blanks
	{
		this->numBlanks = numBlanks;
		for (int i = 0; i < numBlanks; i++)
			blanks[i] = '_';
	}
	void setLetter(int position, char letter)	//Set the letter on a specified position
	{
		blanks[position] = letter;
	}
	bool hasBlanks()							//Check if there are blanks
	{
		for (int i = 0; i < numBlanks; i++)
			if (blanks[i] == '_')
				return true;
		return false;
	}
	void display() 								//Display the progress
	{
		cout << "Welcome to the Hangman Game! Let's begin! " << endl;
		cout << "Player 1 thinks of the word... " << endl;
		cout << "The word is: ";
		for (int i = 0; i < numBlanks; i++)
			cout << blanks[i] << " ";
		cout << "\n";
	}
};

class Guesses	//for player to guess
{
private:
	bool guessed[26];							//26 alphabets in English
public:
	Guesses()									//Initializes the guesses
	{
		for (int i = 0; i < 26; i++)			//Initially A to Z has not been guessed
			guessed[i] = false;
	}
	void addGuess(char letter)					//Set the letter as guessed, assumes letter is in upper case
	{
		int index = letter - 'a';
		guessed[index] = true;
	}
	bool isGuessed(char letter)					//Check if letter has been guessed
	{
		int index = letter - 'a';
		return guessed[index];
	}
	void displayGuessed()						//Display the guessed characters
	{
		cout << "Characters that already has been guessed: ";
		for (int i = 0; i < 26; i++)
			if (guessed[i])
				cout << (char)(i + 'a');
		cout << "\n";
	}
};
void play(Word guessWord) //Play one round
{
	Blanks blanks(guessWord.getStr().size());
	DrawShape shape;
	Guesses guesses;
	shape.draw();
	
	while (shape.getMisses() < 7 && blanks.hasBlanks())				//Keep playing while game is not over
	{
		blanks.display();											//Draw the frame, the blanks, and the guessed letters
		guesses.displayGuessed();

		cout << "Player 2 makes a guess: ";							//Make user guess
		char letter;
		cin >> letter;
		if(!isalpha(letter))
		{
			cout << "Please make sure that you enter an alphabet letter " << endl;
			cin >> letter;
		}
		letter = tolower(letter);

		if (guesses.isGuessed(letter))
		{
			cout << letter << " has been previously guessed" << endl;
		}
		else if (guessWord.findLetter(letter))
		{
			for (unsigned i = 0; i < guessWord.getStr().size(); i++)		//Populate the blank based on the guessed letter
			{
				if (guessWord.getStr()[i] == letter)
					blanks.setLetter(i, letter);
			}
			guesses.addGuess(letter);
		}
		else
		{
			//A miss!
			shape.setMisses(shape.getMisses() + 1);
			cout << letter << " is not one of the letters in the word" << endl;
			guesses.addGuess(letter);
		}
		shape.draw();
	}
	if (!blanks.hasBlanks())
	{
		cout << "Congratulation! You win!\n";
		cout << "The word is: " << guessWord.getStr() << endl;
	}
	else
	{
		cout << "You lose, the word is: " << guessWord.getStr() << endl;
	}
}
// Entry point of the program
int main()
{
	srand((unsigned)time(0));					//Seed the random generator so we get random words every run of program
	
	ifstream inFile;							//Load the words from file
	inFile.open("random_words.txt");

	string words[50];							//Load the words
	int numWords = 0;
	string word;

	while (inFile >> word)
	{
		transform(word.begin(), word.end(), word.begin(), ::tolower);	//Transform words to lower case
		words[numWords] = word;
		numWords++;
	}
	inFile.close();

	bool playAgain = true;						//Given the list of words, we randomly choose one word in the list

	while (playAgain)
	{
		string targetWord = words[rand() % numWords];
		Word guessWord(targetWord);				//Load the objects for the new game
		play(guessWord);

		cout << "Play again? (y/n): ";			//After playing, check if player wants to play again
		char option;
		cin >> option;
		playAgain = (option == 'Y' || option == 'y');
		if(option == 'Y' || option == 'y')
		{
			playAgain = true;
		}else if(option == 'N' || option == 'n'){
			break;
		}else{
			cout << "Invalid input, please enter 'Y', 'y', 'N', or 'n' " << endl;	//check for invalid input
			cin >> option;
		}
	}
	return 0;
}

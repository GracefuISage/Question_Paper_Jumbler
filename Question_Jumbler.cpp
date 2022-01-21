/*
In order to :
Compile : cl .\Question_Jumbler.cpp
Execute : .\Question_Jumbler.exe
*/

#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <fstream>

#define totalQuestions 80
#define question_filename "Question_Paper.txt"
#define answer_filename "Answer_Key.txt"
#define question_shuffles 10000
#define option_shuffles 800
#define new_question_filename "New_Question_Paper.txt"
#define new_answer_filename "New_Answer_Key.txt"


using namespace std;

struct any_Question_Bit {
	string given_options[6];	// given_option[0] - is the question itself, as there can be maximum of 5 choices for any question, there are 6 elements in the array.
	bool correct_answer[6]; // If given_options[2] & given_options[3] are correct for a given question, then correct_answer for that Question_Bit will be an array -> [0, 0, 1, 1, 0, 0], here 0th element is always 0 as for question as it cannot be chosen.
};

struct any_Question_Bit* read_paper_key(any_Question_Bit* QuestionPaper, string question_file_name, string answer_file_name) {

	ifstream question_paper, answer_key;
	question_paper.open(question_file_name);
	answer_key.open(answer_file_name);

	if (question_paper.is_open() && answer_key.is_open()) {

		string reading_line = "", key = "";
		int questions_read = -1;
		int index = 0;

		while (question_paper) {
			getline(question_paper, reading_line);
			if (reading_line.length() == 0) {
				questions_read++;
				index = 0;
				if (questions_read < totalQuestions) {
					getline(answer_key, key);
					// cout << key << "->";
					for (int i = 0; i < key.length(); i++) {
						char option = key.at(i);
						int correct_option = option - 48;	// ASCII of '0'.
						// cout << correct_option << " ";
						QuestionPaper[questions_read].correct_answer[correct_option] = true;
					}
					// cout << endl;
				}
			}
			else {
				QuestionPaper[questions_read].given_options[index] = reading_line;
				index++;
			}
			// cout << reading_line << endl;
		}
		question_paper.close();
		// cout << endl << endl << "Total Questions : " << questions_read + 1 << endl;
	}
	return QuestionPaper;
}

void display_paper(any_Question_Bit* QuestionPaper) {

	for (int i = 0; i < totalQuestions; i++) {
		for (int j = 0; j < 6 && QuestionPaper[i].given_options[j] != ""; j++) {
			cout << QuestionPaper[i].given_options[j] << endl;
		}
		cout << "Ans -> ";
		for (int j = 0; j < 6; j++) {
			if (QuestionPaper[i].correct_answer[j]) {
				cout << "1 ";
			}
			else {
				cout << "0 ";
			}
		}
		cout << endl << endl;
	}
}

void write_paper_key(any_Question_Bit* QuestionPaper, string new_question_file_name, string new_answer_file_name) {
	ofstream question_paper, answer_key;
	question_paper.open(new_question_file_name);
	answer_key.open(new_answer_file_name);

	if (question_paper.is_open() && answer_key.is_open()) {

		for (int i = 0; i < totalQuestions; i++) {
			question_paper << endl;
			for (int j = 0; j < 6 && QuestionPaper[i].given_options[j] != ""; j++) {
				question_paper << QuestionPaper[i].given_options[j] << endl;
			}
			answer_key << to_string(i + 1) << ") ";
			for (int j = 0 ; j < 6 ; j++) {
				if (QuestionPaper[i].correct_answer[j]) {
					answer_key << j;
				}
			}
			answer_key << endl;
		}
		question_paper << endl << endl << endl << endl;
		answer_key << endl << endl << endl << endl;
		question_paper.close();
	}
}

struct any_Question_Bit* shuffle_questions(any_Question_Bit* QuestionPaper, int replacements) {

	for (int i = 0; i < replacements; i++) {
		int x = rand() % totalQuestions;
		int y = rand() % totalQuestions;
		// cout << x << " " << y << endl;

		struct any_Question_Bit Temporary;
		for (int j = 0; j < 6; j++) {
			Temporary.given_options[j] = QuestionPaper[x].given_options[j];
			QuestionPaper[x].given_options[j] = QuestionPaper[y].given_options[j];
			QuestionPaper[y].given_options[j] = Temporary.given_options[j];

			Temporary.correct_answer[j] = QuestionPaper[x].correct_answer[j];
			QuestionPaper[x].correct_answer[j] = QuestionPaper[y].correct_answer[j];
			QuestionPaper[y].correct_answer[j] = Temporary.correct_answer[j];

		}
	}
	return QuestionPaper;
}

struct any_Question_Bit swap_options(any_Question_Bit Question, int optionA, int optionB) {// Swap the options as well as the answers for the given question.
	
	string temporary_for_swapping = Question.given_options[optionA];
	bool flag_for_swapping = Question.correct_answer[optionA];

	Question.given_options[optionA] = Question.given_options[optionB];
	Question.correct_answer[optionA] = Question.correct_answer[optionB];


	Question.given_options[optionB] = temporary_for_swapping;
	Question.correct_answer[optionB] = flag_for_swapping;
	
	return Question;
}


struct any_Question_Bit* shuffle_options(any_Question_Bit* QuestionPaper, int replacements) {

	for (int i = 0; i < totalQuestions; i++) {
		int totalOptions = 0;
		for (int j = 0; j < 6 && QuestionPaper[i].given_options[j] != ""; j++) {
			totalOptions++;
		}
		totalOptions--;	// As the Option at the 0th Index in the Object of any_Question_Bit is not required for any swapping as it is the question itself.
		// cout << totalOptions << endl;
		for (int j = 0; j < replacements; j++) {
			int First_Random = 1 + rand() % totalOptions;
			int Second_Random = 1 + rand() % totalOptions;
			// cout << First_Random << " " << Second_Random << endl;
			QuestionPaper[i] = swap_options(QuestionPaper[i], First_Random, Second_Random);
		}
	}
	return QuestionPaper;
}

struct any_Question_Bit* ordered_questions_options(any_Question_Bit* QuestionPaper) {
	for (int i = 0; i < totalQuestions; i++) {

		int totalOptions = 0;
		for (int j = 0; j < 6 && QuestionPaper[i].given_options[j] != ""; j++) {
			totalOptions++;
		}
		totalOptions--;	// As the Option at the 0th Index in the Object of any_Question_Bit is not required for any swapping as it is the question itself.
		// cout << totalOptions << endl;
		for (int j = 0; j <= totalOptions; j++) {
			if (j == 0) {
				int question_length = QuestionPaper[i].given_options[j].length(), number_of_digits = 1 + log10(i+1/*for now, it is i+1, but Here it is to be the value of type casted to int from the characters in question in between 'Q' & ':'*/);
				QuestionPaper[i].given_options[j] = "Q" + to_string(i + 1) + "  " + QuestionPaper[i].given_options[j].substr(2 + number_of_digits, question_length - 2 - number_of_digits); // care can be taken for index & length of the substring based on the number of digits & other specifics.
			}
			else {
				QuestionPaper[i].given_options[j] = to_string(j) + ") " + QuestionPaper[i].given_options[j];
			}
		}
	}
	return QuestionPaper;
}

int main(void){

	struct any_Question_Bit EmptyPaper[totalQuestions];
	for (int i = 0; i < totalQuestions; i++) {
		for (int j = 0; j < 6; j++) {
			EmptyPaper[i].given_options[j] = "";
			EmptyPaper[i].correct_answer[j] = false;
		}
	}
	/*Although I can execute as many operations as I like, I willonly eecute what is required.*/
	struct any_Question_Bit* myQuestionPaper = read_paper_key(EmptyPaper, question_filename, answer_filename);
	// display_paper(myQuestionPaper);

	struct any_Question_Bit* shuffled_questions_myQuestionPaper = shuffle_questions(myQuestionPaper, question_shuffles);
	// display_paper(shuffled_questions_myQuestionPaper);

	struct any_Question_Bit* shuffled_options_questions_myQuestionPaper = shuffle_options(shuffled_questions_myQuestionPaper, option_shuffles);
	// display_paper(shuffled_options_questions_myQuestionPaper);
	
	struct any_Question_Bit* ordered_shuffled_options_questions_myQuestionPaper = ordered_questions_options(shuffled_options_questions_myQuestionPaper);

	write_paper_key(ordered_shuffled_options_questions_myQuestionPaper, new_question_filename, new_answer_filename);

	cout << "HI";

	return 0;
}

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <json.hpp>

////////////////////////
struct Question {
	std::string question;
	std::vector<std::string> answers;
	int answer;
};

struct Quiz {
	std::string name;
	std::vector<Question> questions;
};

void load(std::vector<Quiz>&);
void play(Quiz&);
///////////////////////

int main() {
	std::vector<Quiz> quizzes;

	load(quizzes);
	
	do {
		int choise;
		std::cout << "Available quizzes: "<< std::endl;
		for(int i = 0; i < quizzes.size(); i++){
			std::cout << i+1 << ". " << quizzes[i].name << std::endl;
		}
		std::cout << "\nInput number 0 or lower to exit" <<std::endl;

		std::cout << "> "; std::cin >> choise;
		if(choise <= 0)
			break;

		play(quizzes[choise-1]);

	} while(true);
	
	return 0;
}

void play(Quiz& quiz){
	int score = 0;
	for(Question& question : quiz.questions){
		int choise = 0;
		std::cout << '\n' << question.question << std::endl;
		for(int i = 0; i < question.answers.size(); i++){
			std::cout << i + 1 << ". " << question.answers[i] << std::endl;
		}
		std::cout << "> "; std::cin >> choise;
		choise -= 1;

		if(choise == question.answer){
			std::cout << "Correct!" <<std::endl;
			score += 1;
		} else {
			std::cout << "Wrong!" << std::endl;
		}
	}
	std::cout << "\n\nYou scored " << score << "/" << quiz.questions.size() << " points.\n\n\n" << std::endl;
}

void load(std::vector<Quiz>& quizzes){
	using json = nlohmann::json;
	using namespace std::filesystem;

	path quizdir_path = "quizzes";
	if(! exists(quizdir_path)){
		create_directory(quizdir_path);
		std::cout << "Created quiz directory" << std::endl;
		return; // Nothing to do more
	}

	for(auto& entry : directory_iterator("quizzes")){
		if(is_regular_file(entry.path()) && entry.path().extension() == ".json") {
			std::ifstream in(entry.path());
			json js;
			in >> js; // Load json string and parse it

			Quiz quiz;
			quiz.name = js["name"];
			for(auto& question_js : js["questions"]){
				Question question;
				question.question = question_js["question"];

				if(!(question_js["answer"].is_number_integer())) // Check if answer number is valid (integer)
					continue;
				question.answer = question_js["answer"];

				for(auto& answer_js : question_js["answers"]){ // Load answers
					question.answers.push_back(answer_js.get<std::string>());
				}

				quiz.questions.push_back(question); // Push Question struct to Quiz struct 
			}
			quizzes.push_back(quiz);
		}
	}
}
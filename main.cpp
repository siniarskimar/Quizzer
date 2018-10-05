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
	std::string author;
	std::string desc;
	std::vector<Question> questions;
};

void load(std::vector<Quiz>&);
void play(Quiz&);
void printAvailable(const std::vector<Quiz>&);
void printHelp();
void printInfo(const Quiz&);
std::vector<std::string> strSplit(const std::string&, char);
///////////////////////

int main() {
	std::vector<Quiz> quizzes;

	load(quizzes);
	printAvailable(quizzes);
	do {
		std::string choice_buff;

		std::cout << "> "; std::getline(std::cin,choice_buff);
		std::cin.clear();

		std::vector<std::string> choice = strSplit(choice_buff, ' ');

		// No switch for strings :(
		if(choice[0] == "q" || choice[0] == "quit")
			break;
		else if((choice[0] == "i" || choice[0] == "info")
				&& !(std::stoi(choice[1]) > quizzes.size()) && !(std::stoi(choice[1]) <= 0)) {
			
			printInfo(quizzes[std::stoi(choice[1]) - 1]);
		} else if((choice[0] == "p" || choice[0] == "play")
				&& !(std::stoi(choice[1]) > quizzes.size()) && !(std::stoi(choice[1]) <= 0)){

			play(quizzes[std::stoi(choice[1]) - 1]);
		} else if(choice[0] == "h" || choice[0] == "help"){
			printHelp();
		} else if(choice[0] == "l" || choice[0] == "list"){
			printAvailable(quizzes);
		}

	} while(true);
	
	return 0;
}

void play(Quiz& quiz){
	int score = 0;
	for(Question& question : quiz.questions){
		int choice = 0;
		std::cout << '\n' << question.question << std::endl;
		for(int i = 0; i < question.answers.size(); i++){
			std::cout << i + 1 << ". " << question.answers[i] << std::endl;
		}
		std::cout << "> "; std::cin >> choice;
		choice -= 1;

		if(choice == question.answer){
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
			quiz.author = (!js["author"].is_null()) ? js["author"] : "Unknown";
			quiz.desc = (!js["description"].is_null()) ? js["description"] : "";

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

void printAvailable(const std::vector<Quiz>& quizzes){
	std::cout << "Available quizzes: "<< std::endl;
	for(int i = 0; i < quizzes.size(); i++){
		std::cout << i+1 << ". " << quizzes[i].name << std::endl;
	}
}
void printInfo(const Quiz& quiz){
	std::cout << "Name: "<< quiz.name << '\n'
		<< "Author: "<< quiz.author << '\n'
		<< "Description: " << quiz.desc <<std::endl;
}
void printHelp(){
	std::cout << "[l]ist - list available quizzes\n"
			<< "[h]elp - print this message\n"
			<< "[p]lay <num> - play a quiz\n"
			<< "[i]nfo <num> - show info about a quiz\n"
			<< "[q]uit - quits the program" << std::endl;
}

std::vector<std::string> strSplit(const std::string& str, char delim){
	std::vector<std::string> tokens;
	std::string buff = "";

	for(int i = 0; i < str.length(); i++){
		const char& ch = str[i];
		if(!(ch == delim)){
			buff += ch;
		} else {
			tokens.push_back(buff);
			buff = "";
		}
	}
	tokens.push_back(buff);

	return tokens;
}
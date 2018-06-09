#pragma once
#include<string>
class Lesson
{
private:
	std::string day;
	std::string less_num;
	std::string less;
	std::string type;
	std::string room_num;
public:
	Lesson();
	Lesson(std::string day, std::string less_num, std::string less, std::string type, std::string room_num);

	std::string get_day() {
		return this->day;
	}
	std::string get_less_num() {
		return this->less_num;
	}
	std::string get_less() {
		return this->less;
	}
	std::string get_type() {
		return this->type;
	}
	std::string get_room_num() {
		return this->room_num;
	}
};


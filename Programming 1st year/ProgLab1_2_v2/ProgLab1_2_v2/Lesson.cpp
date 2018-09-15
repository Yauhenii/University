#include "Lesson.h"

Lesson::Lesson()
{
	this->day = "";
	this->less_num = "";
	this->less = "";
	this->type = "";
	this->room_num = "";
}

Lesson::Lesson(std::string _day, std::string _less_num, std::string _less, std::string _type, std::string _room_num)
{
	this->day = _day;
	this->less_num = _less_num;
	this->less = _less;
	this->type = _type;
	this->room_num = _room_num;
}

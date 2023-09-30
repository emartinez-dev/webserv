#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
{
}

ErrorPage::~ErrorPage()
{
}

ErrorPage::ErrorPage(ErrorPage const &copy)
{	
	this->status_code = copy.status_code;
	this->path = copy.status_code;
}

ErrorPage	&ErrorPage::operator=(const ErrorPage &copy)
{
	this->status_code = copy.status_code;
	this->path = copy.status_code;
	return *this;
}

void ErrorPage::setPath(std::string value) {
	path = value;
}

void ErrorPage::setCode(std::string value) {
	status_code = atoi(value.c_str());
}

int ErrorPage::getCode() {
	return status_code;
}

std::string ErrorPage::getPath() {
	return path;
}
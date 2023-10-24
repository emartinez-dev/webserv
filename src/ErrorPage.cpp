#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
{
}

ErrorPage::~ErrorPage()
{
}

ErrorPage::ErrorPage(ErrorPage const &copy):status_code(copy.status_code), path(copy.path)
{	
}

ErrorPage	&ErrorPage::operator=(const ErrorPage &copy)
{
	if (this != &copy) {
		status_code = copy.status_code;
		path = copy.status_code;
	}
	return *this;
}

void ErrorPage::setPath(std::string value) {
	path = value;
}

void ErrorPage::setCode(std::string value) {
	status_code = atoi(value.c_str());
}

int ErrorPage::getCode() const
{
	return status_code;
}

const std::string &ErrorPage::getPath() const {
	return path;
}

#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
{
}

ErrorPage::~ErrorPage()
{
}

ErrorPage::ErrorPage(ErrorPage const &copy):statusCode(copy.statusCode), path(copy.path)
{	
}

ErrorPage	&ErrorPage::operator=(const ErrorPage &copy)
{
	if (this != &copy) {
		statusCode = copy.statusCode;
		path = copy.statusCode;
	}
	return *this;
}

void ErrorPage::setPath(std::string value) {
	path = value;
}

void ErrorPage::setCode(std::string value) {
	statusCode = atoi(value.c_str());
}

int ErrorPage::getCode() const
{
	return statusCode;
}

const std::string &ErrorPage::getPath() const {
	return path;
}

# webserv
This project is here to make us write our own HTTP server.

## Class Descriptions
<details>
<summary>HttpRequest Class</summary>
It is an object to save the Request to a server.

### Public Methods
```cpp
HttpRequest(const std::string& request_str);
```
+ It is the class constructor that receives the client request.

```cpp
HttpRequest(const HttpRequest& other);
```
+ Copy builder to meet convention standards.

```cpp
HttpRequest& operator=(const HttpRequest& other);
```
+ It is the assignment operator.

```cpp
~HttpRequest();
```
+ It is the destroyer of the object.

```cpp
std::string getMethod() const;
```
+ Function to obtain the request method.

```cpp
std::string getPath() const;
```
+ Function to obtain the route of the request.

```cpp
std::string getVersion() const;
```
+ Function to obtain the request protocol.

```cpp
std::string getHeader(const std::string& name) const;
```
+ Function to obtain the value of a request header.

```cpp
std::string getHeaderKey(const std::string& name) const;
```
+ Function to obtain the key of a request header.

```cpp
std::map<std::string, std::string> getHeaders() const;
```
+ Function to get all the headers of the request.

```cpp
std::map<std::string, std::string> getParameters() const;
```
+ Function to obtain all the parameters of the request.

```cpp
void  printRequest(void);
```
+ Function to print the elements of the HttpRequest object.
</details>
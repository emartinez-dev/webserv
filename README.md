# WebServer

WebServer is a C++98 project developed from the ground up, providing a scalable
and efficient web server solution. This README.md file serves as a
comprehensive guide to understanding the project, setting it up, and
implementing its features.

This is a group project of 42 school curriculum done with my colleagues
[Alberto Barriga](https://github.com/albertobarriga) and [José Carlos
Limones](https://github.com/jlimonesHer). 

To check how we divided the work and how we managed everything, you can check all
our [issues](https://github.com/emartinez-dev/webserv/issues?q=is%3Aissue+is%3Aclosed+is%3Aopen)

## Project Structure

The project structure is organized as follows:

- `include`: Contains header files for the project.
- `src`: Includes the source code files.
- `LICENSE`: The license information for the project.
- `Makefile`: Used for building the project.
- `README.md`: This documentation file.
- `example_config.yaml`: An example configuration file.

## Building the WebServer

To build the project, execute the following commands:

```bash 
make 
```

This will compile the source code and generate the executable named `webserv`.

## Running the WebServer

To run the web server, use the following command:

```bash
./webserv <config_file_path>
```

Replace `<config_file_path>` with the path to your configuration file.

If you want to see the example server that we created to check in the evaluation, 
you can move to [this branch](https://github.com/emartinez-dev/webserv/tree/example-server) 
and run this command there.

## Configuration

The configuration file is crucial for defining server parameters. An example
configuration file (`example_config.yaml`) is provided:

```yaml
server: (a configuration can have multiple servers)
  root: example/ (folder that will be the root of the server)
  server_name: example.com (server virtual name) 
  cgi_path: /usr/bin/python3 (cgi executable path) 
  client_max_body_size: 3000000 (max size that server will allow, expressed on bytes)
  error_page (a server can have multiple error pages):
    path: /errorPages/404.html (custom error page path)
    status_code: 404 (corresponding status code for said error page)
  listen: (ip and port that the server will listen on)
    host: 127.0.0.1 
    port: 8080
  location: (a server can have multiple locations)
    route: / (url that will match this route)
    root: alberto/ (corresponding file system path for this server route, concatenated with server root)
    index: index.html (file that will be served when requesting for a folder)
    autoindex: on (generate a file listing if the index file is deactivated or not found)
    allow_methods: GET POST (allowed methods for this location, can be GET, POST, and/or DELETE)
    allow_uploads: on (if the given location accepts file uploading on post requests)
    uploads_path: files/ (server path to save the uploaded files)
    cgi_ext: .py (call the CGI with files that end with this extension)
```

## Results

Our webserver proved to be really strong on stress tests, as it had an availability 100% of the time
when stressed with siege as you can see here:

![image](https://github.com/emartinez-dev/webserv/assets/73340871/a6be796d-06b9-467e-8133-904ce879931a)
![image](https://github.com/emartinez-dev/webserv/assets/73340871/de943a43-6666-4dda-a174-79dabff30ad9)


Feel free to reach out for any additional information or assistance in further
developing and extending the functionality of this web server!

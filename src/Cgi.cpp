#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Cgi.hpp"

std::string runCGI(const std::string& cgi_path, const std::string& cgi_file, char **envp) {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return "Error en la creación del pipe.";
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return "Error en la creación del proceso hijo.";
    } 
	else if (child_pid == 0) {
        // Proceso hijo: configurar las variables de entorno y ejecutar el script CGI.
        setenv("QUERY_STRING", "param1=value1&param2=value2", 1);
        setenv("REQUEST_METHOD", "GET", 1);

        // Redirigir la salida estándar al extremo de escritura de la tubería.
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        char* script_args[] = {(char*)cgi_path.c_str(), (char*)cgi_file.c_str(), nullptr};
        execve(cgi_path.c_str(), script_args, envp);
        perror("execve");
        exit(1);
    } else {
        // Proceso padre:
        close(pipe_fd[1]);

        std::string result;
        char buffer[1024];
        ssize_t bytes_read;

        // Leer la salida del script CGI desde la tubería y construir el resultado.
        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            result.append(buffer, bytes_read);
        }
        close(pipe_fd[0]);

        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                std::cerr << "Script CGI devolvió un código de salida no nulo: " << exit_status << std::endl;
            }
        } else {
            std::cerr << "El proceso hijo no terminó con éxito." << std::endl;
        }
        return result;
    }
}

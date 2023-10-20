import os

print("<h1>Hello world from python in a CGI</h1>")
print("<p>I send me a variable hola in the URL and I will try to print it")

try:
    print("<h3>" + os.getenv("hola") + "</h3>")
except Exception as e:
    print("<h3>I couldn't</h3>")
print("<h2>Priting the whole environment</h2>")
print("<ul>")
for k,v in os.environ.items():
    print(f"<li>{k}: {v}</li>")
print("</ul>")

import os

print("<h1>Hello world from python in a CGI</h1>")
print("<p>I send me a variable hola in the URL and I will try to print it")

try:
    print("<p>" + os.getenv("hola") + "</p>")
except Exception as e:
    print("<p> I couldn't </p>")
print("<h2>Priting the whole environment</h2>")
print("<ul>")
for k,v in os.environ.items():
    print(f"<li>{k}: {v}</li>")
print("</ul>")

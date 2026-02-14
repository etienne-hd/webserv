import os
import random

def send_cgi(content: str = "", headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def main() -> None:
	files = os.listdir("examples/cgi_redirection/files")
	if (len(files) == 0):
		send_cgi(
			content="<html><body><h2>No file</h2></body></html>",
			headers={
				"Status": 200,
				"Content-Type": "text/html"
			}
		)
	else:
		file = random.choice(files)
		send_cgi(
			headers={
				"Location": os.path.join("/files", file),
				"Cache-Control": "no-cache"
			}
		)

if __name__ == "__main__":
	main()
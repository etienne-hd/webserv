import time

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def main() -> None:
	time.sleep(60)
	send_cgi(
		content="<html><body><h2>Hello from Python!</h2></body></html>",
		headers={
			"Status": 200,
			"Content-Type": "text/html"
		}
	)

if __name__ == "__main__":
	main()
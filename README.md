# Http Web Client
    The project aims to realize the connection between an http client and a server simulating an online book library.
##### Contributor: Dumitrascu Filip-Teodor (323CA)

## Content
0. [Server](#server)
1. [Client](#client)
2. [JSON](#json)
3. [Epoll](#epoll)

## Server
    Already implemented can be found by connection details at HOST: 34.246.184.49 and PORT: 8080. It has the following features:
-   register: register an user
-   login: login a user
-   enter_library: request access to the library 
-   get_books: request all books on the server
-   get_book: request information about a book
-   add_book: add a book
-   delete_book: delete a book
-   logout: log the user out
-   exit: exit the program

## Client
    The main logic behind the project, the client is implemented to take
    into account 2 main actions:
- receiving input from the stdin, validating and sending a corresponding request to the server
- receiving the response from the server and displaying a response to the user

    Requests are sent only after the orders are validated and the buffer received from the server is parsed to display an appropriate message to the client. Commands are grouped and managed according to the request they have to make (enter_library, get_book, get_books, logout - GET; delete - DELETE; register, login, add_book - POST).

## JSON
    Parser used to get strings from json format. After the validation of book or user, their details are inserted into the object and it is serialized to the request content.

## Epoll
    Although it is known every time the client receives input from the stdin and after that data from the server, the use of an epoll is beneficial from the following point of view: if the input entered by the user is invalid, a http request will not be sent to the server (it would not make sense to send it knowing ths client will receive an error code 400). So the next action that will happen for the client will also be done by stdin (reentering a hopefully valid request). The lack of IO multiplexing would have continued the scenario on waiting for a response from the server, an action that makes no sense because the client did not send any request.

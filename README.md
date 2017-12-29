# Instructions for ChatProgram for miniproject
This Mini Project is created in by:<br>
<b>Lasse Krøigaard</b><br>
<b>Stephanie Christoffersen</b><br>
<b>Casper Thomsen</b><br>
<b>Nicolaj Justesen</b><br>
<b>Dimitar Simeonov</b><br><br>

The program has successfully been tested with Microsoft Visual Studio 2013 and Microsoft Visual Studio 2017. It's therefore recommended to use either of these for editing the code.<br><br>

In order to have the program working, a few changes are necessary. Here is a small guide, which describes step-by-step on what needs to be done:<br><br>

1: Open the Server folder.<br>
2: Open the Project3.sln file.<br>
3: Find #define section.<br>
4: Click on your internet connections, click your connected network and click "Properties"<br>
5: Scroll down and find your IP-address and Network Channel.<br>
6: Go back to the Project3.sln file.<br>
7: Edit #define IP_ADDRESS to your IP: Remember the quotation marks.<br>
8: Edit #define DEFAULT_PORT to your Network Channel: Remember the quotation marks. <br>
Now the server is good to go. Let's continue with the client.<br>
9: Open the Client folder.<br>
10: Open the Client.sln file.<br>
11: Edit #define IP_ADDRESS to your ip: Remember quotation marks.<br>
12: Edit #define DEFAULT_PORT to your Network Channel: Remeber quotation marks.<br>
Note: It is <b>very</b> important that the server and client have the same IP-address and Port, otherwise the program will not run.<br><br>
13: Now in the Project3.sln file press the button labeled local windows debugger and the same in the Client.sln file afterwards.
14: After the both have built you should now both have a client and server open and can chat with the client. Now there should be a new folder in the Cleint folder called Debug. Open this and click the Client.exe a few times and multiple clients will connect to the server.

You are now good to go. You can open multiple clients as well.

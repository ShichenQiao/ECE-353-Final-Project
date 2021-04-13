# ECE-353-Final-Project
Authorized users: Justin Qiao, Mark Xia, and ECE 353 staff members (if any). DO NOT SHARE TO ANYONE ELSE

****General Information****
Project Name: Airplane Fight
Initiated Date: Apr 10, 2021
Team Members: Justin Qiao, Mark Xia
Professor Name: Younghyun Kim

****Project Description****
The project will begin with a Main Page. Users will be able to select between two modes: Gaming and Setting. In the Gaming mode, the user needs to press S1 on MKII to start a new game. The airplane is controlled by the Accelerometer on MKII, and the user can move the board to move the airplane in the x or y directions accordingly. When a game is going on, pressing S1 will launch the ball from the airplane, pressing S2 will change the color of the ball if it is not launched. Enemy square will be randomly generated at the top section of the LCD screen. When the ball hit a square, if the color of the ball matches the color of the square, the square will be destroyed, the user gets scored, and then the ball will be reset. If the color does not match, or the ball is travelling beyond the top edge of the LCD screen, the ball will be reset directly.

****TODOs****
1. (Mark) Implement buzzer and light sensor by Apr 16, midnight.
2. (Justin) Improve usage of queue, semaphore on SPI, and show score and time of the side of Gaming mode by Apr 16, midnight.
3. Implement main page.
4. Implement Setting mode.
5. Improve complexity

****Journals****

**Apr 12, 2021 (J.Q.)**
1. Uploaded prototype 1.0
2. Started README file
3. Talked to TAs and Prof, get the following advices: improve usage of Queue, make usage of Accelerometer more extensive, increase complexity (mult layers, recursions, different angles of launches, show score and time on board, shake to change color, and so on).

# ECE-353-Final-Project
Authorized users: Justin Qiao, Mark Xia, and ECE 353 staff members (if any). DO NOT SHARE TO ANYONE ELSE

****General Information****
Project Name: Breaker
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

**Apr 13, 2021 (J.Q.)**
1. Generated data info for arial font.
2. Generated tank_front, tank_left, tank_right(may replace the airplane).
3. Note: to make y-directional movements necessary, try making "airplane" smaller, then adding random obstacles

**Apr 14, 2021 (J.Q.)**
1. Generated image data for the thank in for directions. Width = 20, Height = 15.
2. Replaced the airplane by the tank.
3. Add tank movenment Animation. Tank will change direction according to last non-center command. If a center command is received, no change in direction.
4. Now the tank change color with the ball.
5. Added score board section on top of the LCD.
6. Genreated font database, and functions to access it, see font_arial.h for details.
7. Updated Accelerometer sensitivity for better user experience.
8. Enabled launching ball in four directions, same as the direction that the tank is facing to when S1 is pressed.
9. Changed the corrdinates of the ball when not launched to have the same center as the tank.
10. Restricted the zone where the ball can be in (4 sides).
11. Now, the tank have a larger zone it can moves in.
12. Changed square size from 10 by 10 to 12 by 12.
13. Updated project name, updated code to version: Breaker 2.1
14. Plans for next steps: Implement barrier matrix so that the tank and the ball will not overlap any enemy square, vise versa; generate random obstacles while generating a new game to make y-direction movements more necessary. 

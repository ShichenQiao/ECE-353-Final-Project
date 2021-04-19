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
1. (Mark) Implement buzzer and light sensor by Apr 18, midnight.
2. (Justin) Improve usage of queues.
3. Implement main page.
4. Implement Setting mode.

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
3. Add tank movement Animation. Tank will change direction according to last non-center command. If a center command is received, no change in direction.
4. Now the tank change color with the ball.
5. Added score board section on top of the LCD.
6. Generated font database, and functions to access it, see font_arial.h for details.
7. Updated Accelerometer sensitivity for better user experience.
8. Enabled launching ball in four directions, same as the direction that the tank is facing to when S1 is pressed.
9. Changed the coordinates of the ball when not launched to have the same center as the tank.
10. Restricted the zone where the ball can be in (4 sides).
11. Now, the tank has a larger zone it can moves in.
12. Changed square size from 10 by 10 to 12 by 12.
13. Updated project name, updated code to version: Breaker 2.1
14. Plans for next steps: Implement barrier matrix so that the tank and the ball will not overlap any enemy square, vice versa; generate random obstacles while generating a new game to make y-direction movements more necessary. 

**Apr 16, 2021 (J.Q.)**
1. Uploaded Breaker 2.2, enabled four direction movements and launches. Still have problem with lcd/matrix race condition, and have problems with hitting an edge between two matched-color squares.

**Apr 17, 2021 (J.Q.)**
1. Uploaded Breaker 2.5.
2. Made sure that newly generated square will not overlaps the ball, nor the tank.
3. Solved lcd/matrix race condition.
4. IMPORTANT: Added new game rule: when a ball hit a square whose color does not match, the ball will be reset, and the square will get a new randomly generated color.
5. When a ball hit on the edge between two squares, the ball will be reset and the condition of the two squares will be determined simultaneously.
6. Fixed edge condition: when a ball hit the edge of two squares next to each other, the ball will be reset, and the conditions of the two squares will be determined simultaneously so that there will not be any half-broken squares caused by this edge condition.
7. Added buffer values when calculating indexes from coordinates. This solved the problem will ball may go across a mis-matched square on its top or left.
8. Fixed bug that the tank can not change direction when it is blocked by a square.
9. Updated and unified comments in all files.

**Apr 18, 2021 (J.Q.)**
1. Implemented timer and score counters on the score board.
2. Uploader Breaker 3.1.
3. Added count down timer to terminate a game after a specified amount of seconds, by default, 120 seconds.
4. When a game ends, a message box will show up with scores and instructions. (This version only have the box, to be finished by tomorrow.)
5. When the messgae appears, press S1 will reset the game, and press S1 again will start the game, just like the first round.
6. Seperated software initializations from major task_*** functions to idependent reset functions for convinence.
7. Problem left: Free up S2 when game is not ongoing.

# ECE-353-Final-Project
Authorized users: Justin Qiao, Mark Xia, and ECE 353 staff members (if any). DO NOT SHARE TO ANYONE ELSE

****General Information****
Project Name: Breaker
Initiated Date: Apr 10, 2021
Team Members: Justin Qiao, Mark Xia
Professor Name: Younghyun Kim

****Project Description****
Our Project’s name is Breaker. It starts with a home page, where users could press S1 to start a new game or S2 to settings. In the gaming page, users could press S1 to start a game or S2 to go back to home, a theme song will also be played. During a game, user will control a tank moving in four directions around the LCD from the accelerometer or console window (WASD). There will be squares with random colors generated randomly generated around the LCD. Users could press S1 or hit Space on their key board to shoot out a ball with the same color as the tank and moving in the same direction as the tank is facing to. When the ball hit a square with the same color, the score will be destroyed and the user will score one points, showing on a score board at the top of the screen. If the color does not match, the color of the square(s) will be re-chosen. A ball can hit at most 2 squares at a time. When the timer on the top of the LCD reaches 0, the game will end with score and instructions printed to the LCD. In the setting mode, the user will go through three pages to set the game duration, number of colors used, and background (by lux reading from the light sensor). There will be detailed instructions printed to the console window at all stages of the game.


****Demo Video****
https://youtu.be/FxTAj4saZ78

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

**Apr 19, 2021 (J.Q.)**
1. Updated to Breaker 3.4
2. Freed S2 for other use (go back to main page, maybe) when a game is not on going
3. Finished up end game message. The score will be shown and buttons will be freeze for 5 seconds (count down time shown on LCD). After the pause, press S1 could reset the game.
4. Reorganized lcd related code.
5. Added Pre-game message
6. Solved font bugs

**Apr 23, 2021 (J.Q.)**
1. Updated to Breaker 4.1
2. Implemented console window commands (only when game_on_going == true): 'w', 'a', 's', 'd' (case insensitive) from console will move the tank by 1 pixel to up, left, down, and right if there is no commands from the accelerometer. The space character ' ' will launch the ball when possible.
3. Added TANK_CMD_LAUNCH for launching the ball from console window to TANK_CMD_t.
4. [FIXED] Bug detected: the ball can not be launched if the tank is at it up-most row (will be fixed in the next version).
5. TODO: print messages (and directions) to the console while important events happens in the game.

**Apr 28, 2021 (J.Q.)**
1. Updated to final version: 5.3
2. Finished Home page and Setting mode.
3. Integreated light sensor, led effects, and buzzer effects.
4. Finalized comments and styles.
5. Added detailed instructions to console window.

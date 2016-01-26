#2D-Shooter-Game

This game uses **OpenGl2** and **freeglut**.
freeglut may need to be installed - possibly this should suffice:

`sudo apt-get install freeglut3`

For the audio, **SFML** library is used. Install using:

`sudo apt-get install libsfml-dev`

####Keyboard Controls

The keyboard controls for the game are:

	Space - Shoot
	    f - Increase shooting power
	    s - Decrease shooting power
	    a - Rotate the cannon Upwards
	    b - Rotate the cannon Downwards
	    p - Move the cannon Upwards
	    l - Move the cannon Downwards
	   Up - Zoom In
	 Down - Zoom Out
	Right - Pan scene Right
	 Left - Pan scene Left

####Mouse Controls

The mouse controls for the game are:

 							Left Button - Shoot
	Hold Left Button and move Upwards/Downwards - Increase/Decrease shooting power
	     Hold Left Button and move Mouse around - Rotate cannon Upwards/Downwards
	      Hold Right Button and move Left/Right - Pan scene Left/Right
			      	Scroll Button up/down - Zoom In/Out

The keyboard will still need to be used to move the cannon upwards and downwards.

####Starting the game

- `cd game`
- `make`
- `./game`

Enter your name.
Choose between two **game modes** - `Land`/`Underwater`
The underwater mode gives a kind of slow mo effect with a bit of different physics used and of course a different scene.

If you get a **black screen**, then try running `./game` till you get a game window. It works.

The game has **three levels**.

The **objective** of the game is to shoot down the spinning/rotating targets which appear on top of the moving obstacles.
You get a `specific amount of time to finish each level`.
The first level must be finished within **70 seconds**, the second within **55 seconds** and the third within **40 seconds**, failing which the game ends.
The higher levels have `faster missiles`, `lesser time` and a more `persistent monster`! Brace yourself.
You get an increment of **five seconds** for every target that you shoot.

There are no limitations on the number of shots you fire except in the third level, where you get only `10 shots` to shoot down all the targets.

There are four types of targets:

- **Circles**	- **5** points
- **Triangles**	- **10** points
- **Squares**	- **15** points
- **Stars**	- **20** points

The balls rebound from obstacles and will die out if they move out from the right part of the window or if their velocity is below a particular threshold.

The best part of the game is the evil monster which will try to bust your cannon by firing missiles.
You have three lives in each level and every successful hit by a missile will **decrease your life** by one.
The monster cannot be shot down.

You finish the game if you complete all the levels. A pretty hard task!

Enjoy and challenge your friends to a battle of highscores.

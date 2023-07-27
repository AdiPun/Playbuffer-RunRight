# Playbuffer-RunRight

![Screenshot1](https://github.com/AdiPun/Playbuffer-RunRight/blob/main/playbuffer-RunRight/RunRight_Screenshot.png?raw=true)

## Hello!

Welcome to RunRight, a side-scrolling jump and beat-em-up game I developed using SumoDigital's Playbuffer 2D game development framework.

### The game and using Playbuffer

This game was incredibily fun to make. Using Sumodigital's header and tutorial taught me various essential concepts in game development using C++. Throughout the development process, I learned how to:

- Code in C++
- Create functions and use enumeration, states, and types
- Utilise the Playbuffer framework to create game objects
- Assign types, velocities, and acceleration to game objects
- Implement collision detection using if loops
- Spawn game objects dynamically
- Code logically and efficiently

### Gameplay

RunRight is a side-scrolling game where your character never moves on the x-axis. Instead, your arrow keys make different layers of the background move at varying velocities, that create a feeling of speed and a parallax effect that I'm very proud of.

### Features

- I added variation to the gameplay by spawning in a selection of random debris the spaceship can throw at you. The velocity at which they travel upwards and towards you are also varied from a random set range.
- When the debris 'bounces' on the ground, it is given random upwards velocity from another set range. I think this adds a feel of variety and unpredictibility to the debris.
- I used player_states to have different controls and animations when the player is airborne and it plays a rolling
animation when they land after jumping.
- I used player_states to have the debris behave differently when the player is in their attack state or rolling state.
- All the assets I used were from various talented creators on itch.io.

### RunRight
It's a hot summer Tuesday, the Sun's just about to dip below the horizon and you're chasing a spaceship through a barron highway in the rocky mountains. All kinds of junk is being thrown your way in an effort to lose you. 
The worst part? It stole your {thing. you like not being stolen}. 
Jump, Slash and Somersault your way through the debris and take back what is rightfully yours. The {thing that you prefer to be on your person a majoraty of the time}

### Controls

- Left and right arrow keys: Run left or right
- Up arrow key: Jump
- Down arrow key: Roll to evade debris / Somersault if airborne
- Spacebar: Sword slash

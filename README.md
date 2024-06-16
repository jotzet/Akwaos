# Akwaos - Aquatic Ecosystem Simulator

This project is a simple game inspired by John Conway's Game of Life. It simulates an aquatic ecosystem where various types of fish interact. Players can introduce different fish species into the environment and observe how they influence the ecosystem.
<br>
The name "Akwaos" combines the Polish prefix "akwa-" meaning water-related, with "chaos" to highlight the unpredictable nature of the system.

# Running the simulation

To start the simulation, run the executable file (`AKWAOS.exe`) provided.

## Modifying the simulation

If you wish to make changes to the simulation, such as adjusting the number of fish or the size of the ecosystem, you can do so by editing the `main.cpp` file. The relevant functions are located at the top of the file and are commented. After making your changes, compile the program by running the make command in the game directory. This will generate a new executable file, which you can then run to start the updated simulation.

## Resetting the simulation

To reset the simulation at any time, press the "R" key on your keyboard.

# Fish Species

**0. Basic Fish**<br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/f10290a9-4da8-41cf-883a-14daa09ec106) <br>
Characteristics: Standard behavior. <br>
Reproduction: Normal rate. <br>

**1. Goldfish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/6c9d6155-f4ec-41d3-99a3-3039ab39c18d)
<br>
Characteristics: Small and fast. <br>
Reproduction: Very rapid. <br>

**2. Crazy fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/e2b625ed-60a3-4429-9aae-88ab011f6960)
<br>
Characteristics: Crazy unpredictable movement <br>
Reproduction: Quite rapid. <br>

**3. Dumb Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/f41de752-a738-4150-8e0c-ec6086563671)
<br>
Characteristics: Slow-moving. <br>
Reproduction: Extremely rapid. <br>

**4. Disgusting Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/310a1cde-f7bb-42ae-ad46-303f9a7826b2)
<br>
Characteristics: Similar to Basic Fish. <br>
Special Trait: Cannot be eaten. <br>

**5. Goldfish Eater** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/aeae0e8b-77e3-4ec9-a394-1e0c4c26120e)
<br>
Characteristics: Aggressive, small and fast. <br>
Special Trait: Can only eat goldfish. <br>

**6. Aggressive Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/1e042cf1-7b1c-4b55-980b-d84b2436bf80) <br>
Characteristics: Large, aggressive, and slow. <br>
Reproduction: Extremely slow. <br>

**7. Canibal Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/0d4d5cfc-d7d6-4795-9dbd-c939d26b2f71) <br>
Characteristics: Quite fast and aggressive. <br>
Reproduction: Fast. <br>
Special trait: When meeting fish of the same species, they can reproduce or consume them (or both). <br>

**8. Zombie Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/e4a30bc8-3e38-4bff-a316-528a67b4619b)
 <br>
Characteristics: Slow, unedible. <br>
Reproduction: Cannot reproduce. <br>
Special trait: Turn other fish into zombies. They change their colors to green, they can't reproduce and they are now aggressive canibals and can eat even slightly bigger fish. <br>

**9. Freezing Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/ea9ec8d2-d3db-4c08-8e76-8f4568f7ae2f)<br>
Characteristics: Slow, unedible. <br>
Reproduction: Cannot reproduce. <br>
Special trait: Freeze other fish - they can't move (they change color to blue) <br>

**10. Healing Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/d2f6d52a-0310-402b-a5fb-d19860e91933)
 <br>
Characteristics: Fast, unedible <br>
Reproduction: Cannot reproduce. <br>
Special trait: Can heal and unfreeze other fish. <br>

**11. Psycho Fish** <br>
![image](https://github.com/jotzet/Akwaos/assets/91730870/670efd6e-159d-420c-89b9-e565b7d08e33) <br>
Characteristics: Unpredictable. <br>
Reproduction: Unpredictable. <br>
Special trait: Get spawned with random behaviour. <br>

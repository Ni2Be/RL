# RL

An Ai Framework

Implemented games:
 - Snake
 - Pong
 
Implemented Agents:
 - MCTS-Agent
 - TD-Agent
 
The framework supports multiple agents playing together. Each agent calculates its next move in a separate thread, so if an agent that needs to calculate many moves (like the MCTS-Agent) is playing against an agent that doesn’t need to calculate so much (like the TD-Agent) both can show their full potential. This also ensures if two or more agents are playing against each other the fairness is handled by the operating system.

|One MCTS-Agent|Two MCTS-Agent|
|-------------------------------|-----------------------------|
|![one player MCTS](https://github.com/Ni2Be/RL/blob/template_state/one%20player.gif)|![two player MCTS](https://github.com/Ni2Be/RL/blob/template_state/two%20player.gif)|




### Setup for devs

1. download branch "template_state"
2. download 64bit sfml https://www.sfml-dev.org/download/sfml/2.5.1/ 
   unzip in lib-folder and delete -2.5.*
   should look like this:
   RL-template_state/lib/SFML/include
3. Open CleanStart sln
4. In VS open „Projects/Settings“
5. change “Configuration” to “All”
6. in configuration settings change your Windows SDK-Version to your version
7. Hit OK
8. If “Debug” is selected change it to “Release” and 64 bit
9. CTRL+F5 should now build and run the project


# Ketu

Lightweight framework to simulate drone formations.

Ketu provider simple abstractions to mimick the sensors available on drones and create
scenarios to see how the drones react to each other in the world.
Using ketu you can simulate autonomous drones that align to create any shape.
Checkout this [video](https://www.youtube.com/watch?v=yfkrUXFjLRA) for a demonstration.

## Why use Ketu?

Ketu provides a sandbox for people to quickly try out new drone formation techniques 
without amount of large setup.
For a more detailed simulation you can use ROS but to quickly try out
a formation algorithm Ketu is your best bet.

![ketu image](./images/formation.png)

## Key Concepts

Ketu takes a simplistic approach to how simulations are done.
Every simulation is modelled as a scenario where you can add nodes that sense and interact with each other.
A scenario is an empty 3D world with the rendering / update abstracted away for you so you can focus on how nodes
interact with each other. A scenario has a render loop that you can hook into to orchestrate creation and movement of drone formations.

You can add multiple nodes to your world in a scenario. A node can mean anything but in Ketu we specifically
use it to mean drones in a formation. You can of course  subclass node and create other types of nodes.

Each node in a formation has access to a sensing client and a communication client.
You can use the sensing client to get information about objects in the world
and you can use the communication client to send messages to other objects.
Simple implementations of these clients are provided, but you are encouraged to 
extend these clients to create your own clients. 

Creating drone formations is handled by formation co-ordinators through message passing between nodes.
The mesh formation coordinator aligns randomly placed nodes into a formation defined a by a 3D mesh file provided in obj format.
You can create more complex formation co-ordinators by extending the interface.

Checkout the RandomNodes scenarios in the scenarios directory to get started.


## It's easy to setup!

Ketu has very few dependencies namely
 - [Raylib](https://www.raylib.com)
 - [TinyOBJ](https://github.com/tinyobjloader/tinyobjloader/tree/release) - header already included, no installation required 

That's it, once you have raylib installed on your machine you're good to go.

## Contributions

Contributions are welcome. Just keep it simple, avoid complex design patterns unless necessary.




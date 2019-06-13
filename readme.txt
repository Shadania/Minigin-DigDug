In this project I will code my own 2D game engine and recreate Dig Dug in it.
I am a secondyear student at DAE (Howest Kortrijk) and this project is for the Programming 4 final assignment.
The base of the project is the Minigin framework.

Engine specifics:
-Uses a entity-component system. A game exists out of a scene that holds gameobjects which hold components.
-Components can need rendering ("GameObject::AddComponentNeedRendering(comp)") or not.
-Has a service locator which can locate you the following services: Audio, CollisionManager, GameTime, InputManager, Renderer, ResourceManager, SceneManager. Also holds general info about the game (window height and width).
-Uses smart pointers everywhere where memory management needs to be kept in mind. Weak pointers or raw pointers are used by objects that do not own the object pointed at and are used depending on if the source is a smart pointer or an owning object (e.g. class A spawns class B but class B needs a reference to class A, so class A gives it its this pointer)

Audio uses SDL mixer.

Collision system: If a component needs to collide with other components, you add a CollisionComponent to it. On construction it needs a tag, which will be used by other objects to check if they want to get collision from this object. It holds two vectors of tags, one of tags it is interested in generating collision with, and one of tags it does not want to generate collision for. Tags used in the DigDug game are: 0 (main DigDug), 1 (rock), 2 (Pooka), 3 (Fygar), 4 (main DigDug's pump), 5 (Fygar fire), 6 (secondary DigDug in Co-op), 7 (secondary DigDug's pump). Collision components can register, unregister, add targets or add ignores at runtime.

GameTime: Generic gametime manager. Used to request elapsed seconds.

Input Manager: Manages input from keyboard and up to two controllers.
Keyboard: Uses SDL events. Can create Commands and Axes ingame, Axes taking two keycodes (a negative and a positive) and returning a value between -1 and 1. Commands can be Up, Down, or continuously down. This is managed in the update and bools are requested during regular game time.
Controller: Uses XInput.

Renderer: Uses SDL image and textures. Has about four RenderTexture functions that render Textures in slightly different ways using different input variables. Also holds the Scale of the game, if you want a more pixelated game you just zoom in (DigDug is played at scale 3.0f)

ResourceManager: Loads in textures and fonts. Holds a default font.

SceneManager: Gets the calls from the main game loop in Minigin. Sends them to the currently active scene.


Grid-based editable terrain: Works with agents and obstacles. Has agents that can carve and agents that can not carve. Has little walls on the sides of the tunnels that have not been carved fully through. These thin walls can be traversed by agents that can pass through them (bool). Has pathfinding for agents using AI. Has ghost ability for Pookas and Fygars. Can work with layers, which are nothing but a size_t generated upon request, to check how deep a certain cell is (used for scoring).

GitHub link: https://github.com/Shadania/Minigin-DigDug
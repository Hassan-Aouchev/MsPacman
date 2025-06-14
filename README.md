# Ms. Pac-Man Engine

A game engine implementation built using the Minigin project as a foundation, incorporating design patterns from Robert Nystrom's "Game Programming Patterns" book.

## Design Patterns Implementation

### GameObject and component pattern
The most important pattern in my opinion. This lays the foundation of how objects should be created inside a scene, how they should be updated and rendered. Each Game object is created and added to the scene with the possibility of setting the parent to another gameobject.

The components that execute the actual (game) logic can then be added to any of the gameobjects using a template class.

Gameobjects use dirty flags to change the world position if the local position is changed to make sure that unnecessary calculations are not done until really needed.

### Command Pattern
The command pattern handles input binding where user actions must be registered through various input methods (keyboard and controller). The implementation uses:

- **Controller abstraction** that encapsulates the XInput library using the pImpl idiom
- **Input processing** where each input is processed and stored within the InputManager class
- **Action decoupling** separating input detection from action execution

### Observer Pattern
Communication between game objects follows a one-way notification system using the observer pattern. For instance, when the player dies, both the HUD and Ghost entities need to be notified.

**Implementation details:**
- Objects sending events contain a Subject component
- Objects receiving events inherit from the Observer class
- While inheritance is typically avoided for this pattern, it's acceptable here since components should not inherit from one another
- Alternative composition-based approach could be implemented to eliminate inheritance entirely

### Service Locator
The engine employs a service locator pattern to manage services, currently implemented for audio functionality. This approach provides several advantages over singleton patterns:

- **Improved testability** - services can be easily mocked or replaced
- **Enhanced flexibility** - sound can be disabled using a null sound service, or enhanced with logging capabilities
- **Better maintainability** - different SoundService implementations can derive from an abstract SoundService base class

#### the sound service

Just a quick note: To make sure the game frames is not affected by the sound service all sound is done on a separate thread. This also uses an event queue although it's not reusable throughout the engine.
Also it keeps the loaded sounds in cache if it's frequently used so no useless loading and decoding is needed. And Finally the SDL library is also pImpled away.

#### Example
→ Player shoots → 'PlaySound("laser.wav")' called 30 times/second

→ First call: Loads and caches "laser.wav" (slow)

→ Subsequent calls: Uses cached version (fast)

This is probably overkill for ms pacman but would probably be needed for any game engine.

### State Management
The engine implements a hybrid approach combining hierarchical state machines with finite state machines.

**Core functionality:**
- Uses finite state machine principles as the foundation
- Supports temporary state interruptions through hierarchical state management
- Allows pausing the current state machine to handle time-sensitive states

**Known limitations:**
- Manual state popping is required, which can be error-prone
- Potential for losing track of current state position
- **Proposed improvement:** Temporary pushed states should utilize condition-based transitions similar to standard FSM behavior
```cpp
// Current manual and error-prone
stateMgr.PushState(new Whatever());
// ... somewhere else ...
stateMgr.PopState();  // Easy to forget or call at wrong time
```

## Links and references

[avadea](https://github.com/avadae) - [Minigin project](https://github.com/avadae/minigin)

**Nystrom, R.** - (2014). *Game Programming Patterns*.
